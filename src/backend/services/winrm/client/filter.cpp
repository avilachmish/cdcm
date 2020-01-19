//=====================================================================================================================
// Trustwave ltd. @{SRCH}
//														filter.cpp
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

#include "filter.hpp"
#include "epr.hpp"
#include <cstdlib>

using namespace trustwave;

winrm_filter::winrm_filter(const winrm_filter& filter): filter(filter_copy(filter.getFilter())) {}

winrm_filter::winrm_filter(const std::string& dialect, const std::string& query):
    filter(filter_create_simple(dialect.c_str(), query.c_str()))
{
}

winrm_filter::winrm_filter(const NameValuePairs* s): filter(nullptr) { addSelectors(s); }

winrm_filter& winrm_filter::operator=(const winrm_filter& other)
{
    filter = filter_copy(other.getFilter());
    return *this;
}

winrm_filter::winrm_filter(const winrm_epr& epr, enum WsmanAssocType assocType, const std::string& assocClass,
                         const std::string& resultClass, const std::string& role, const std::string& resultRole,
                         const std::vector<std::string>& resultProp)
{
    int i = 0;
    char** props = nullptr;

    if(!resultProp.empty()) {
        props = new char*[resultProp.size()];
        std::vector<std::string>::const_iterator itr;
        for(itr = resultProp.begin(); itr != resultProp.end(); itr++, i++) {
            props[i] = const_cast<char*>(itr->c_str());
        }
    }

    filter
        = filter_create_assoc(epr, static_cast<int>(assocType), (assocClass.empty() ? nullptr : assocClass.c_str()),
                              (resultClass.empty() ? nullptr : resultClass.c_str()), (role.empty() ? nullptr : role.c_str()),
                              (resultRole.empty() ? nullptr : resultRole.c_str()), props, i);

    delete[] props;
}

winrm_filter::~winrm_filter()
{
    if(filter) {
        filter_destroy(filter);
        filter = nullptr;
    }
}

filter_t* winrm_filter::makeFilterSelector()
{
    if(!filter) filter = filter_create_selector(nullptr);
    return filter;
}

int winrm_filter::addSelector(const char* key, const char* value)
{
    return filter_add_selector(makeFilterSelector(), key, value);
}

int winrm_filter::addSelector(const std::string& name, const std::string& value)
{
    return addSelector(name.c_str(), value.c_str());
}

void winrm_filter::addSelectors(const NameValuePairs& selectors)
{
    NameValuePairs::const_iterator it;
    for(it = selectors.begin(); it != selectors.end(); ++it) addSelector(it->first, it->second);
}

void winrm_filter::addSelectors(const NameValuePairs* selectors)
{
    if(!selectors) return;

    addSelectors(*selectors);
}

filter_t* winrm_filter::getFilter() const { return filter; }

winrm_filter::operator filter_t*() const { return getFilter(); }
