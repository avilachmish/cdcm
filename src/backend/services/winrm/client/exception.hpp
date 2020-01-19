//=====================================================================================================================
// Trustwave ltd. @{SRCH}
//														exception.hpp
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
#ifndef SRC_BACKEND_SERVICES_WINRM_CLIENT_EXCEPTION_HPP
#define SRC_BACKEND_SERVICES_WINRM_CLIENT_EXCEPTION_HPP

#include <stdexcept>
#include <string>

// Error Codes
constexpr auto WSMAN_SUCCESS =0;
// A general error occurred
constexpr auto WSMAN_GENERAL_ERROR= 1;
// A transport level error occurred
constexpr auto WSMAN_TRANSPORT_ERROR= 2;
// An HTTP error occurred
constexpr auto WSMAN_HTTP_ERROR= 3;
// The WS-MAN server returned a soap fault
constexpr auto WSMAN_SOAP_FAULT =4;
// A string to type or type to string error occurred
constexpr auto WSMAN_TYPE_CONVERSION_ERROR =5;
// Failed to parse or write an XML string
constexpr auto  WSMAN_XML_ERROR =6;
// Input information is missing
constexpr auto  WSMAN_MISSING_INPUT =7;
// An unexpected response was received
constexpr auto  WSMAN_RESPONSE_UNKNOWN =8;
// Error resulting from MustUnderstand attribute
constexpr auto  WSMAN_MUST_UNDERSTAND= 9;
// The soap message is invalid or of invalid version
constexpr auto  WSMAN_SOAP_MESSAGE_INVALID= 10;
// The Soap header is too long
constexpr auto  WSMAN_SOAP_HDR_OVERFLOW= 11;
// a UDP error occurred
constexpr auto  WSMAN_UDP_ERROR= 12;
// a TCP error occurred
constexpr auto  WSMAN_TCP_ERROR =13;
// failed to connect to server
constexpr auto  WSMAN_CONNECT_ERROR= 14;
// resource not found
constexpr auto  WSMAN_RESOURCE_NOT_FOUND= 15;

namespace trustwave {
    class winrm_exception: public std::exception {
    private:
        std::string _what;
        unsigned int error;

    public:
        winrm_exception(const char* what, unsigned int err = WSMAN_GENERAL_ERROR): _what(what), error(err) {}

        virtual ~winrm_exception() noexcept = default;

        virtual std::string getDetail() { return _what; }

        virtual const char* what() const noexcept { return _what.c_str(); };

        virtual unsigned int getErr() const noexcept { return error; }

        std::string getStrErr()
        {
            std::string res = "Unknown error";
            switch(error) {
                case WSMAN_SUCCESS:
                    res = "Operation succeeded";
                    break;
                case WSMAN_GENERAL_ERROR:
                    res = "General error occurred";
                    break;
                case WSMAN_TRANSPORT_ERROR:
                    res = "Transport error occurred";
                    break;
                case WSMAN_HTTP_ERROR:
                    res = "HTTP error occurred";
                    break;
                case WSMAN_SOAP_FAULT:
                    res = "SOAP error occurred";
                    break;
                case WSMAN_TYPE_CONVERSION_ERROR:
                    res = "Conversion type error occurred";
                    break;
                case WSMAN_XML_ERROR:
                    res = "XML error occurred";
                    break;
                case WSMAN_MISSING_INPUT:
                    res = "Missing input error occurred";
                    break;
                case WSMAN_RESPONSE_UNKNOWN:
                    res = "Response unknown";
                    break;
                case WSMAN_MUST_UNDERSTAND:
                    res = "Must understand error";
                    break;
                case WSMAN_SOAP_MESSAGE_INVALID:
                    res = "SOAP message invalid";
                    break;
                case WSMAN_SOAP_HDR_OVERFLOW:
                    res = "Overflow soap header";
                    break;
                case WSMAN_UDP_ERROR:
                    res = "UDP error occurred";
                    break;
                case WSMAN_TCP_ERROR:
                    res = "TCP error occurred";
                    break;
                case WSMAN_CONNECT_ERROR:
                    res = "Connection error occurred";
                    break;
            }
            return res;
        }
    };

    // WS-MAN Client exceptions
    // Exception thrown when server error occurs
    class winrm_client_exception: public winrm_exception {
    public:
        winrm_client_exception(const char* message, unsigned int err = WSMAN_GENERAL_ERROR): winrm_exception(message, err)
        {
        }

        virtual ~winrm_client_exception() noexcept {}
    };

    // Exception thrown if the server returned a soap fault
    class winrm_soap_exception: public winrm_client_exception {
    private:
        std::string soapCodeValue;
        std::string soapSubcodeValue;
        std::string soapReason;
        std::string soapDetail;

    public:
        winrm_soap_exception(const char* message, const std::string& faultCode = std::string(),
                       const std::string& faultSubcode = std::string(), const std::string& faultReason = std::string(),
                       const std::string& faultDetail = std::string()):
            winrm_client_exception(message, WSMAN_SOAP_FAULT)
        {
            SetWsmanFaultFields(faultCode, faultSubcode, faultReason, faultDetail);
        }

        virtual ~winrm_soap_exception() noexcept {}

        void SetWsmanFaultFields(const std::string& faultCode, const std::string& faultSubcode,
                                 const std::string& faultReason, const std::string& faultDetail) noexcept
        {
            soapCodeValue = faultCode;
            soapSubcodeValue = faultSubcode;
            soapReason = faultReason;
            soapDetail = faultDetail;
        }

        std::string GetFaultCode() const noexcept { return soapCodeValue; }

        std::string GetFaultSubcode() const noexcept { return soapSubcodeValue; }

        std::string GetFaultReason() const noexcept { return soapReason; }

        std::string GetFaultDetail() const noexcept { return soapDetail; }
    };

    // Exception throw if a resource is not found
    class winrm_resource_not_found_exception: public winrm_client_exception {
    public:
        winrm_resource_not_found_exception(const char* message): winrm_client_exception(message, WSMAN_RESOURCE_NOT_FOUND) {}

        virtual ~winrm_resource_not_found_exception() noexcept {}
    };
} // namespace trustwave
#endif // SRC_BACKEND_SERVICES_WINRM_CLIENT_EXCEPTION_HPP