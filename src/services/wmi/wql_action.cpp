//
// Created by rfrenkel on 11/5/2019.
//

#include "wql_action.hpp"
#include "../../clients/wmi/wmi_client.hpp"
#include "../../common/session.hpp"

using namespace trustwave;

int WQL_Query_Action::act(boost::shared_ptr<session> sess, std::shared_ptr<action_msg>action, std::shared_ptr<result_msg> res)
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


    result r=client->connect(*sess);
    if (!std::get <0>(r)){
        AU_LOG_DEBUG("Failed connecting to %s err: ", sess->remote().c_str(),win_errstr(std::get <1>(r)));
        res->res(std::string("Error: ")+std::string(win_errstr(std::get <1>(r))));
        return -1;
    }

    if (!std::get <0>(c->open_key(keact->key_.c_str()))){
        AU_LOG_DEBUG("Failed opening  %s", keact->key_.c_str());
        res->res("False");
    }
    else{
        res->res("True");
    }

    return 0;
}