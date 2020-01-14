//=====================================================================================================================
// Trustwave ltd. @{SRCH}
//														action_manager.cpp
//
//---------------------------------------------------------------------------------------------------------------------
// DESCRIPTION: 
//
//
//---------------------------------------------------------------------------------------------------------------------
// By      : Assaf Cohen
// Date    : 12/4/19
// Comments:
//=====================================================================================================================
//                          						Include files
//=====================================================================================================================
#include "action_manager.hpp"
#include "dispatcher.hpp"
#include "action.hpp"
#include "shared_library.hpp"
#include "singleton_runner/authenticated_scan_server.hpp"
using namespace trustwave;

std::vector<std::shared_ptr<shared_library>> action_manager::load(const boost::filesystem::path& p_dir,Dispatcher<Action_Base>& d) {
    boost::system::error_code ec;
    boost::filesystem::directory_iterator p(p_dir, ec), end_iter;
    std::vector<std::shared_ptr<shared_library>> sl_vec;
    for (; p != end_iter; ++p) {
        auto sl = std::make_shared<trustwave::shared_library >(p->path());
        auto actionl = sl->get<import_action_cb_t>("import_action");
        if(actionl)
        {
            auto action=actionl();
            d.register1(action);
            sl_vec.push_back(sl);
            AU_LOG_DEBUG("%s loaded.",p->path().filename().string().c_str());
        }
        else
        {
            AU_LOG_ERROR("%s failed to load.",p->path().filename().string().c_str());
            sl->close();
        }

    }
    return std::move(sl_vec);
}