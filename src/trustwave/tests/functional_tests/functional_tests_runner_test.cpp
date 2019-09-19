//=====================================================================================================================
// Trustwave ltd. @{SRCH}
//														functional_tests_runner_test.cpp
//
//---------------------------------------------------------------------------------------------------------------------
// DESCRIPTION: 
//
//
//---------------------------------------------------------------------------------------------------------------------
// By      : Assaf Cohen
// Date    : 9/19/19
// Comments:
//=====================================================================================================================
//                          						Include files
//=====================================================================================================================
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include "../components/boost_test_wrapper/wrapper.hpp"
BOOST_AUTO_TEST_SUITE(Functional)

    BOOST_AUTO_TEST_CASE(standard) {
        boost::asio::io_service ios;
        trustwave::wrapper w(ios);
        std::future<std::error_code> e;
        auto pa = std::make_pair(std::future<std::string>(),std::future<std::string>());
        auto p=w.start_external_test("exe_test","./functional_tests/actions.xml",std::move(pa),std::move(e));
        ios.run();
        auto out = pa.first.get();
        auto err = pa.second.get();

        std::cout<<out;

    }
BOOST_AUTO_TEST_SUITE_END()