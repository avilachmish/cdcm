//=====================================================================================================================
// Trustwave ltd. @{SRCH}
//														identify.hpp
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
#ifndef SRC_BACKEND_SERVICES_WINRM_ACTIONS_IDENTIFY_HPP
#define SRC_BACKEND_SERVICES_WINRM_ACTIONS_IDENTIFY_HPP
//=====================================================================================================================
//                                                  Include files
//=====================================================================================================================
#include "action.hpp"
#include "protocol/msg_types.hpp"
#include "protocol/protocol.hpp"
#include <iostream>
//=====================================================================================================================
//                                                  namespaces
//=====================================================================================================================
namespace trustwave {
    struct winrm_action_identify_msg: public action_msg {
        static constexpr std::string_view act_name{"winrm_identify"};
        winrm_action_identify_msg(): action_msg(act_name) {}
    };
} // namespace trustwave
namespace tao::json {
    template<>
    struct traits<trustwave::winrm_action_identify_msg>:
        binding::object<binding::inherit<traits<trustwave::action_msg>>> {
    TAO_JSON_DEFAULT_KEY(trustwave::winrm_action_identify_msg::act_name.data());

    template<template<typename...> class Traits>
    static trustwave::winrm_action_identify_msg as(const tao::json::basic_value<Traits>& v)
    {
        trustwave::winrm_action_identify_msg result;
        const auto o = v.at(trustwave::winrm_action_identify_msg::act_name);
        result.id_ = o.at("id").template as<std::string>();
        return result;
    }
};
} // namespace tao::json
namespace trustwave {

    class Winrm_Identify_Action: public Action_Base {
    public:
        Winrm_Identify_Action(): Action_Base(trustwave::winrm_action_identify_msg::act_name) {}

        int act(boost::shared_ptr<session> sess, std::shared_ptr<action_msg>, std::shared_ptr<result_msg>) override;
        [[nodiscard]] std::shared_ptr<action_msg> get_message(const tao::json::value& v) const override
        {
            return v.as<std::shared_ptr<trustwave::winrm_action_identify_msg>>();
        }
    };

} // namespace trustwave
#endif // SRC_BACKEND_SERVICES_WINRM_ACTIONS_IDENTIFY_HPP