//=====================================================================================================================
// Trustwave ltd. @{SRCH}
//														start_session.cpp
//
//---------------------------------------------------------------------------------------------------------------------
// DESCRIPTION: @{HDES}
// -----------
//---------------------------------------------------------------------------------------------------------------------
// CHANGES LOG: @{HREV}
// -----------
// Revision: 01.00
// By      : Assaf Cohen
// Date    : 20 May 2019
// Comments:

//=====================================================================================================================
//                          						Include files
//=====================================================================================================================
#include "../internal/start_session.hpp"
#include "../../authenticated_scan_server.hpp"
#include "sessions_container.hpp"
#include "protocol/msg_types.hpp"

using namespace trustwave;

int Start_Session::act(const header& header, std::shared_ptr<action_msg> action, std::shared_ptr<result_msg> res)
{

    std::cout<<"In start session"<<std::endl;
    auto gsact = std::dynamic_pointer_cast<local_start_session_msg>(action);
    trustwave::credentials creds(gsact->domain, gsact->username, gsact->password, gsact->workstation);
    trustwave::session s(gsact->remote, creds);
    authenticated_scan_server::instance().sessions.insert_session(s);

    res->id(action->id());
    res->res(s.idstr());
    return 0;

}

Dispatcher<Action_Base>::Registrator Start_Session::m_registrator(new Start_Session,
                authenticated_scan_server::instance().public_dispatcher);