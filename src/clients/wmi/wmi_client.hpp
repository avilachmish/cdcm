//
// Created by rfrenkel on 10/23/2019.
//

#ifndef TRUSTWAVE_CLIENTS_WMI_WMI_CLIENT_HPP
#define TRUSTWAVE_CLIENTS_WMI_WMI_CLIENT_HPP

#ifdef __cplusplus
extern "C" {
#endif

#include "wmi/openvas_wmi_interface.h"

#ifdef __cplusplus
}
#endif

#include <string>

namespace trustwave {
    class wmi_client :public cdcm_client {

    public:
        wmi_client();
        ~wmi_client();

        int connect(const session& session);
        std::string query_remote(); //rotem TODO: find another name
    private:
        int init();

    private:
        std::string wql_query;
        char delim='|';
        std::string wmi_namespace;
        WMI_HANDLE handle;
    };


} //end namespace trustwave
#endif //TRUSTWAVE_CLIENTS_WMI_WMI_CLIENT_HPP
