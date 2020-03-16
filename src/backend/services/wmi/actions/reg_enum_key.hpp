//
// Created by rfrenkel on 1/1/2020.
//

#ifndef TRUSTWAVE_SERVICES_WMI_REG_ENUM_KEY_ACTION_HPP_
#define TRUSTWAVE_SERVICES_WMI_REG_ENUM_KEY_ACTION_HPP_
//=====================================================================================================================
//                          						Include files
//=====================================================================================================================
#include "action.hpp"
#include "protocol/msg_types.hpp"
#include "protocol/protocol.hpp"

//=====================================================================================================================
//                          						namespaces
//=====================================================================================================================
namespace trustwave {

    struct wmi_action_wmi_reg_enum_key_msg: public action_msg
    {
        static constexpr std::string_view act_name{"wmi_reg_enum_key"};

        wmi_action_wmi_reg_enum_key_msg() : action_msg(act_name) {

        }


        std::string hive;
        std::string key;
        std::string wmi_namespace;  //rotem TODO: do we bneed name space for registry actions? if not, remove

    };
} // end of namespace trustwave

namespace tao::json {

    template<>
    struct traits<trustwave::wmi_action_wmi_reg_enum_key_msg> :
            binding::object<
                    binding::inherit<traits<trustwave::action_msg> >,
                    TAO_JSON_BIND_OPTIONAL( "hive", &trustwave::wmi_action_wmi_reg_enum_key_msg::hive ),
                    TAO_JSON_BIND_REQUIRED( "key", &trustwave::wmi_action_wmi_reg_enum_key_msg::key ),
                    TAO_JSON_BIND_OPTIONAL( "wmi_namespace", &trustwave::wmi_action_wmi_reg_enum_key_msg::wmi_namespace ) >
    {
        TAO_JSON_DEFAULT_KEY( trustwave::wmi_action_wmi_reg_enum_key_msg::act_name.data() );


        template< template< typename... > class Traits >
        static trustwave::wmi_action_wmi_reg_enum_key_msg as( const tao::json::basic_value< Traits >& v )
        {
            trustwave::wmi_action_wmi_reg_enum_key_msg result;
            const auto o = v.at(trustwave::wmi_action_wmi_reg_enum_key_msg::act_name);
            result.id_ = o.at( "id" ).template as< std::string >();
            result.hive = o.at( "hive" ).template as< std::string >();
            result.key = o.at( "key" ).template as< std::string >();
            result.wmi_namespace = o.at( "wmi_namespace" ).template as< std::string >();

            return result;
        }

    };


} // end of namespace tao::json

class session;
class result_msg;
namespace trustwave {

    class WMI_Reg_Enum_key_Action : public Action_Base {

    public:
        WMI_Reg_Enum_key_Action() : Action_Base(wmi_action_wmi_reg_enum_key_msg::act_name){}

        action_status act(boost::shared_ptr <session> sess, std::shared_ptr<action_msg> action, std::shared_ptr<result_msg> res) override;
        [[nodiscard]] std::shared_ptr<action_msg> get_message(const tao::json::value& v) const override
        {
            return v.as<std::shared_ptr<wmi_action_wmi_reg_enum_key_msg>>();
        }
    };

} //end namespace trustwave

#endif //TRUSTWAVE_SERVICES_WMI_REG_ENUM_KEY_ACTION_HPP_
