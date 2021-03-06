//===========================================================================
// Trustwave ltd. @{SRCH}
//								zmq_helpers.cpp
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

#include "zmq_helpers.hpp"

#include <csignal> // for sigaction, sigemptyset, SIGINT
#include <cstdarg> // for va_end, va_list, va_start
#include <cstdio> // for printf, vprintf
#include <ctime> // for localtime, nanosleep, strftime
#include <iomanip> // for operator<<, setfill, setw
#include <iostream> // for basic_ostream::operator<<
#include <zmq.h> // for zmq_version, ZMQ_IDENTITY
#include <zmq.hpp> // for socket_t
using namespace trustwave;
std::string zmq_helpers::set_id(zmq::socket_t& socket)
{
    std::stringstream ss;
    ss << std::hex << std::uppercase << std::setw(4) << std::setfill('0') << within(0x10000) << "-" << std::setw(4)
       << std::setfill('0') << within(0x10000);
    socket.setsockopt(ZMQ_IDENTITY, ss.str().c_str(), ss.str().length());
    return ss.str();
}

void zmq_helpers::version_assert(int want_major, int want_minor)
{
    int major;
    int minor;
    int patch;
    zmq_version(&major, &minor, &patch);
    if(major < want_major || (major == want_major && minor < want_minor)) {
        std::cout << "Current 0MQ version is " << major << "." << minor << std::endl;
        std::cout << "Application needs at least " << want_major << "." << want_minor << " - cannot continue"
                  << std::endl;
        exit(EXIT_FAILURE);
    }
}

//  Return current system clock as milliseconds
std::chrono::time_point<std::chrono::system_clock> zmq_helpers::clock() { return std::chrono::system_clock::now(); }

//  Sleep for a number of milliseconds
void zmq_helpers::sleep(std::chrono::milliseconds msecs)
{
    struct timespec t {
    };
    t.tv_sec = std::chrono::duration_cast<std::chrono::seconds>(msecs).count();
    t.tv_nsec = (msecs.count() % 1000);
    nanosleep(&t, nullptr);
}

void zmq_helpers::console(const char* format, ...)
{
    time_t curtime = std::chrono::system_clock::to_time_t(clock());
    struct tm* loctime = localtime(&curtime);
    auto formatted = new char[20];
    strftime(formatted, 20, "%y-%m-%d %H:%M:%S ", loctime);
    printf("%s", formatted);
    delete[] formatted;

    va_list argptr;
    va_start(argptr, format);
    vprintf(format, argptr);
    va_end(argptr);
    printf("\n");
}

//  ---------------------------------------------------------------------
//  Signal handling
//
//  Call s_catch_signals() in your application at startup, and then exit
//  your main loop if s_interrupted is ever 1. Works especially well with
//  zmq_poll.

bool zmq_helpers::interrupted = false;
void zmq_helpers::signal_handler(int) { interrupted = true; }

void zmq_helpers::catch_signals()
{
    struct sigaction action {
    };
    action.sa_handler = signal_handler;
    action.sa_flags = 0;
    sigemptyset(&action.sa_mask);
    sigaction(SIGINT, &action, nullptr);
    sigaction(SIGTERM, &action, nullptr);
}
