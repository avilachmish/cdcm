//=====================================================================================================================
// Trustwave ltd. @{SRCH}
//														filter.hpp
//
//---------------------------------------------------------------------------------------------------------------------
// DESCRIPTION:
//
//
//---------------------------------------------------------------------------------------------------------------------
// By      : Assaf Cohen
// Date    : 1/19/20
// Comments:
//=====================================================================================================================
//                          						Include files
//=====================================================================================================================
#ifndef SRC_BACKEND_SERVICES_WINRM_CLIENT_FILTER_HPP
#define SRC_BACKEND_SERVICES_WINRM_CLIENT_FILTER_HPP

#include "wsman-filter.h"
#include <map>
#include <string>
#include <vector>

namespace trustwave {

    typedef std::map<std::string, std::string> NameValuePairs;
    typedef std::map<std::string, std::string>::const_iterator PairsIterator;

    class winrm_epr;

    enum WsmanAssocType { WSMAN_ASSOCIATED = 0, WSMAN_ASSOCIATOR };

    class winrm_filter {
    private:
        filter_t* filter;

        filter_t* makeFilterSelector();

    public:
        winrm_filter(const winrm_filter& filter);

        winrm_filter& operator=(const winrm_filter& other);

        winrm_filter(const std::string& dialect, const std::string& query);

        explicit winrm_filter(const NameValuePairs* s = nullptr);

        winrm_filter(const winrm_epr& epr, enum WsmanAssocType assocType, const std::string& assocClass = std::string(),
                    const std::string& resultClass = std::string(), const std::string& role = std::string(),
                    const std::string& resultRole = std::string(),
                    const std::vector<std::string>& resultProp = std::vector<std::string>());

        virtual ~winrm_filter();

        int addSelector(const char* key, const char* value);

        int addSelector(const std::string& name, const std::string& value);

        void addSelectors(const NameValuePairs& selectors);

        void addSelectors(const NameValuePairs* s);

        filter_t* getFilter() const;

        operator filter_t*() const;
    };
} // namespace trustwave
#endif // SRC_BACKEND_SERVICES_WINRM_CLIENT_FILTER_HPP