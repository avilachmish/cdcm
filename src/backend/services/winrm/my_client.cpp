//=====================================================================================================================
// Trustwave ltd. @{SRCH}
//														my_client.cpp
//
//---------------------------------------------------------------------------------------------------------------------
// DESCRIPTION: 
//
//
//---------------------------------------------------------------------------------------------------------------------
// By      : Assaf Cohen
// Date    : 10/6/19
// Comments:
//=====================================================================================================================
//                          						Include files
//=====================================================================================================================
#include <OpenWsmanClient.h>
#include <iostream>
#include <vector>
int main()
{
    WsmanClientNamespace::OpenWsmanClient cli("192.168.120.233",5985,"/wsman","http","Basic","administrator","Finjan123");
    cli.Identify();
    NameValuePairs nvp={{"Name","winmgmt"}};

    const std::string r{"http://schemas.microsoft.com/wbem/wsman/1/wmi/root/cimv2/Win32_Service"};
    std::cerr << cli.Get(r,addressof(nvp));
    const std::string r2{"http://schemas.dmtf.org/wbem/wscim/1/cim-schema/2/CIM_ComputerSystem"};

    std::cerr << "1,\n";
    std::vector<std::string> res2;
    cli.Enumerate(r,res2);
    for(const auto e : res2)
    {
        std::cerr << e <<", ";
    }
    std::cerr << std::endl;
}

