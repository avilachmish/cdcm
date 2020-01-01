//
// Created by rfrenkel on 1/1/2020.
//

#include "reg_enum_key.hpp"

#include <tuple>
#include <string>
#include <boost/tokenizer.hpp>

#include "../wmi_client.hpp"
#include "session.hpp"
#include "singleton_runner/authenticated_scan_server.hpp"


using namespace trustwave;

/********************************************************
 *
 *  return value: -1 for error, 0 for success
 *********************************************************/
int WMI_Reg_Enum_key_Action::act(boost::shared_ptr<session> sess, std::shared_ptr<action_msg> action, std::shared_ptr<result_msg> res)
{
    if (!sess || (sess && sess->id().is_nil())){
        AU_LOG_ERROR("Session not found");
        res->res("Error: Session not found");
        return -1;
    }

    auto wmi_reg_enum_key_action = std::dynamic_pointer_cast<wmi_action_wmi_reg_enum_key_msg>(action);
    if (!wmi_reg_enum_key_action){
        AU_LOG_ERROR("Failed dynamic cast");
        res->res("Error: Internal error");
        return -1;
    }

    auto client = std::dynamic_pointer_cast <wmi_client>(sess->get_client <wmi_client>(cdcm_client_type::WMI_CLIENT));
    if (!client){
        AU_LOG_ERROR("Failed dynamic cast");
        res->res("Error: Failed dynamic cast");
        return -1;
    }

    //rotem: TODO: think how to distingush between our error and legit error
    auto connect_result = client->connect_reg(*sess, wmi_reg_enum_key_action->wmi_namespace);
    if (false == std::get<0>(connect_result) )
    {
        AU_LOG_ERROR("failed to connect to the asset");
        res->res(std::string("Error: Failed to connect to the asset"));
        return -1;
    }

    auto query_result = client->registry_enum_key(std::stoi(wmi_reg_enum_key_action->hive), wmi_reg_enum_key_action->key);
    if (false == std::get<0>(query_result) )
    {
        AU_LOG_ERROR("failed to get wmi registry response");
        res->res(std::string(std::get<1>(query_result)));
        return -1;
    }

    std::string wmi_reg_response = std::get<1>(query_result);

    res->res(wmi_reg_response);
    return 0;
}


// instance of the our plugin
static std::shared_ptr<WMI_Reg_Enum_key_Action> instance = nullptr;

// extern function, that declared in "action.hpp", for export the plugin from dll
std::shared_ptr<Action_Base> import_action() {
    return instance ? instance : (instance = std::make_shared<WMI_Reg_Enum_key_Action>());
}