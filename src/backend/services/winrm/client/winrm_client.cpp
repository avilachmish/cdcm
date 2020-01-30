//----------------------------------------------------------------------------
//
//  Copyright (C) Intel Corporation, 2007.
//            (C) Red Hat, Inc, 2015.
//
//  File:       winrm_client.cpp
//
//  Contents:   An implementation of the IWsmanClient interface using openwsman
//
//----------------------------------------------------------------------------

#include "winrm_client.hpp"

#include <sstream>

extern "C" {
#include "u/libu.h"
#include "wsman-api.h"
#include "wsman-client-transport.h"
}

constexpr std::string_view WSMAN_ENCODING{"UTF-8"};
enum : long {
    OK = 200,
    ERROR=400,
    SERVER_ERROR=500

};
using trustwave::winrm_client;

[[nodiscard]] static bool CheckWsmanResponse(WsManClient* cl, WsXmlDocH& doc);

static bool ResourceNotFound(WsManClient* cl, WsXmlDocH& enumerationRes);

static std::string XmlDocToString(WsXmlDocH& doc);

static client_opt_t* SetOptions(WsManClient* cl);

static std::string GetSubscribeContext(WsXmlDocH& doc);

static std::string ExtractPayload(WsXmlDocH& doc);

static std::string ExtractItems(WsXmlDocH& doc);

winrm_client::winrm_client(const std::string& host, const int port, const std::string& path, const std::string& scheme,
                           const std::string& auth_method, const std::string& username, const std::string& password):cl(nullptr)
{
    cl = wsmc_create(host.c_str(), port, path.c_str(), scheme.c_str(), username.c_str(), password.c_str());
    if(nullptr == cl) {
        std::string error;
        error.append("wsmc_create failed: ");
        error.append("host: " + host);
        error.append("username: " + username);
        throw winrm_client_exception(error.c_str(), WSMAN_GENERAL_ERROR);
    }
    SetAuth(auth_method);

    wsmc_transport_init(cl, (void*)nullptr);
}

// Destructor.
winrm_client::~winrm_client()
{
    wsmc_transport_fini(cl);
    wsmc_release(cl);
}

std::pair<bool,std::string> winrm_client::Identify() const
{
    winrm_options options;
    options.setNamespace(GetNamespace());

    WsXmlDocH identifyResponse = wsmc_action_identify(cl, options);
    bool res_ok = CheckWsmanResponse(cl, identifyResponse);
    std::string xml = ExtractPayload(identifyResponse);
    ws_xml_destroy_doc(identifyResponse);
    return std::make_pair(res_ok,xml);
}

void winrm_client::Enumerate(const std::string& resourceUri, std::vector<std::string>& enumRes,
                             const winrm_options& options, const winrm_filter& filter) const
{
    WsXmlDocH doc;
    char* enumContext;
    WsXmlDocH enum_response = wsmc_action_enumerate(cl, (char*)resourceUri.c_str(), options, filter);

    if(ResourceNotFound(cl, enum_response)) trustwave::winrm_resource_not_found_exception(resourceUri.c_str());

    enumContext = wsmc_get_enum_context(enum_response);
    ws_xml_destroy_doc(enum_response);

    while(enumContext != nullptr && enumContext[0] != 0) {
        doc = wsmc_action_pull(cl, resourceUri.c_str(), options, nullptr, enumContext);
        CheckWsmanResponse(cl, doc);
        std::string payload = ExtractItems(doc);

        if(payload.length() > 0) enumRes.push_back(payload);

        wsmc_free_enum_context(enumContext);
        enumContext = wsmc_get_enum_context(doc);
        ws_xml_destroy_doc(doc);
    }

    wsmc_free_enum_context(enumContext);
}

void winrm_client::Enumerate(const std::string& resourceUri, std::vector<std::string>& enumRes,
                             const NameValuePairs* s) const
{
    winrm_options options;
    options.setNamespace(GetNamespace());
    options.addSelectors(s);

    Enumerate(resourceUri, enumRes, options, winrm_filter());
}

void winrm_client::Enumerate(const std::string& resourceUri, winrm_filter& filter,
                             std::vector<std::string>& enumRes) const
{
    winrm_options options;
    options.setNamespace(GetNamespace());

    Enumerate(resourceUri, enumRes, options, filter);
}

std::string winrm_client::Get(const std::string& resourceUri, const winrm_options& options) const
{
    WsXmlDocH doc = wsmc_action_get(cl, (char*)resourceUri.c_str(), options);
    CheckWsmanResponse(cl, doc);
    std::string xml = ExtractPayload(doc);
    ws_xml_destroy_doc(doc);
    return xml;
}

std::string winrm_client::Get(const std::string& resourceUri, const NameValuePairs* s) const
{
    winrm_options options;
    options.setNamespace(GetNamespace());
    options.addSelectors(s);

    return Get(resourceUri, options);
}

std::string GetSubscribeContext(WsXmlDocH& doc)
{
    std::string str;
    char* buf = nullptr;
    WsXmlNodeH bodyNode = ws_xml_get_soap_body(doc);
    WsXmlNodeH tmp = nullptr;
    if(bodyNode == nullptr) return str;
    bodyNode = ws_xml_get_child(bodyNode, 0, XML_NS_EVENTING, WSEVENT_SUBSCRIBE_RESP);
    if(bodyNode == nullptr) return str;
    bodyNode = ws_xml_get_child(bodyNode, 0, XML_NS_EVENTING, WSEVENT_SUBSCRIPTION_MANAGER);
    if(bodyNode == nullptr) return str;
    tmp = ws_xml_get_child(bodyNode, 0, XML_NS_ADDRESSING, WSA_REFERENCE_PARAMETERS);
    if(tmp == nullptr) {
        tmp = ws_xml_get_child(bodyNode, 0, XML_NS_ADDRESSING, WSA_REFERENCE_PROPERTIES);
        if(tmp == nullptr) return str;
    }
    wsmc_node_to_buf(tmp, &buf);
    str = std::string(buf);
    u_free(buf);
    return str;
}

std::string ExtractPayload(WsXmlDocH& doc)
{
    WsXmlNodeH bodyNode = ws_xml_get_soap_body(doc);
    WsXmlNodeH payloadNode = ws_xml_get_child(bodyNode, 0, nullptr, nullptr);
    char* buf = nullptr;
    wsmc_node_to_buf(payloadNode, &buf);
    std::string payload = std::string(buf);
    u_free(buf);
    return payload;
}

std::string ExtractItems(WsXmlDocH& doc)
{
    std::string payload;
    WsXmlNodeH bodyNode = ws_xml_get_soap_body(doc);
    WsXmlNodeH pullResponse = ws_xml_get_child(bodyNode, 0, XML_NS_ENUMERATION, WSENUM_PULL_RESP);
    WsXmlNodeH itemsNode = ws_xml_get_child(pullResponse, 0, XML_NS_ENUMERATION, WSENUM_ITEMS);
    WsXmlNodeH n = ws_xml_get_child(itemsNode, 0, nullptr, nullptr);
    if(n) {
        char* buf = nullptr;
        wsmc_node_to_buf(n, &buf);
        payload = std::string(buf);
        u_free(buf);
    }
    return payload;
}

std::string XmlDocToString(WsXmlDocH& doc)
{
    char* buf;
    int len;
    ws_xml_dump_memory_enc(doc, &buf, &len, WSMAN_ENCODING.data());
    std::string str = (buf) ? std::string(buf) : ""; // This constructor copies the data.
    if(buf) {
        u_free(buf);
    }
    return str;
}

bool CheckWsmanResponse(WsManClient* cl, WsXmlDocH& doc)
{
    long lastError = wsmc_get_last_error(cl);

    if(lastError) {
        std::stringstream ss1;
        ss1 << "Failed to establish a connection with the server." << std::endl
            << "Openwsman last error = " << lastError;
        ws_xml_destroy_doc(doc);
        throw trustwave::winrm_client_exception(ss1.str().c_str(), WSMAN_CONNECT_ERROR);
    }

    long responseCode = wsmc_get_response_code(cl);

    if(responseCode != OK && responseCode != ERROR && responseCode != SERVER_ERROR) {
        std::stringstream ss2;
        ss2 << "An HTTP error occurred." << std::endl << "HTTP Error = " << responseCode;
        ws_xml_destroy_doc(doc);
        throw trustwave::winrm_client_exception(ss2.str().c_str(), WSMAN_HTTP_ERROR);
    }

    if(!doc) throw trustwave::winrm_client_exception("The Wsman response was nullptr.");

    if(wsmc_check_for_fault(doc)) {
        WsManFault fault = {0};
        wsmc_get_fault_data(doc, &fault);
        std::string subcode_s = fault.subcode ? std::string(fault.subcode) : "";
        std::string code_s = fault.code ? std::string(fault.code) : "";
        std::string reason_s = fault.reason ? std::string(fault.reason) : "";
        std::string detail_s = fault.fault_detail ? std::string(fault.fault_detail) : "";
        ws_xml_destroy_doc(doc);

        std::stringstream ss3;
        ss3 << "A Soap Fault was received:" << std::endl;
        ss3 << "FaultCode: " << code_s << std::endl;
        ss3 << "FaultSubCode: " + subcode_s << std::endl;
        ss3 << "FaultReason: " + reason_s << std::endl;
        ss3 << "FaultDetail: " + detail_s << std::endl;
        ss3 << "HttpCode:  = " << responseCode;
        throw trustwave::winrm_soap_exception(ss3.str().c_str(), code_s, subcode_s, reason_s, detail_s);
    }

    return true;
}

bool ResourceNotFound(WsManClient* cl, WsXmlDocH& enumerationRes)
{
    long responseCode = wsmc_get_response_code(cl);
    if(wsmc_get_last_error(cl) || (responseCode != 200 && responseCode != 400 && responseCode != 500)
       || !enumerationRes) {
        CheckWsmanResponse(cl, enumerationRes);
    }

    if(!wsmc_check_for_fault(enumerationRes)) return false;

    WsManFault fault = {0};
    bool ret = false;
    wsmc_get_fault_data(enumerationRes, &fault);
    std::string subcode_s = fault.subcode ? std::string(fault.subcode) : "";
    if(subcode_s.find("DestinationUnreachable") != std::string::npos) return true;

    if(!ret) CheckWsmanResponse(cl, enumerationRes);

    return ret;
}

void winrm_client::SetAuth(const std::string& auth_method)
{
    if(auth_method.empty()) return;

    wsman_transport_set_auth_method(cl, auth_method.c_str());
    if(wsmc_transport_get_auth_value(cl) == WS_MAX_AUTH) {
        // Authentication method not supported, reverting to digest
        wsman_transport_set_auth_method(cl, "digest");
    }
}

void winrm_client::SetTimeout(unsigned long mtime) { wsman_transport_set_timeout(cl, mtime); }

void winrm_client::SetUserName(const std::string& user_name)
{
    if(user_name.empty()) return;

    wsman_transport_set_userName(cl, user_name.c_str());
}

void winrm_client::SetPassword(const std::string& password)
{
    if(password.empty()) return;

    wsman_transport_set_password(cl, password.c_str());
}

void winrm_client::SetEncoding(const std::string& encoding)
{
    if(encoding.empty()) return;

    wsmc_set_encoding(cl, encoding.c_str());
}

void winrm_client::SetNamespace(const std::string& ns)
{
    if(ns.empty()) return;

    wsmc_set_namespace(cl, ns.c_str());
}

// Set server certificate params
// params: cainfo - std::string naming a file holding one or more certificates
// to verify the peer with.
//         capath - std::string naming a dierctory holding multiple CA
//         certificates to verify the peer with.
// Give empty strings if you want curl to search for certificates inthe default
// path
void winrm_client::SetServerCert(const std::string& cainfo, const std::string& capath)
{
    // This means curl verifies the server certificate
    wsman_transport_set_verify_peer(cl, 1);

    // This means the certificate must indicate that the server is the server to
    // which you meant to connect.
    wsman_transport_set_verify_host(cl, 2);

    if(!cainfo.empty()) wsman_transport_set_cainfo(cl, cainfo.c_str());

    if(!capath.empty()) wsman_transport_set_capath(cl, capath.c_str());
}

// Set client certificates params
// params: cert - file name of your certificate.
//         key  - file name of your private key.
void winrm_client::SetClientCert(const std::string& cert, const std::string& key)
{
    if(!cert.empty()) wsman_transport_set_cert(cl, cert.c_str());

    if(!key.empty()) wsman_transport_set_key(cl, key.c_str());
}

std::string winrm_client::GetNamespace() const
{
    char* ns = wsmc_get_namespace(cl);
    return ns ? std::string(ns) : std::string();
}
