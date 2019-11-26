//
// Created by rfrenkel on 10/23/2019.
//

#include "wmi_client.hpp"

#ifdef __cplusplus
extern "C" {
#endif

#include "wmi/openvas_wmi_interface.h"

#include "credentials.h"


#ifdef __cplusplus
}
#endif

#include <iostream> //rotem to delete
#include <memory>
#include <functional>
#include "../../common/session.hpp"

#include "../../common/singleton_runner/authenticated_scan_server.hpp"
#include "../../common/singleton_runner/log_macros.hpp"

using namespace trustwave;

wmi_client::wmi_client()
{

}


wmi_client::~wmi_client()
{

}

/************************************************************
 *
 * @param session       - reference to the session object
 * @param wmi_namespace - wmi namespace, should be known when connecting to the asset
 * @return              - tuple of bool and result string. if bool is false, an error occurred
 ***********************************************************/
std::tuple<bool, std::string> wmi_client::connect(const session& session, std::string wmi_namespace)
{
    //build the argc,argv
    auto credentials = session.creds();
    std::string domain_usr_pass_arg;
    domain_usr_pass_arg.append(((credentials.domain_))).append("/").append(credentials.username_).append("%").append(credentials.password_);

    std::string remote_asset_arg;
    remote_asset_arg.append("//").append(session.remote());

    std::string wmi_namespace_arg;
    if (  wmi_namespace.empty() ) {
        wmi_namespace_arg = "root\\cimv2";
        AU_LOG_DEBUG("wmi_namespace wasn't supplied. using default: root\cimv2");
    }
    else {
        wmi_namespace_arg = wmi_namespace;
    }

    std::string host_arg;
    std::vector<std::string> arguments = { "wmic","-U", domain_usr_pass_arg, remote_asset_arg, wmi_namespace_arg};
    //std::vector<std::string> arguments = { "wmic","-U", "kkkkkkkkk/administrator%finjan123", "//192.168.140.32", "root\\cimv2"};
    std::vector<char*> argv;
    for (const auto& arg : arguments)
        argv.push_back((char*)arg.data());
    argv.push_back(nullptr);

    //connect to the asset
    wmi_handle = wmi_connect(argv.size()-1, argv.data());
    if (!wmi_handle)
    {
        AU_LOG_ERROR("Error: Failed to connect to asset %s", session.remote().c_str());
        return std::make_tuple(false, std::string( "Error: Failed to connect to asset: " + session.remote()) );
    }

    AU_LOG_DEBUG("connection to asset %s succeeded", session.remote().c_str());
    return std::make_tuple(true, std::string("connection to asset " +  session.remote() + " succeeded"));
}

std::tuple<bool, std::string> wmi_client::query_remote_asset(std::string wql_query)
{
    if (wql_query.empty())
    {
        AU_LOG_ERROR("Error: wql query is empty");
        return std::make_tuple(false, "Error: wql query is empty");
    }

    char* response_raw_ptr;
    int ret = wmi_query(wmi_handle, wql_query.data(), &response_raw_ptr);
    std::string response_str(response_raw_ptr);
    //talloc_free(response_raw_ptr);
    //rotem TODO: use unique_ptr to own the resource using a custom deleter
    //rotem TODO: find how i should release response_raw_ptr. when using free i have double release. where the first release came from?
    // also, maybe need to use talloc_free and not only free. see https://linux.die.net/man/3/talloc
    //rotem: TODO: in case of error i don't have access to this info
    if (-1 == ret)
    {
        AU_LOG_DEBUG("wmi query result with an error: %s", response_str.c_str());
        //std::cout << "wmi query result with an error: " << response_str << std::endl; //rotem to delete
        return std::make_tuple(false, "Error: Some wmi error happened");
    }
    //std::cout << "wmi query result: \n" << response_str << std::endl; //rotem to delete
    AU_LOG_DEBUG("wmi query result: \n%s", response_str.c_str());
    return {true, response_str};
}