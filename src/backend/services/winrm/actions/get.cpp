//=====================================================================================================================
// Trustwave ltd. @{SRCH}
//														get.cpp
//
//---------------------------------------------------------------------------------------------------------------------
// DESCRIPTION:
//
//
//---------------------------------------------------------------------------------------------------------------------
// By      : Assaf Cohen
// Date    : 1/22/20
// Comments:
//=====================================================================================================================
//                          						Include files
//=====================================================================================================================
#include "get.hpp"
#include "protocol/msg_types.hpp"
#include "session.hpp"
#include "singleton_runner/authenticated_scan_server.hpp"
#include "client/winrm_client.hpp"
#include "xml2json/include/xml2json.hpp"

using trustwave::Winrm_Get_Action;
using action_status = trustwave::Action_Base::action_status;
action_status Winrm_Get_Action::act(boost::shared_ptr<session> sess, std::shared_ptr<action_msg> action,
                                std::shared_ptr<result_msg> res)
{
    if(!sess || (sess && sess->id().is_nil())) {
        res->res("Error: Session not found");
        return action_status::FAILED;
    }
    try {
        winrm_client cli(sess->remote(), 5985, "/wsman", "http", "Basic", sess->creds().username(),
                         sess->creds().password());
        auto winrm_get_action = std::dynamic_pointer_cast<winrm_action_get_msg>(action);
        std::vector<std::string> get_res;

        res->res( tao::json::from_string(xml2json(cli.Get(winrm_get_action->uri_, std::addressof(winrm_get_action->selectors_)).c_str())) );


    }
    catch(const winrm_client_exception& e) {
        res->res(e.what());
        return action_status::FAILED;
    }
    return action_status::SUCCEEDED;
}

// instance of the our plugin
static std::shared_ptr<Winrm_Get_Action> instance = nullptr;

// extern function, that declared in "action.hpp", for export the plugin from dll
std::shared_ptr<trustwave::Action_Base> import_action()
{
    return instance ? instance : (instance = std::make_shared<Winrm_Get_Action>());
}
