//
// Created by rfrenkel on 11/5/2019.
//

#include "wql_action.hpp"

#include <tuple>
#include <string>
#include <boost/tokenizer.hpp>

#include "../../clients/wmi/wmi_client.hpp"
#include "../../common/session.hpp"
#include "../../common/protocol/msg_types.hpp"
#include "../../common/protocol/protocol.hpp"
#include "../../common/singleton_runner/authenticated_scan_server.hpp"


using namespace trustwave;

std::string wql_resp_to_json(std::string work_str) {

    boost::char_separator<char> end_of_line_delim("\n");
    boost::char_separator<char> keys_delim("|");

    //tokenized_response will hold the parsed response as a table
    // at the first row there will be only the keys names
    // the rest of the rows will hold only the values of the key, matching by the index:
    // | x | y |
    // | 1 | 2 |
    // means x=1, y=2
    std::vector<std::vector<std::string>> tokenized_response;

    boost::tokenizer<boost::char_separator<char>> rows_tokens(work_str, end_of_line_delim);
    int row_number = 0;
    for (auto rows_iter = rows_tokens.begin(); rows_iter != rows_tokens.end(); ++rows_iter, ++row_number)
    {
        tokenized_response.push_back( std::vector<std::string>() );
        boost::tokenizer<boost::char_separator<char>> keys_tokens(*rows_iter, keys_delim);
        for (auto keys_iter = keys_tokens.begin(); keys_iter != keys_tokens.end(); ++keys_iter)
        {
            tokenized_response[row_number].push_back(*keys_iter);
        }
    }

    //rotem to delete start
    std::cout << "vec.rows: " << tokenized_response.size() << std::endl;
    for (auto row : tokenized_response)
    {
        std::cout << "num of elements: " << row.size() << std::endl;
    }

    for (auto row: tokenized_response)
    {
        for (auto column : row)
        {
            std::cout << column << " || ";
        }
        std::cout << std::endl;
    }
    //rotem to delete end

    tao::json::events::to_value consumer;
    consumer.begin_array();
    for (std::vector<std::vector<std::basic_string<char>>>::size_type i = 1; i< tokenized_response.size(); ++i)
    {
        std::cout << "i: " << i << std::endl; //rotem to delete
        consumer.begin_object();
        for (std::vector<std::basic_string<char> >::size_type j=0; j< tokenized_response[i].size(); ++j )
        {

            std::cout << j << " inserting pair: " <<  tokenized_response[0][j] << " : "  << tokenized_response[i][j] << std::endl; //rotem to delete
            consumer.key( tokenized_response[0][j] );
            consumer.string( tokenized_response[i][j]);
            consumer.member();
        }
        consumer.end_object();
        consumer.element();
    }

    consumer.end_array();
    const tao::json::value json_value = std::move( consumer.value );
    std::string json_value_as_str = to_string(json_value,1);
    std::cout << json_value_as_str  << std::endl; //rotem to delete
    return json_value_as_str;
}


/********************************************************
 *
 *  return value: -1 for error, 0 for success
 *********************************************************/
int WQL_Query_Action::act(boost::shared_ptr<session> sess, std::shared_ptr<action_msg> action, std::shared_ptr<result_msg> res)
{
    if (!sess || (sess && sess->id().is_nil())){
        AU_LOG_ERROR("Session not found");
        res->res("Error: Session not found");
        return -1;
    }

    auto wmi_wql_action = std::dynamic_pointer_cast<wmi_action_wql_query_msg>(action);
    if (!wmi_wql_action){
        AU_LOG_ERROR("Failed dynamic cast");
        res->res("Error: Internal error");
        return -1;
    }

    auto client = std::dynamic_pointer_cast <trustwave::wmi_client>(sess->get_client <trustwave::wmi_client>(2)); //rotem: should use enum
    if (!client){
        AU_LOG_ERROR("Failed dynamic cast");
        res->res("Error: Failed dynamic cast");
        return -1;
    }

   // using result = std::tuple<bool,WERROR>;

    auto connect_result = client->connect(*sess, wmi_wql_action->wmi_namespace);
    if (false == std::get<0>(connect_result) )
    {
        AU_LOG_ERROR("failed to connect to the asset");
        res->res(std::string("Error: ")+std::string("Failed to connect"));
        return -1;
    }

    auto query_result = client->query_remote_asset(wmi_wql_action->wql);
    if (false == std::get<0>(query_result) )
    {
        AU_LOG_ERROR("failed to get wql response. Error: %s", std::get<1>(query_result).c_str());
        res->res(std::string("Error: ")+std::string(std::get<1>(query_result)));
        return -1;
    }


    std::string wql_raw_response = std::get<1>(query_result);
    std::cout << "temp response not parsed: \n" << wql_raw_response << std::endl;
    std::string wql_resp_json = wql_resp_to_json(wql_raw_response);

    res->res(wql_resp_json);
    return 0;
}


Dispatcher<Action_Base>::Registrator WQL_Query_Action::m_registrator(new WQL_Query_Action,
                                                                  authenticated_scan_server::instance().public_dispatcher);