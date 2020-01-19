//=====================================================================================================================
// Trustwave ltd. @{SRCH}
//														epr.hpp
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
#ifndef SRC_BACKEND_SERVICES_WINRM_CLIENT_EPR_HPP
#define SRC_BACKEND_SERVICES_WINRM_CLIENT_EPR_HPP
#include "wsman-epr.h"
#include <string>

namespace trustwave {

    class winrm_epr {
        friend void swap(winrm_epr& a, winrm_epr& b) noexcept {
            using std::swap;
            swap(a.epr, b.epr);

        }
    private:
        epr_t* epr;

    public:
        winrm_epr() = default;
        explicit winrm_epr(const std::string& eprstring);

        winrm_epr(const std::string& uri, const std::string& address);

        winrm_epr(winrm_epr& epr);
        winrm_epr(winrm_epr&& epr) noexcept ;

        winrm_epr& operator=(const winrm_epr other)noexcept ;

        ~winrm_epr();

        int addTextSelector(const std::string& name, const std::string& value);

        int addEprSelector(const std::string& name, winrm_epr& epr);

        int deleteSelector(const std::string& name);

        bool operator==(const winrm_epr& rhs) const;

        bool operator!=(const winrm_epr& rhs) const;

        [[nodiscard]]epr_t* getepr() const;

        operator epr_t*() const;
    };

} // namespace trustwave

#endif // SRC_BACKEND_SERVICES_WINRM_CLIENT_EPR_HPP