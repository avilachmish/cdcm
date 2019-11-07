//
// Created by rfrenkel on 11/5/2019.
//

#ifndef TRUSTWAVE_SERVICES_WMI_WQL_ACTION_HPP_
#define TRUSTWAVE_SERVICES_WMI_WQL_ACTION_HPP_
//=====================================================================================================================
//                          						Include files
//=====================================================================================================================
#include "../../common/action.hpp"
//=====================================================================================================================
//                          						namespaces
//=====================================================================================================================
namespace trustwave {

class session;

class WQL_Query_Action : public Action_Base {

    static Dispatcher<Action_Base>::Registrator m_registrator;

public:
    WQL_Query_Action() : Action_Base("wql_query", "wql_query"){}

    int act(boost::shared_ptr <session> sess, std::shared_ptr<action_msg> action, std::shared_ptr<result_msg> res) override;

};

} //end namespace trustwave
#endif //TRUSTWAVE_SERVICES_WMI_WQL_ACTION_HPP_
