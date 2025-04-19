#include "httprequests.hpp"

void HTTPResponse::setStatusCode(Status_Codes status)
{
    response.clear();
    response += "HTTP/1.1 ";
    switch(status)
    {
        case Status_Codes::OK_200:
            response += "200 OK\r\n";
            break;
        case Status_Codes::NOT_FOUND_404:
            response += "404 Not Found\r\n";
            break;
        case Status_Codes::HTTP_VERSION_NOT_SUPPORTED_505:
            response += "505 Http Version Not Supported";
            break;
        default:
            response += "501 Not Implemented\r\n";
    }
}


void HTTPResponse::setConnection(Connection stat)
{
    response += "Connection: ";
    switch(stat)
    {
        case Connection::KEEP_ALIVE:
            response += "keep-alive\r\n";
            break;
        default:
            response += "close\r\n";
    }
}

void HTTPResponse::setContentType(MIME_TYPE type)
{
    response += "Content-Type: ";
    switch(type)
    {
        case MIME_TYPE::APPLICATION:
            response += "application/html\r\n";
            break;
        case MIME_TYPE::PLAIN_TEXT:
            response += "text/plain\r\n";
            break;
        default:
            response += "text/html; charset=ASCII\r\n";
    }
}

void HTTPResponse::setContentLength(size_t length)
{
    response += "Content-Length: " + std::to_string(length) + "\r\n";
}

std::string HTTPResponse::getResponse()
{
    response += "\r\n";
    return response;
}