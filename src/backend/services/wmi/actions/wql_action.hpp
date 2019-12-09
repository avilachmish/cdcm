//
// Created by rfrenkel on 11/5/2019.
//

#ifndef TRUSTWAVE_SERVICES_WMI_WQL_ACTION_HPP_
#define TRUSTWAVE_SERVICES_WMI_WQL_ACTION_HPP_
//=====================================================================================================================
//                          						Include files
//=====================================================================================================================
#include "../../../../common/action.hpp"
//=====================================================================================================================
//                          						namespaces
//=====================================================================================================================
namespace trustwave {

class session;
class result_msg;
class WMI_WQL_Action : public Action_Base {

    static Dispatcher<Action_Base>::Registrator m_registrator;

public:
    WMI_WQL_Action() : Action_Base("wmi_wql", "wmi_wql"){}

    int act(boost::shared_ptr <session> sess, std::shared_ptr<action_msg> action, std::shared_ptr<result_msg> res) override;

};

} //end namespace trustwave
#endif //TRUSTWAVE_SERVICES_WMI_WQL_ACTION_HPP_
