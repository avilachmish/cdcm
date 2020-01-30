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
using trustwave::Winrm_Get_Action;

int Winrm_Get_Action::act(boost::shared_ptr<session> sess, std::shared_ptr<action_msg> action,
                                std::shared_ptr<result_msg> res)
{
    if(!sess || (sess && sess->id().is_nil())) {
        res->res("Error: Session not found");
        return -1;
    }
    try {
        winrm_client cli(sess->remote(), 5985, "/wsman", "http", "Basic", sess->creds().username(),
                         sess->creds().password());
        auto winrm_get_action = std::dynamic_pointer_cast<winrm_action_get_msg>(action);
        std::vector<std::string> get_res;
        res->res(cli.Get(winrm_get_action->uri_, std::addressof(winrm_get_action->selectors_)));
    }
    catch(const winrm_client_exception& e) {
        res->res(e.what());
        return -1;
    }
    return 0;
}

// instance of the our plugin
static std::shared_ptr<Winrm_Get_Action> instance = nullptr;

// extern function, that declared in "action.hpp", for export the plugin from dll
std::shared_ptr<trustwave::Action_Base> import_action()
{
    return instance ? instance : (instance = std::make_shared<Winrm_Get_Action>());
}
