//=====================================================================================================================
// Trustwave ltd. @{SRCH}
//														session.hpp
//
//---------------------------------------------------------------------------------------------------------------------
// DESCRIPTION: @{HDES}
// -----------
//---------------------------------------------------------------------------------------------------------------------
// CHANGES LOG: @{HREV}
// -----------
// Revision: 01.00
// By      : Assaf Cohen
// Date    : 19 Apr 2019
// Comments:

#ifndef TRUSTWAVE_COMMON_SESSION_HPP_
#define TRUSTWAVE_COMMON_SESSION_HPP_
#include "credentials.hpp" // for credentials
#include <array>
#include <boost/uuid/string_generator.hpp> // for uuid
#include <boost/uuid/uuid.hpp> // for uuid
#include <memory>
#include <string> // for string

namespace trustwave {

enum cdcm_client_type {
    REGISTRY_CLIENT=0,
//  SMB_CLIENT,//Irreleavant
    WMI_CLIENT,
    CLIENTS_COUNT
};

    class cdcm_client;
    class session final {
    public:
        session();
        session(std::string remote, const credentials& creds);
        const boost::uuids::uuid& id() const;
        std::string idstr() const;

        const std::string& remote() const;
        credentials creds() const;

        template<typename T> std::shared_ptr<cdcm_client> get_client(cdcm_client_type c)
        {
            if(!clients_[c]) {
                clients_[c].reset(new T);
            }
            return clients_[c];
        }

        explicit operator bool() const { return uuid_.is_nil(); }

        bool id(const std::string& ids)
        {
            try {
                uuid_ = boost::uuids::string_generator()(ids);
            }
            catch(std::runtime_error& ex) {
                return false;
            }
            return true;
        }

        void remote(const std::string& dest) { remote_ = dest; }

        void creds(const std::string& domain, const std::string& username, const std::string& password,
                   const std::string& workstation)
        {
            creds_.creds(domain, username, password, workstation);
        }

    private:
        boost::uuids::uuid uuid_;
        std::string remote_;
        credentials creds_;
        std::array<std::shared_ptr<cdcm_client>, CLIENTS_COUNT> clients_;
    };
} // namespace trustwave

#endif /* TRUSTWAVE_COMMON_SESSION_HPP_ */
