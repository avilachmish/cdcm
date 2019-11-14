//
// Created by rfrenkel on 11/5/2019.
//

#include "wql_action.hpp"

#include <tuple>
#include <string>
#include "../../clients/wmi/wmi_client.hpp"
#include "../../common/session.hpp"
#include "../../common/protocol/msg_types.hpp"
#include "../../common/protocol/protocol.hpp"
#include "../../common/singleton_runner/authenticated_scan_server.hpp"


using namespace trustwave;
/********************************************************
 *
 *  return value: -1 for error, 0 for success
 *********************************************************/
int WQL_Query_Action::act(boost::shared_ptr<session> sess, std::shared_ptr<action_msg> action, std::shared_ptr<result_msg> res)
{
    if (!sess || (sess && sess->id().is_nil())){
        AU_LOG_ERROR("Session not found");
        res->res("Error: Session not found");
        return -1;
    }

    auto wmi_wql_action = std::dynamic_pointer_cast<wmi_action_wql_query_msg>(action);
    if (!wmi_wql_action){
        AU_LOG_ERROR("Failed dynamic cast");
        res->res("Error: Internal error");
        return -1;
    }

    auto client = std::dynamic_pointer_cast <trustwave::wmi_client>(sess->get_client <trustwave::wmi_client>(2)); //rotem: should use enum
    if (!client){
        AU_LOG_ERROR("Failed dynamic cast");
        res->res("Error: Failed dynamic cast");
        return -1;
    }

   // using result = std::tuple<bool,WERROR>;

    auto connect_result = client->connect(*sess, wmi_wql_action->wmi_namespace);
    if (false == std::get<0>(connect_result) )
    {
        AU_LOG_ERROR("failed to connect to the asset");
        res->res(std::string("Error: ")+std::string("Failed to connect"));
        return -1;
    }

    auto query_result = client->query_remote_asset(wmi_wql_action->wql);
    if (false == std::get<0>(query_result) )
    {
        AU_LOG_ERROR("failed to get wql response. Error: %s", std::get<1>(query_result).c_str());
        res->res(std::string("Error: ")+std::string(std::get<1>(query_result)));
        return -1;
    }

    //rotem: take the wql_raw_response, parse and create the json string
    std::string wql_raw_response = std::get<1>(query_result);

    res->res("temp response not parsed: " + wql_raw_response); //rotem TODO: chane this temp return value

    return 0;
}


Dispatcher<Action_Base>::Registrator WQL_Query_Action::m_registrator(new WQL_Query_Action,
                                                                  authenticated_scan_server::instance().public_dispatcher);