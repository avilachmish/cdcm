//=====================================================================================================================
// Trustwave ltd. @{SRCH}
//														identify.cpp
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
#include "identify.hpp"
#include "protocol/msg_types.hpp"
#include "session.hpp"
#include "singleton_runner/authenticated_scan_server.hpp"
#include "client/winrm_client.hpp"
using trustwave::Winrm_Identify_Action;

int Winrm_Identify_Action::act(boost::shared_ptr<session> sess, std::shared_ptr<action_msg> action,
                                std::shared_ptr<result_msg> res)
{
    if(!sess || (sess && sess->id().is_nil())) {
        res->res("Error: Session not found");
        return -1;
    }
    try {
        winrm_client cli(sess->remote(), 5985, "/wsman", "http", "Basic", sess->creds().username(),
                         sess->creds().password());
        res->res(cli.Identify().first ? "True" : "False");
    }
    catch(const winrm_client_exception& e) {
        res->res(e.what());
        return -1;
    }

    return 0;
}

// instance of the our plugin
static std::shared_ptr<Winrm_Identify_Action> instance = nullptr;

// extern function, that declared in "action.hpp", for export the plugin from dll
std::shared_ptr<trustwave::Action_Base> import_action()
{
    return instance ? instance : (instance = std::make_shared<Winrm_Identify_Action>());
}
