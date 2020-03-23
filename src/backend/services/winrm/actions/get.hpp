//=====================================================================================================================
// Trustwave ltd. @{SRCH}
//														get.hpp
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
#ifndef SRC_BACKEND_SERVICES_WINRM_ACTIONS_GET_HPP
#define SRC_BACKEND_SERVICES_WINRM_ACTIONS_GET_HPP
#include "action.hpp"
#include "protocol/msg_types.hpp"
#include "protocol/protocol.hpp"
#include <iostream>
//=====================================================================================================================
//                                                  namespaces
//=====================================================================================================================
namespace trustwave {
    struct winrm_action_get_msg: public action_msg {
        static constexpr std::string_view act_name{"winrm_get"};
        winrm_action_get_msg(): action_msg(act_name) {}
        std::string uri_;
        std::map<std::string, std::string> selectors_;
    };
} // namespace trustwave
namespace tao::json {
    template<>
    struct traits<trustwave::winrm_action_get_msg>:
        binding::object<binding::inherit<traits<trustwave::action_msg>>,
        TAO_JSON_BIND_REQUIRED("uri", &trustwave::winrm_action_get_msg::uri_),
    TAO_JSON_BIND_REQUIRED("selectors", &trustwave::winrm_action_get_msg::selectors_)> {
    TAO_JSON_DEFAULT_KEY(trustwave::winrm_action_get_msg::act_name.data());

    template<template<typename...> class Traits>
    static trustwave::winrm_action_get_msg as(const tao::json::basic_value<Traits>& v)
    {
        trustwave::winrm_action_get_msg result;
        const auto o = v.at(trustwave::winrm_action_get_msg::act_name);
        result.id_ = o.at("id").template as<std::string>();
        result.uri_ = o.at("uri").template as<std::string>();
        result.selectors_ = o.at("selectors").template as<std::map<std::string, std::string>>();
        return result;
    }
};
} // namespace tao::json
namespace trustwave {

    class Winrm_Get_Action: public Action_Base {
    public:
        Winrm_Get_Action(): Action_Base(trustwave::winrm_action_get_msg::act_name) {}

        action_status act(boost::shared_ptr<session> sess, std::shared_ptr<action_msg>, std::shared_ptr<result_msg>) override;
        [[nodiscard]] std::shared_ptr<action_msg> get_message(const tao::json::value& v) const override
        {
            return v.as<std::shared_ptr<trustwave::winrm_action_get_msg>>();
        }
    };

} // namespace trustwave

#endif // SRC_BACKEND_SERVICES_WINRM_ACTIONS_GET_HPP