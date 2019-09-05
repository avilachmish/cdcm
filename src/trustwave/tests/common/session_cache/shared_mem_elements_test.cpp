//=====================================================================================================================
// Trustwave ltd. @{SRCH}
//														shared_mem_elements_test.cpp
//
//---------------------------------------------------------------------------------------------------------------------
// DESCRIPTION: 
//
//
//---------------------------------------------------------------------------------------------------------------------
// By      : ascohen
// Date    : 8/27/19
// Comments:
//=====================================================================================================================
//                          						Include files
//=====================================================================================================================
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>
#include "shmem_fixtures.hpp"
#ifdef __cplusplus

extern "C" {
#endif

#include "auth/credentials/credentials.h"
#ifdef __cplusplus
}
#endif
namespace utf = boost::unit_test;
using  namespace trustwave;


BOOST_AUTO_TEST_SUITE(Shared_Mem_Elements)

    BOOST_FIXTURE_TEST_CASE(Credentials_Domain,OneInCache) {
        auto f=sessions->get_session_by<shared_mem_sessions_cache::remote>("192.168.0.1");
        BOOST_REQUIRE(f);
        auto creds = f->creds();
        std::string domain= cli_credentials_get_domain(creds);

        BOOST_TEST(domain == "WORKGROUP");
    }
    BOOST_FIXTURE_TEST_CASE(Credentials_Username,OneInCache) {
        auto f=sessions->get_session_by<shared_mem_sessions_cache::remote>("192.168.0.1");
        BOOST_REQUIRE(f);
        auto creds = f->creds();
        std::string username = cli_credentials_get_username(creds);
        BOOST_TEST(
        username=="admin1");
    }
    BOOST_FIXTURE_TEST_CASE(Credentials_Password,OneInCache) {
        auto f=sessions->get_session_by<shared_mem_sessions_cache::remote>("192.168.0.1");
        BOOST_REQUIRE(f);
        auto creds = f->creds();

        std::string password = cli_credentials_get_password(creds);

        BOOST_TEST(password == "pass1");
    }
    BOOST_FIXTURE_TEST_CASE(Credentials_Workstation,OneInCache) {
        auto f=sessions->get_session_by<shared_mem_sessions_cache::remote>("192.168.0.1");
        BOOST_REQUIRE(f);
        auto creds = f->creds();
        std::string workstation = cli_credentials_get_workstation(creds);
        BOOST_TEST(workstation == "ws1");
    }
    BOOST_FIXTURE_TEST_CASE(Session_Remote,OneInCache) {
        auto f=sessions->get_session_by<shared_mem_sessions_cache::remote>("192.168.0.1");
        BOOST_REQUIRE(f);
        BOOST_TEST(f->remote() == "192.168.0.1");
    }

BOOST_AUTO_TEST_SUITE_END()