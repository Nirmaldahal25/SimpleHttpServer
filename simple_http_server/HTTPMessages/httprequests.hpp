#ifndef REQUEST_PARSER
#define REQUEST_PARSER 1

#include <tuple>
#include <sstream>
#include <string>
#include <vector>

enum class Status_Line
{
    GET,
    POST,
    HEAD,
    UNIDENTIFIED
};

enum class MIME_TYPE
{
    PLAIN_TEXT,
    HTML_TEXT,
    APPLICATION,
    MULTIPART_FORM_DATA
};

enum class Connection
{
    KEEP_ALIVE,
    CLOSE
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
namespace RequestParser{
    typedef std::tuple<Status_Line,float, std::string> RequestType;

    // get tuple <statusLine, version, path>
    RequestType requestType(std::string& str);

    //get Connection Type
    Connection getConnection(const std::string& str);

    size_t getContentLength(const std::string& str);

    //get Media Type on the request
    std::vector<MIME_TYPE> getContentType(const std::string& str);
}

class HTTPResponse
{   
public:
    //return Complete HttP response
    std::string getResponse();

    //set status Codes {200 OK, 404 Not Found}
    void setStatusCode(Status_Codes status);

    // set Connection to keep-alive or close
    void setConnection(Connection stat = Connection::CLOSE);

    // content type of data about to send
    void setContentType(MIME_TYPE type = MIME_TYPE::HTML_TEXT);

    // set length of content about to send
    void setContentLength(size_t length);
private: 
    std::string response;
};
#endif