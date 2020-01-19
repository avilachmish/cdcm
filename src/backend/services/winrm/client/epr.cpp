//=====================================================================================================================
// Trustwave ltd. @{SRCH}
//														epr.cpp
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

#include "epr.hpp"

using trustwave::winrm_epr;

winrm_epr::winrm_epr(const std::string& eprstring): epr(epr_from_string(eprstring.c_str())) {}

winrm_epr::winrm_epr(const std::string& uri, const std::string& address): epr(epr_create(uri.c_str(), nullptr, address.c_str())) {}

winrm_epr::winrm_epr(winrm_epr& epr): epr(epr_copy(epr.getepr())) {}

winrm_epr::winrm_epr(winrm_epr&& epr) noexcept : winrm_epr()
{
    swap(*this,epr);
}

winrm_epr& winrm_epr::operator=(winrm_epr other) noexcept
{
    swap(*this,other);
    return *this;
}

winrm_epr::~winrm_epr()
{
    if(epr) {
        epr_destroy(epr);
        epr = nullptr;
    }
}

int winrm_epr::addTextSelector(const std::string& name, const std::string& value)
{
    return epr_add_selector_text(epr, name.c_str(), value.c_str());
}

int winrm_epr::addEprSelector(const std::string& name, winrm_epr& epr)
{
    return epr_add_selector_epr(epr, name.c_str(), epr.getepr());
}

int winrm_epr::deleteSelector(const std::string& name) { return epr_delete_selector(epr, name.c_str()); }

bool winrm_epr::operator==(const winrm_epr& rhs) const { return epr_cmp(epr, rhs.epr) == 0; }

bool winrm_epr::operator!=(const winrm_epr& rhs) const { return !(*this == rhs); }

epr_t* winrm_epr::getepr() const { return epr; }

winrm_epr::operator epr_t*() const { return getepr(); }
