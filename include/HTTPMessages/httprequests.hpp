#ifndef REQUEST_PARSER
#define REQUEST_PARSER 1

#include <string_view>
#include <tuple>
#include <sstream>
#include <string>

namespace RequestParser{
    enum class Status_Line
    {
        GET,
        POST,
        HEAD,
        UNIDENTIFIED
    };
    enum class Status_Codes
    {
        OK_200,
        NOT_FOUND_404,
        LENGTH_REQUIRED,
        INTERNAL_SERVER_ERROR_500,
        NOT_IMPLEMENTED_501,
        SERVICE_UNAVAILABLE_503,
        HTTP_VERSION_NOT_SUPPORTED_505
    };
    typedef std::tuple<Status_Line,float, std::string> RequestType;
    RequestType requestType(std::string_view& str);
}

#endif