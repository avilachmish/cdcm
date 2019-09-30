//=====================================================================================================================
// Trustwave ltd. @{SRCH}
//														query_value.cpp
//
//---------------------------------------------------------------------------------------------------------------------
// DESCRIPTION: @{HDES}
// -----------
//---------------------------------------------------------------------------------------------------------------------
// CHANGES LOG: @{HREV}
// -----------
// Revision: 01.00
// By      : Assaf Cohen
// Date    : 15 May 2019
// Comments:

//=====================================================================================================================
//                          						Include files
//=====================================================================================================================
#include "query_value.hpp"

#include "../../common/protocol/msg_types.hpp"
#include "../../common/session.hpp"
#include "../../common/singleton_runner/authenticated_scan_server.hpp"
#include "../../clients/registry/registry_client.hpp"
#include "../../clients/registry/registry_value.hpp"
//=====================================================================================================================
//                          						namespaces
//=====================================================================================================================
using namespace trustwave;

int Query_Value_Action::act(boost::shared_ptr <session> sess, std::shared_ptr <action_msg> action, std::shared_ptr <result_msg> res)
{
    if (!sess || (sess && sess->id().is_nil())) {
        res->res("Error: Session not found");
         return -1;
     }


    auto c = client(sess, res);

    if (!c){
        return -1;
    }
    auto qvact = std::dynamic_pointer_cast <reg_action_query_value_msg>(action);
    if (!qvact){
        AU_LOG_ERROR("Failed dynamic cast");
        res->res("Error: internal error");
        return -1;
    }

    if (!c->connect(*sess)){
        AU_LOG_DEBUG("Failed connecting to %s", sess->remote().c_str());
        res->res("Error: Failed to connect");
        return -1;
    }
    if (!std::get <0>(c->open_key(qvact->key_.c_str()))){
        AU_LOG_DEBUG("Failed opening  %s", qvact->key_.c_str());
        res->res("Error: Failed to open key");
        return -1;
    }
    trustwave::registry_value rv;
    c->key_get_value_by_name(qvact->value_.c_str(), rv);
    if (rv.value().empty()) {
        res->res("Error: value is empty");
        AU_LOG_ERROR("Error: value is empty");
    }
    else {
        res->res(rv.value());
        AU_LOG_INFO(rv.value().c_str());
    }
    return 0;

}

Dispatcher <Action_Base>::Registrator Query_Value_Action::m_registrator(new Query_Value_Action,
                authenticated_scan_server::instance().public_dispatcher);