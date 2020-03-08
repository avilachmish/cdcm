//=====================================================================================================================
// Trustwave ltd. @{SRCH}
//														enumerate.cpp
//
//---------------------------------------------------------------------------------------------------------------------
// DESCRIPTION:
//
//
//---------------------------------------------------------------------------------------------------------------------
// By      : Assaf Cohen
// Date    : 12/30/19
// Comments:
//=====================================================================================================================
//                          						Include files
//=====================================================================================================================
#include "enumerate.hpp"
#include "protocol/msg_types.hpp"
#include "session.hpp"
#include "singleton_runner/authenticated_scan_server.hpp"
#include "client/winrm_client.hpp"
#include "xml2json/include/xml2json.hpp"

using trustwave::Winrm_Enumerate_Action;
auto push_back = [](tao::json::events::to_value& c, const std::string& k, const std::string& v) {
    c.begin_object();
    c.key(k);
    c.string(v);
    c.member();
    c.end_object();
    c.element();
};
int Winrm_Enumerate_Action::act(boost::shared_ptr<session> sess, std::shared_ptr<action_msg> action,
                                std::shared_ptr<result_msg> res)
{
    if(!sess || (sess && sess->id().is_nil())) {
        res->res("Error: Session not found");
        return -1;
    }
    try {
        winrm_client cli(sess->remote(), 5985, "/wsman", "http", "Basic", sess->creds().username(),
                         sess->creds().password());
        auto winrm_enumerate_action = std::dynamic_pointer_cast<winrm_action_enumerate_msg>(action);
        trustwave::winrm_filter filt("http://schemas.microsoft.com/wbem/wsman/1/WQL", winrm_enumerate_action->filter_);
        std::vector<std::string> enumerate_res;
        cli.Enumerate(winrm_enumerate_action->uri_, filt, enumerate_res);

        std::vector<tao::json::value> enumerate_res_as_json_value;
        for (size_t i=0; i < enumerate_res.size(); ++i)
        {
            enumerate_res_as_json_value.emplace_back(tao::json::from_string(xml2json(enumerate_res[i].c_str())));
        }

        res->res(enumerate_res_as_json_value);
    }
    catch(const winrm_client_exception& e) {
        res->res(e.what());
        return -1;
    }
    return 0;
}

// instance of the our plugin
static std::shared_ptr<Winrm_Enumerate_Action> instance = nullptr;

// extern function, that declared in "action.hpp", for export the plugin from dll
std::shared_ptr<trustwave::Action_Base> import_action()
{
    return instance ? instance : (instance = std::make_shared<Winrm_Enumerate_Action>());
}

