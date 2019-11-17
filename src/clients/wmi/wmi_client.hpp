//
// Created by rfrenkel on 10/23/2019.
//

#ifndef TRUSTWAVE_CLIENTS_WMI_WMI_CLIENT_HPP
#define TRUSTWAVE_CLIENTS_WMI_WMI_CLIENT_HPP

#ifdef __cplusplus
extern "C" {
#endif

#include "wmi/openvas_wmi_interface.h"
//#include <samba/auth/credentials/credentials.c>

#ifdef __cplusplus
}
#endif

#include <string>
#include <tuple>
#include "../../common/client.hpp"

namespace trustwave {

    class session; //forward declaration

    class wmi_client :public cdcm_client {

    public:
        wmi_client();
        ~wmi_client();

        std::tuple<bool, std::string> connect(const session& session, std::string wmi_namespace);
        std::tuple<bool, std::string> query_remote_asset(std::string wql_query);
    private:
        int init();

    private:
        std::string wql_query;
        char delim='|';
        std::string wmi_namespace;
        WMI_HANDLE wmi_handle;
    };


} //end namespace trustwave
#endif //TRUSTWAVE_CLIENTS_WMI_WMI_CLIENT_HPP
