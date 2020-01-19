//=====================================================================================================================
// Trustwave ltd. @{SRCH}
//														options.hpp
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
#ifndef SRC_BACKEND_SERVICES_WINRM_CLIENT_OPTIONS_HPP
#define SRC_BACKEND_SERVICES_WINRM_CLIENT_OPTIONS_HPP

#include <string>

extern "C" {
#include "wsman-api.h"
}

#include "filter.hpp" // For NameValuePairs

namespace trustwave {
    class winrm_epr;

    class winrm_options {
    private:
        client_opt_t* options;

        winrm_options(const winrm_options& copy);

        winrm_options& operator=(const winrm_options& rhs);

    public:
        winrm_options();

        explicit winrm_options(unsigned long flags);

        ~winrm_options();

        void setNamespace(const char* namespace_);

        void setNamespace(const std::string& namespace_);

        void setDeliveryMode(WsmanDeliveryMode delivery_mode);

        void setDeliveryURI(const char* delivery_uri);

        void setDeliveryURI(const std::string& delivery_uri);

        void setReference(const char* reference);

        void setReference(const std::string& reference);

        void setExpires(const float expires);

        void setHeartbeatInterval(const float heartbeat_interval);

        void addProperty(const char* key, const char* value);

        void addProperty(const std::string& key, const std::string& value);

        void addProperty(const std::string& key, const winrm_epr& epr);

        void addSelector(const char* key, const char* value);

        void addSelector(const std::string& key, const std::string& value);

        void addSelectors(const NameValuePairs& selectors);

        void addSelectors(const NameValuePairs* selectors);

        void addFlag(unsigned long flag);

        void removeFlag(unsigned long flag);

        unsigned long getFlags() const;

        client_opt_t* getOptions() const;

        operator client_opt_t*() const;
    };
} // namespace trustwave
#endif // SRC_BACKEND_SERVICES_WINRM_CLIENT_OPTIONS_HPP