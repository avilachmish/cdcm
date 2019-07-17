//===========================================================================
// Trustwave ltd. @{SRCH}
//								message_worker.cpp
//
//---------------------------------------------------------------------------
// DESCRIPTION: @{HDES}
// -----------
//---------------------------------------------------------------------------
// CHANGES LOG: @{HREV}
// -----------
// Revision: 01.00
// By      : Assaf Cohen
// Date    : 26 Jun 2019
// Comments: 

//  ---------------------------------------------------------------------
//  Constructor
#include "backend/message_worker.hpp"

#include <zmq.hpp>
#include <iostream>
#include <thread>

#include "../common/client.hpp"
#include "../common/Logger/include/Logger.h"
#include "../common/protocol/protocol.hpp"
#include "../common/singleton_runner/authenticated_scan_server.hpp"
#include "../common/zmq/mdp.hpp"
#include "../common/zmq/zmq_helpers.hpp"
#include "../common/zmq/zmq_message.hpp"
#ifdef __cplusplus
extern "C" {
#endif
#include "../lib/talloc/talloc.h"
#include "lib/tevent/tevent.h"
#ifdef __cplusplus
}
#endif
using namespace trustwave;
message_worker::message_worker(zmq::context_t &ctx) :
                context_(ctx), heartbeat_at_(-1), liveness_(
                                authenticated_scan_server::instance().settings.heartbeat_liveness_), heartbeat_(
                                authenticated_scan_server::instance().settings.heartbeat_interval_), reconnect_(
                                authenticated_scan_server::instance().settings.reconnect_), expect_reply_(false), replied_(
                                0)

{
}

//  ---------------------------------------------------------------------
//  Destructor

message_worker::~message_worker()
{
    std::cerr << replied_ << " client msgs replied" << std::endl;
}

//  ---------------------------------------------------------------------
//  Send message to broker
//  If no _msg is provided, creates one internally
void message_worker::send_to_broker(const char *command, std::string option, zmsg *_msg)
{
    zmsg *msg = _msg ? new zmsg(*_msg) : new zmsg();

    //  Stack protocol envelope to start of message
    if (option.length() != 0) {
        msg->push_front(option.c_str());
    }
    msg->push_front(command);
    msg->push_front(MDPW_WORKER);
    msg->push_front("");

    zmq_helpers::console("I: sending %s to broker", mdps_commands[(int) *command]);
    msg->dump();
    msg->send(*worker_);
    delete msg;
}

//  ---------------------------------------------------------------------
//  Connect or reconnect to broker

void message_worker::connect_to_broker()
{
    worker_.reset(new zmq::socket_t(context_, ZMQ_DEALER));
    int linger = 0;
    worker_->setsockopt(ZMQ_LINGER, &linger, sizeof(linger));
    zmq_helpers::set_id(*worker_);
    worker_->connect(authenticated_scan_server::instance().settings.worker_connect_ep_);
    zmq_helpers::console("I: connecting to broker at %s...",
                    authenticated_scan_server::instance().settings.worker_connect_ep_.c_str());

    //  Register service with broker
    send_to_broker(MDPW_READY, "", NULL);

    //  If liveness hits zero, queue is considered disconnected
    liveness_ = authenticated_scan_server::instance().settings.heartbeat_liveness_;
    heartbeat_at_ = zmq_helpers::clock() + heartbeat_;
}

//  ---------------------------------------------------------------------
//  Send reply, if any, to broker and wait for next request.

zmsg *
message_worker::recv(zmsg *&reply_p)
{
    //  Format and send the reply if we were provided one
    zmsg *reply = reply_p;
    assert(reply || !expect_reply_);
    if (reply) {
        assert(reply_to_.size() != 0);
        reply->dump();
        reply->wrap(reply_to_.c_str(), "");
        reply_to_ = "";
        send_to_broker(MDPW_REPLY, "", reply);
        ++replied_;
        delete reply_p;
        reply_p = 0;
    }
    expect_reply_ = true;

    while (!zmq_helpers::interrupted) {
        zmq::pollitem_t items[] = { { worker_->operator void *(), 0, ZMQ_POLLIN, 0 } };
        zmq::poll(items, 1, heartbeat_);

        if (items[0].revents & ZMQ_POLLIN) {
            zmsg *msg = new zmsg(*worker_);
            zmq_helpers::console("I: received message from broker:");
            msg->dump();
            liveness_ = authenticated_scan_server::instance().settings.heartbeat_liveness_;

            //  Don't try to handle errors, just assert noisily
            assert(msg->parts() >= 3);

            std::basic_string<unsigned char> empty = msg->pop_front();
            assert(empty.compare(reinterpret_cast<const unsigned char *>("")) == 0);

            std::basic_string<unsigned char> header = msg->pop_front();
            zmq_helpers::console("I: input message (%s)", header.c_str());
            assert(header.compare(reinterpret_cast<const unsigned char * >(MDPW_WORKER)) == 0);

            std::string command = reinterpret_cast<const char *>(msg->pop_front().c_str());
            if (command.compare(MDPW_REQUEST) == 0) {
                //  We should pop and save as many addresses as there are
                //  up to a null part, but for now, just save one...
                reply_to_ = msg->unwrap();
                return msg;     //  We have a request to process
            }
            else if (command.compare(MDPW_HEARTBEAT) == 0) {
                //  Do nothing for heartbeats
            }
            else if (command.compare(MDPW_DISCONNECT) == 0) {
                connect_to_broker();
            }
            else {
                zmq_helpers::console("E: invalid input message (%d)", (int) *(command.c_str()));
                msg->dump();
            }
            delete msg;
        }
        else if (--liveness_ == 0) {
            zmq_helpers::console("W: disconnected from broker - retrying...");
            zmq_helpers::sleep(reconnect_);
            connect_to_broker();
        }
        //  Send HEARTBEAT if it's time
        if (zmq_helpers::clock() >= heartbeat_at_) {
            send_to_broker( MDPW_HEARTBEAT, "", NULL);
            heartbeat_at_ += heartbeat_;
        }
    }
    if (zmq_helpers::interrupted)
        printf("W: interrupt received, killing worker...\n");
    return NULL;
}
int message_worker::main_func(size_t id)
{

    LoggerSource::instance()->set_source(::trustwave::logger::worker, id);
    zmq_helpers::version_assert(4, 0);
    zmq_helpers::catch_signals();
    zmq::context_t ctx(1);
    message_worker mw(ctx);
    mw.connect_to_broker();
    using namespace tao::json;
    zmsg *reply = nullptr;
    while (1) {
        zmsg *request = mw.recv(reply);
        if (request == 0) {
            break;              //  Worker was interrupted
        }
        std::string mstr(request->body());
        const auto t1 = from_string(mstr);
        AU_LOG_DEBUG("msg: %s", to_string(t1, 2).c_str());
        auto a1 = t1.as<trustwave::msg>();
        auto res = std::make_shared<trustwave::result_msg>();
        trustwave::res_msg res1;
        res1.hdr = a1.hdr;
        res1.msgs.push_back(res);
        AU_LOG_DEBUG("msgs size is %zu", a1.msgs.size());
        for (auto aa : a1.msgs) {
            std::cerr << "Looking " << aa->name() << std::endl;
            auto act1 = trustwave::authenticated_scan_server::instance().public_dispatcher.find(aa->name());
            std::cerr << "Calling " << aa->name() << std::endl;
            if (-1 == act1->act(a1.hdr, aa, res)) {
                std::cerr << aa->name() << " ERROR" << std::endl;
            }
            std::cerr << aa->name() << " Done" << std::endl;
            AU_LOG_DEBUG("Done %s", res1.msgs[0]->res().c_str());
        }
        const tao::json::value v1 = res1;
        auto sssss = to_string(v1, 2);
        std::cerr << " XXXXX " << sssss << " XXXXX " << std::endl;
        reply = new zmsg;
        reply->append(sssss.c_str());        //  Echo is complex... :-)
    }
    if (zmq_helpers::interrupted) {
        printf("W: interrupt received, shutting down...\n");
    }
return 0;
}
