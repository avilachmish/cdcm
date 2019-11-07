//
// Created by rfrenkel on 10/23/2019.
//

#include "wmi_client.hpp"
#include <iostream> //rotem to delete

#include "../../common/session.hpp"
#include <string>
using namespace trustwave;

wmi_client::wmi_client()
{

}


wmi_client::~wmi_client()
{

}


int wmi_client::connect(const session& session)
{
    //build the argc,argv
    std::string domain_user_pass;
    auto credentials = session.creds();

    std::string domain_usr_pass_arg;
    domain_usr_pass_arg.appe

    domatials.domain << "/" << credentials.username << "%" << credentials.password;
    in_usr_pass << creden
    std::string host_arg
    std::vector<std::string> arguments = { "wmic","-U", domain_user_pass., "//192.168.140.32", "root\\cimv2"};
    std::vector<std::string> arguments = { "wmic","-U", "kkkkkkkkk/administrator%finjan123", "//192.168.140.32", "root\\cimv2"};
    std::vector<char*> argv;
    for (const auto& arg : arguments)
        argv.push_back((char*)arg.data());
    argv.push_back(nullptr);

    WMI_HANDLE handle;
    std::cout << "argc: " << argv.size()-1 << std::endl;
    handle = wmi_connect(argv.size()-1, argv.data());
    if (!handle)
    {
        std::cout<< "error happend" << std::endl;
        //rotem:log error
        return -1;
    }
    std::cout << "connection done" << std::endl;

    std::string wql_query = "select * from Win32_LoggedOnUser";

    std::string result_string;
    char* x ;
    int ret = wmi_query(handle, wql_query.data(), &x);
    std::cout << "result: " << x << std::endl;

    //std::string wql_query = "select * from Win32_Bios";
    //std::string wql_query = "select name 	from Win32_SystemDriver";
    //std::string wql_query = "select * from Win32_LoggedOnUser";
    std::string wql_query = "select Antecedent from Win32_LoggedOnUser";
    std::string result_string;
    char* x ;
    int ret = wmi_query(handle, wql_query.data(), &x);
    std::cout << "result: " << x << std::endl;



    return 0;


}










//int wmi_client::connect(const session& session)
//{
//
//    std::vector<char*> argv;
//    for (const auto& arg : arguments)
//        argv.push_back((char*)arg.data());
//    argv.push_back(nullptr);
//
//    std::cout << "argc: " << argv.size()-1 << std::endl;
//    handle = wmi_connect(argv.size()-1, argv.data());
//    if (!handle) {
//        std::cout << "error happend" << std::endl;
////rotem:log error
//        return -1;
//    }
//    std::cout << "connection done" << std::endl;
//}
