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
    AU_LOG_ERROR("rotem WMI_Reg_Enum_key_Action:"); //rotem to delete

    std::cout << "rotem WMI_Reg_Enum_key_Action: " << std::endl; //rotem to delete

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
    trustwave::wmi_client client;

    std::cout << "rotem bye: " << std::endl; //rotem to delete

    //rotem: TODO: think how to distingush between our error and legit error
    auto connect_result = client.connect_reg(*sess, wmi_reg_enum_key_action->wmi_namespace);
    if (false == std::get<0>(connect_result) )
    {
        AU_LOG_ERROR("failed to connect to the asset");
        res->res(std::string("Error: Failed to connect to the asset"));
        return -1;
    }

    std::cout << "rotem hive: "  << wmi_reg_enum_key_action->hive << std::endl; //rotem to delete
    std::cout << "rotem hive: " << std::stoul(wmi_reg_enum_key_action->hive, 0, 16) << std::endl; //rotem to delete

    auto query_result = client.registry_enum_key(0x80000002, wmi_reg_enum_key_action->key);
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