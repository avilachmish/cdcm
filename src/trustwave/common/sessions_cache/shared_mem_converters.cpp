//===========================================================================
// Trustwave ltd. @{SRCH}
//								session_converter.cpp
//
//---------------------------------------------------------------------------
// DESCRIPTION: @{HDES}
// -----------
//---------------------------------------------------------------------------
// CHANGES LOG: @{HREV}
// -----------
// Revision: 01.00
// By      : Assaf Cohen
// Date    : 15 Jul 2019
// Comments: 

//===================================================================
//                          Include files
//===================================================================
#include "../../common/sessions_cache/shared_mem_converters.hpp"

#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/set.hpp>

#include "../../common/session.hpp"
#include "../../common/sessions_cache/shared_mem_session.hpp"

#ifdef __cplusplus
extern "C" {
#endif
#include "auth/credentials/credentials.h"
#ifdef __cplusplus
}
#endif

trustwave::sp_session_t trustwave::session_converter::convert(const shared_mem_session &sm)
{
    sp_session_t rv = boost::make_shared <session>();
    if (!rv->id(std::string(sm.uuid_.c_str(), (size_t) sm.uuid_.length()))){
        return sp_session_t();
    }
    rv->remote(std::string(sm.remote_.c_str(), (size_t) sm.remote_.length()));
    rv->creds(std::string(sm.creds_.domain_.c_str(), (size_t) sm.creds_.domain_.length()),
                    std::string(sm.creds_.username_.c_str(), (size_t) sm.creds_.username_.length()),
                    std::string(sm.creds_.password_.c_str(), (size_t) sm.creds_.password_.length()),
                    std::string(sm.creds_.workstation_.c_str(), (size_t) sm.creds_.workstation_.length()));
    return rv;
}

trustwave::shared_mem_session trustwave::session_converter::convert(const sp_session_t &session, const void_allocator &va)
{
    shared_mem_session rv(va);
    rv.uuid_ = String(session->idstr().c_str(), session->idstr().length(), va);
    rv.remote_ = String(session->remote().c_str(), session->remote().length(), va);
    struct cli_credentials* c = session->creds();

    std::string val(cli_credentials_get_domain(c));
    rv.creds_.domain_ = String(val.c_str(), val.length(), va);

    val.assign(cli_credentials_get_username(c));
    rv.creds_.username_ = String(val.c_str(), val.length(), va);

    val.assign(cli_credentials_get_password(c));
    rv.creds_.password_ = String(val.c_str(), val.length(), va);

    val.assign(cli_credentials_get_workstation(c));
    rv.creds_.workstation_ = String(val.c_str(), val.length(), va);
        return rv;
}
