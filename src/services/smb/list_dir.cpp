//=====================================================================================================================
// Trustwave ltd. @{SRCH}
//														list_dir.cpp
//
//---------------------------------------------------------------------------------------------------------------------
// DESCRIPTION: 
//
//
//---------------------------------------------------------------------------------------------------------------------
// By      : Assaf Cohen
// Date    : 11/4/19
// Comments:
//=====================================================================================================================
//                          						Include files
//=====================================================================================================================
#include <unistd.h>
#include <sstream>
#include <iomanip>
#include <string>

#include "../../clients/smb/smb_client.hpp"
#include "list_dir.hpp"
#include "taocpp-json/include/tao/json.hpp"
#include "taocpp-json/include/tao/json/contrib/traits.hpp"
#include "../../common/protocol/msg_types.hpp"
#include "../../common/session.hpp"
#include "../../common/singleton_runner/authenticated_scan_server.hpp"
#include "../../common/wildcards.hpp"

using namespace trustwave;
namespace tao {
    namespace json {
        template<>
        struct traits<trustwave::dirent> : binding::object<
                TAO_JSON_BIND_REQUIRED ("name", &trustwave::dirent::name_),
                TAO_JSON_BIND_REQUIRED ("type", &trustwave::dirent::type_) > {
        };
    }
}

int SMB_List_Dir::act(boost::shared_ptr<session> sess, std::shared_ptr<action_msg> action,
                      std::shared_ptr<result_msg> res) {

    if (!sess || (sess && sess->id().is_nil())) {
        res->res("Error: Session not found");
        return -1;
    }

    auto smb_action = std::dynamic_pointer_cast<smb_list_dir_msg>(action);
    std::string base("smb://");
    base.append(sess->remote()).append("/").append(smb_action->param);
    std::string tmp_name("/tmp/" + sess->idstr() + "-" + action->id());
    trustwave::smb_client rc;
    std::vector<trustwave::dirent> dir_entries;
    if (!rc.list(base.c_str(), dir_entries)) {
        res->res("Error: List Failed");
        return -1;
    }
    if (!smb_action->pattern.empty()) {
        dir_entries.erase(std::remove_if(dir_entries.begin(),
                                         dir_entries.end(),
                                         [&](const trustwave::dirent &fname) -> bool {
                                   return !wildcards::match(
                                           cx::make_string_view(fname.name_.c_str(), fname.name_.length()),
                                           cx::make_string_view(smb_action->pattern.c_str(),
                                                                smb_action->pattern.length()));
                               }),
                          dir_entries.end());
    }
    const tao::json::value v1 = dir_entries;
    res->res(to_string(v1, 2));

    return 0;

}

Dispatcher<Action_Base>::Registrator SMB_List_Dir::m_registrator(new SMB_List_Dir,
                                                                 authenticated_scan_server::instance().public_dispatcher);