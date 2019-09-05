//===========================================================================
// Trustwave ltd. @{SRCH}
//								mdclient2.cpp
//
//---------------------------------------------------------------------------
// DESCRIPTION: @{HDES}
// -----------
//---------------------------------------------------------------------------
// CHANGES LOG: @{HREV}
// -----------
// Revision: 01.00
// By      : Assaf Cohen
// Date    : 23 Jun 2019
// Comments: 

//
//  Majordomo Protocol client example - asynchronous
//  Uses the mdcli API to hide all MDP aspects
//
//  Lets us 'build mdclient' and 'build all'
//
//     Andreas Hoelzlwimmer <andreas.hoelzlwimmer@fh-hagenberg.at>
//

#include "mdcliapi2.hpp"
#include <thread>
#include <boost/uuid/uuid.hpp>         // streaming operators etc.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#include <boost/uuid/random_generator.hpp>  // for random_generator
#pragma GCC diagnostic pop
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.
#include <functional>
#include <vector>

#include "../../common/protocol/protocol.hpp"
std::vector<std::function<std::string(std::string)>> msgs1 ={
[&](std::string id) ->std::string {return
                    R"(
{      
"H":
    {
        "session_id" : ")"
                    + id
                    + R"("
    },
"msgs":
    [

        {
                "enumerate" :
                {
                    "id": ")"
                    + boost::uuids::to_string(boost::uuids::random_generator()())
                    + R"(",
                    "key":"SOFTWARE\\\\Microsoft\\\\Windows NT\\\\CurrentVersion"
                    
                }
        }
    ]
})";},
[&](std::string id) ->std::string {return
                    R"(
{      
"H":
    {
        "session_id" : ")"
                    + id
                    + R"("
    },
"msgs":
    [

        {
                "key_exists" :
                {
                    "id": ")"
                    + boost::uuids::to_string(boost::uuids::random_generator()())
                    + R"(",
                    "key":"SOFTWARE\\\\Microsoft\\\\Windows NT\\\\CurrentVersion"
                    
                }
        }
    ]
})";}};
/*
 *                  {
 "get_file" :
 {
 "id": ")"
 + act_id2
 + R"(",
 "param":"ADMIN$/winhlp32.exe"
 }
 },
 {
 "get_remote_file_version" :
 {
 "id": ")"
 + act_id3
 + R"(",
 "param":"ADMIN$/winhlp32.exe"
 }
 },
 */
static void enumf( std::string msg)
{
    mdcli session("tcp://127.0.0.1:5555", 1);

    zmsg *reply = session.send_and_recv(msg);
    if (reply) {
        std::cout << reply->body() << std::endl;
        delete reply;
    }

}
static void fc(int )
{
    mdcli session("tcp://127.0.0.1:5555", 1);
    auto act_id1 = boost::uuids::random_generator()();
    std::string get_session_m =
                    R"(
          { 
               "H":
               {
                   "session_id" : ")"
                                    + std::string("N/A")
                                    + R"(" 
               },
               "msgs":
                   [

                       {
                           "start_session" :
                           {
                               "id": ")"
                                    + boost::uuids::to_string(act_id1)
                                    + R"(",
                               "remote":"192.168.140.32",
                               "domain":"WORKGROUP",
                               "username":"administrator",
                               "password":"finjan123",
                               "workstation":"SSS"
                           }
                       }
                   ]
           })";

    zmsg *reply = session.send_and_recv(get_session_m);

    if (reply) {
        std::cout << reply->body() << std::endl;
        using namespace tao::json;
        std::string mstr(reply->body());
        const auto t1 = from_string(mstr);
        printf("msg: %s", to_string(t1, 2).c_str());

        auto a1 = t1.as<trustwave::res_msg>();
        auto act_id4 = boost::uuids::to_string(boost::uuids::random_generator()());
        auto new_session_id = a1.msgs[0]->res();
        delete reply;
        reply = nullptr;

        std::vector<std::thread> tp;
        for (unsigned int i = 0; i < 2; ++i)        //context+broker
                        {
            tp.push_back(std::move(std::thread(enumf, msgs1[i % msgs1.size()](new_session_id))));

        }
        for (unsigned int i = 0; i < tp.size(); i++) {
            tp.at(i).join();
        }
        std::string actions =
                        R"(
          {      
            "H":
                {
                    "session_id" : ")"
                                        + new_session_id
                                        + R"("
                },
            "msgs":
                [
   
                    {
                            "enumerate" :
                            {
                                "id": ")"
                                        + act_id4
                                        + R"(",
                                "key":"SOFTWARE\\\\Microsoft\\\\Windows NT\\\\CurrentVersion"
                                
                            }
                    }
                ]
        })";
        printf("Request is:\n%s\n", actions.c_str());

        zmsg *reply = session.send_and_recv(actions);
        if (reply) {
            std::cout << reply->body() << std::endl;
            auto act_id1 = boost::uuids::to_string(boost::uuids::random_generator()());
            auto new_session_id = a1.msgs[0]->res();
            delete reply;
            reply = nullptr;
            std::string actions =
                            R"(
                          {      
                            "H":
                                {
                                    "sessio n_id" : ")"
                                            + new_session_id
                                            + R"("
                                },
                            "msgs":
                                [
                   
                                    {
                                            "key_exists" :
                                            {
                                                "id": ")"
                                            + act_id1
                                            + R"(",
                                                "key":"SOFTWARE\\\\Microsoft\\\\Windows NT\\\\CurrentVersion"
                                                
                                            }
                                    }
                                ]
                        })";
            printf("Request is:\n%s\n", actions.c_str());

            zmsg *reply = session.send_and_recv(actions);
            if (reply) {
                std::cout << reply->body() << std::endl;

            }
            else {
                printf("Break\n");

            }

        }
        else {
            printf("Break\n");

        }

    }
}
int main(int , char *[])
{
    std::vector<std::thread> tp;
    for (unsigned int i = 0; i < 1; ++i)        //context+broker
                    {
        tp.push_back(std::move(std::thread(fc, 1)));

    }
    for (unsigned int i = 0; i < tp.size(); i++) {
        tp.at(i).join();
    }

    return 0;
}