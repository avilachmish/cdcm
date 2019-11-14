//
// Created by rfrenkel on 10/23/2019.
//

#include "wmi_client.hpp"

#ifdef __cplusplus
extern "C" {
#endif

#include "wmi/openvas_wmi_interface.h"
#include <samba/auth/credentials/credentials.c>
#include "credentials.h"


#ifdef __cplusplus
}
#endif

#include <iostream> //rotem to delete
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

    //rotem: TODO: maybe better to build with stringstream<< instead of append
    //rotem: use cli_credentials from samba or openvas?
    // rotem: TODO:  anyway, should use the get functions
    std::string domain_usr_pass_arg;
    domain_usr_pass_arg.append(((*credentials.domain))).append("/").append(*credentials.username).append("%").append(*credentials.password);

    std::string remote_asset_arg;
    remote_asset_arg.append("//").append(session.remote());

    string wmi_namespace_arg;
    if ( !wmi_namespace || wmi_namespace.empty() ) {
        wmi_namespace_arg = "root\\cimv2";
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

    std::cout << "argc: " << argv.size()-1 << std::endl; //rotem to delete
    // done with preparing the argc, argv

    //connect to the asset
    wmi_handle = wmi_connect(argv.size()-1, argv.data());
    if (!wmi_handle)
    {
        //rotem:log error
        return std::make_tuple(false, "Error: Failed to connect");
    }

    std::cout << "connection done" << std::endl; //rotem: todo delete


    return std::make_tuple(true, "connection succeeded");
}

std::tuple<bool, std::string> wmi_client::query_remote_asset(std::string wql_query)
{
    //std::string wql_query = "select * from Win32_LoggedOnUser";
    //std::string wql_query = "select * from Win32_Bios";
    //std::string wql_query = "select name 	from Win32_SystemDriver";
    //std::string wql_query = "select * from Win32_LoggedOnUser";
    //std::string wql_query = "select Antecedent from Win32_LoggedOnUser";

    if (!qwl_query)
    {
        //rotem TODO: log an error
        return std::make_tuple(false, "Error: wql query is empty");
    }
    std::string result_string;
    char* x ;
    //int ret = wmi_query(handle, wql_query.data(), &x);
    int ret = wmi_query(wmi_handle, wql_query.data(), &x);
    //rotem: TODO: in case of error i don't have access to this info
    if (-1 == ret)
    {
        //rotem TODO: log an error
        std::cout << "result: " << x << std::endl;
        return std::make_tuple(false, "Error: Some wmi error happened");
    }
    std::cout << "result: " << x << std::endl;
    return {true, std::string(x)};
}