//=====================================================================================================================
// Trustwave ltd. @{SRCH}
//														options.cpp
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

#include <cstring>

extern "C" {
#include "wsman-client-api.h"
}
#include "options.hpp"
#include "epr.hpp"

using namespace trustwave;

winrm_options::winrm_options(): options(wsmc_options_init())
{
    if(!options) throw std::bad_alloc();
}

winrm_options::winrm_options(unsigned long flags): options(wsmc_options_init())
{
    if(options)
        options->flags = flags;
    else
        throw std::bad_alloc();
}

winrm_options::~winrm_options()
{
    if(options) {
        wsmc_options_destroy(options);
        options = nullptr;
    }
}

void winrm_options::setNamespace(const char* namespace_)
{
    if(strlen(namespace_) == 0) return;
    wsmc_set_cim_ns(namespace_, options);
}

void winrm_options::setNamespace(const std::string& namespace_) { setNamespace(namespace_.c_str()); }

void winrm_options::setDeliveryMode(WsmanDeliveryMode delivery_mode) { wsmc_set_delivery_mode(delivery_mode, options); }

void winrm_options::setDeliveryURI(const char* delivery_uri) { wsmc_set_delivery_uri(delivery_uri, options); }

void winrm_options::setDeliveryURI(const std::string& delivery_uri) { setDeliveryURI(delivery_uri.c_str()); }

void winrm_options::setReference(const char* reference) { wsmc_set_reference(reference, options); }

void winrm_options::setReference(const std::string& reference) { setReference(reference.c_str()); }

void winrm_options::setExpires(const float expires) { options->expires = expires; }

void winrm_options::setHeartbeatInterval(const float heartbeat_interval)
{
    options->heartbeat_interval = heartbeat_interval;
}

void winrm_options::addProperty(const char* key, const char* value) { wsmc_add_property(options, key, value); }

void winrm_options::addProperty(const std::string& key, const std::string& value) { addProperty(key.c_str(), value.c_str()); }

void winrm_options::addProperty(const std::string& key, const winrm_epr& epr)
{
    wsmc_add_property_epr(options, key.c_str(), epr);
}

void winrm_options::addSelector(const char* key, const char* value) { wsmc_add_selector(options, key, value); }

void winrm_options::addSelector(const std::string& key, const std::string& value)
{
    if(value.length() == 0) return;
    addSelector(key.c_str(), value.c_str());
}

void winrm_options::addSelectors(const NameValuePairs& selectors)
{
    NameValuePairs::const_iterator it;
    for(it = selectors.begin(); it != selectors.end(); ++it) addSelector(it->first, it->second);
}

void winrm_options::addSelectors(const NameValuePairs* selectors)
{
    if(!selectors) return;

    addSelectors(*selectors);
}

void winrm_options::addFlag(unsigned long flag) { options->flags |= flag; }

void winrm_options::removeFlag(unsigned long flag) { options->flags &= ~flag; }

unsigned long winrm_options::getFlags() const { return options->flags; }

client_opt_t* winrm_options::getOptions() const { return options; }

winrm_options::operator client_opt_t*() const { return getOptions(); }
