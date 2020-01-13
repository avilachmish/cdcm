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
#include <tuple>

#include "../../../common/client.hpp"

namespace trustwave {

    class session; //forward declaration

    class wmi_client :public cdcm_client {

    public:
        wmi_client() = default; // c'tor
        ~wmi_client() override; // d'tor
        wmi_client(const wmi_client &) = delete; // delete copy c'tor
        wmi_client operator=(const wmi_client &) = delete; // delete operator=
        wmi_client(wmi_client &&) = default; //move c'tor
        wmi_client operator=(const wmi_client &&) = delete; // delete move operator=

        std::tuple<bool, std::string> connect(const session& session, std::string & wmi_namespace);
        std::tuple<bool, std::string> connect_reg(const session& session, std::string & wmi_namespace);
        std::tuple<bool, std::string> query_remote_asset(const std::string & wql_query);
        std::tuple<bool, std::string> registry_enum_key(uint32_t hive, const std::string & key);

        std::tuple<bool, std::string> close_connection();

    private:
        WMI_HANDLE wmi_handle = nullptr;
    };


} //end namespace trustwave
#endif //TRUSTWAVE_CLIENTS_WMI_WMI_CLIENT_HPP
