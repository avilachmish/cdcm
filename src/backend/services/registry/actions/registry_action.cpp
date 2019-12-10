//===========================================================================
// Trustwave ltd. @{SRCH}
//								registry_action.cpp
//
//---------------------------------------------------------------------------
// DESCRIPTION: @{HDES}
// -----------
//---------------------------------------------------------------------------
// CHANGES LOG: @{HREV}
// -----------
// Revision: 01.00
// By      : Assaf Cohen
// Date    : 5 Aug 2019
// Comments: 

#include "registry_action.hpp"
#include "../registry_client.hpp"
#include "../../../../common/session.hpp"
#include "../../../../common/singleton_runner/authenticated_scan_server.hpp"
#include "../../../../common/protocol/msg_types.hpp"
using namespace trustwave;
std::shared_ptr <trustwave::registry_client> Registry_Action::client(boost::shared_ptr <session> sess,
                std::shared_ptr <result_msg> res)
{
    //auto c = std::dynamic_pointer_cast <trustwave::registry_client>(sess->get_client <trustwave::registry_client>(0)); //rotem TODO: delete after code review
    auto c = std::dynamic_pointer_cast <trustwave::registry_client>(sess->get_client <trustwave::registry_client>(trustwave::cdcm_client_type::REGISTRY_CLIENT));

    if (!c){
            AU_LOG_ERROR("Failed dynamic cast");
            res->res("Error: Failed dynamic cast");
    }
    return c;
}