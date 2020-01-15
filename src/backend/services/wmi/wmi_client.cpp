//
// Created by rfrenkel on 10/23/2019.
//

#include "wmi_client.hpp"

#include <iostream>
#include <tuple>

#include "../../../common/session.hpp"
#include "../../../common/singleton_runner/authenticated_scan_server.hpp"
#include "../../../common/singleton_runner/log_macros.hpp"

using namespace trustwave;

wmi_client::~wmi_client()
{
    AU_LOG_DEBUG("closing wmi connection to remote asset");
    close_connection();
}

/************************************************************
 *
 * @param session       - reference to the session object
 * @param wmi_namespace - wmi namespace, should be known when connecting to the asset
 * @return              - tuple of bool and result string. if bool is false, an error occurred, otherwise the
 *                        tuple's second will hold the response
 ***********************************************************/
std::tuple<bool, std::string> wmi_client::connect(const session& session, std::string & wmi_namespace)
{
    //build the argc,argv
    auto credentials = session.creds();
    std::string domain_usr_pass_arg;
    constexpr size_t domain_usr_pass_arg_size = 512;
    domain_usr_pass_arg.reserve(domain_usr_pass_arg_size);
    domain_usr_pass_arg.append(((credentials.domain()))).append("/").append(credentials.username()).append("%").append(credentials.password());

    std::string remote_asset_arg;
    constexpr size_t remote_asset_arg_size = 256;
    remote_asset_arg.reserve(remote_asset_arg_size);
    remote_asset_arg.append("//").append(session.remote());

    std::string wmi_namespace_arg ("root\\cimv2");
    if (  !wmi_namespace.empty() ) {
        wmi_namespace_arg = wmi_namespace;
        AU_LOG_DEBUG("wmi_namespace was supplied:  %s", wmi_namespace.c_str());
    }
    else {
        AU_LOG_DEBUG("wmi_namespace was not supplied. using default: root\\cimv2");
    }

    std::vector<char*> argv = { std::string("wmic").data(),
                                std::string("-U").data(),
                                domain_usr_pass_arg.data(),
                                remote_asset_arg.data(),
                                wmi_namespace_arg.data()};
    //connect to the asset
    wmi_handle = wmi_connect(argv.size(), argv.data());
    if (!wmi_handle)
    {
        AU_LOG_ERROR("Error: Failed to connect to asset %s", session.remote().c_str());
        return std::make_tuple(false, std::string( "Error: Failed to connect to asset: " + session.remote()) );
    }

    AU_LOG_DEBUG("connection to asset %s succeeded", session.remote().c_str());
    return std::make_tuple(true, std::string("connection to asset " +  session.remote() + " succeeded"));
}

// rotem TODO: see if the response can arrive from outside as std::string
std::tuple<bool, std::string> wmi_client::query_remote_asset(const std::string & wql_query)
{
    if (wql_query.empty())
    {
        AU_LOG_ERROR("Error: wql query is empty");
        return std::make_tuple(false, "Error: wql query is empty");
    }

    char* response_raw_ptr = nullptr;
    int ret = wmi_query(wmi_handle, wql_query.data(), &response_raw_ptr);
    //rotem,assaf TODO: run with valgrind to discover if the response_raw_ptr is released or i should do that using the talloc_free(response_raw_ptr); ( see https://linux.die.net/man/3/talloc )
    // NOTE: in case of error i can't get the exact error
    if (-1 == ret)
    {
        AU_LOG_DEBUG("wmi query result with an error");
        return std::make_tuple(false, "Error: Some wmi error happened");
    }
    std::string response_str(response_raw_ptr);
    //AU_LOG_DEBUG("wmi query result: \n%s", response_str.c_str());
    return {true, std::move(response_str)};
}

std::tuple<bool, std::string> wmi_client::close_connection()
{
    int ret = wmi_close(wmi_handle);
    if (-1 == ret)
    {
        AU_LOG_ERROR("wmi_close returned with an error");
        return std::make_tuple(false, "Error: wmi_close returned with an error");
    }
    return {true, "wmi_close succeeded"};
}