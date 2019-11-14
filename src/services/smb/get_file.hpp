//=====================================================================================================================
// Trustwave ltd. @{SRCH}
//														get_file.hpp
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

#ifndef TRUSTWAVE_SERVICES_SMB_ACTIONS_SMB_ACTION_HPP_
#define TRUSTWAVE_SERVICES_SMB_ACTIONS_SMB_ACTION_HPP_
//=====================================================================================================================
//                          						Include files
//=====================================================================================================================
#include "../../common/action.hpp"
//=====================================================================================================================
//                          						namespaces
//=====================================================================================================================
namespace trustwave {

class SMB_Get_File: public Action_Base
{
    static Dispatcher<Action_Base>::Registrator m_registrator;

public:
    SMB_Get_File() :
                    Action_Base("get_file", "get_file")
    {
    }

    int act(boost::shared_ptr <session> sess, std::shared_ptr<action_msg>, std::shared_ptr<result_msg>) override;
};

}

#endif /* TRUSTWAVE_SERVICES_SMB_ACTIONS_SMB_ACTION_HPP_ */
