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
#include <boost/container/flat_map.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/range/iterator_range_io.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/algorithm/string.hpp>
#include "../components/boost_test_wrapper/wrapper.hpp"
#include <string>
BOOST_AUTO_TEST_SUITE(Functional)

    BOOST_AUTO_TEST_CASE(standard) {

         boost::asio::io_service ios;
        trustwave::wrapper w(ios);
        std::future<std::error_code> e;
        auto pa = std::make_pair(std::future<std::string>(),std::future<std::string>());
        auto p = w.start_external_test("exe_test","./functional_tests/actions.xml",std::move(pa),std::move(e));
        ios.run();
        auto out = pa.first.get();
        auto err = pa.second.get();
        std::cout<<out;
        std::vector<std::string > cont;
        boost::split(cont, out, boost::is_any_of("\n"));

        for (auto line:cont)
        {
            if(boost::starts_with(line, "result")) {
                std::map<std::string,std::string> key_val;
                std::vector<std::string > cols;
                boost::split(cols, line, boost::is_any_of(" "));
                for(auto col:cols)
                {
                    std::cout<<col<<std::endl;
                    auto sep_it = std::find(std::begin(col),std::end(col),':');
                    if(sep_it!=std::end(col))
                    {
                        key_val.emplace(std::string(std::begin(col),sep_it), std::string (sep_it + 1, std::end(col)));
                    }
                    else throw std::runtime_error("cannot parse find Rotem!");
                }
                BOOST_TEST(key_val["result"] == "passed");
            }
        }
}
BOOST_AUTO_TEST_SUITE_END()