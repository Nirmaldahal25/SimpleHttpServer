#include "httprequests.hpp"

inline bool isFloat(const std::string& s)
{
    std::istringstream iss(s);
    float dummy;
    iss >> std::noskipws >> dummy;
    return iss && iss.eof();
} 

inline bool isNumber(const std::string& s)noexcept(false)
{
    for(const char& a : s)
    {
        if(std::isdigit(a) == 0) 
            return false;
    }
    return true;
}

static Status_Line matchStatusLine(const std::string& status)
{
    if(status == "GET")
    {
        return Status_Line::GET;
    }
    else if(status == "POST")
    {
        return Status_Line::POST;
    }
    else if(status == "HEAD")
    {
        return Status_Line::HEAD;
    }
    else
    {
        return Status_Line::UNIDENTIFIED;
    }
}

namespace RequestParser
{
    RequestType requestType(std::string& str)
    {
        float version = 0.0;
        Status_Line status_line;
        std::string path;
        std::string ver;

        size_t i = 0;
        std::string temp="";
        size_t ind = 0;
        size_t token_no = 0;
        

        for(int i=0; i<str.size() && (str[i] != '\r' || str[i]!='\n'); i++) 
        {
            if(str[i] == '=' ) continue;
            if(str[i] == ' ' || ((i+1)<str.size() && str[i+1] == '\r'))
            {
                if(temp.size() != 0)
                {
                    switch(token_no)
                    {
                        case 0:
                            status_line = matchStatusLine(temp);
                            break;
                        case 1:
                            path = std::move(temp);
                            break;
                        case 2:
                            temp += str[i];
                            ind = temp.find('/');
                            if(ind != std::string::npos)
                            {
                                ver = temp.substr(ind+1, temp.size());
                                version = isFloat(ver) ? std::stof(ver) : 0.f;
                            }
                        default:
                            temp.clear();
                    }
                    token_no++;
                    temp.clear();
                }
                continue;
            }
            else temp += str[i];
        }
        return {status_line, version,path};
    }


    Connection getConnection(const std::string& str)
    {
        int index = str.find("Connection");
        index += 12;

        std::string temp;
        if(index != std::string::npos)
        {
            while(str[index] != '\r')
            {
                if(str[index] == ' ')
                {
                    index++;
                    continue;
                }
                else
                {
                    temp += str[index];
                }
                index++;
            } 
            if(temp == "keep-alive")
            {
                return Connection::KEEP_ALIVE;
            }else
            {
                return Connection::CLOSE;
            }
        }
        return Connection::CLOSE;
    }

    size_t getContentLength(const std::string& str)
    {
        int index = str.find("Content-Length");
        index += 16;

        size_t length = 0;

        while(str[index] != '\r')
        {
            if(str[index] == ' ')
            {
                index++;
                continue;
            }
            else if(std::isdigit(str[index]))
            {
                if(length == 999999999) break;
                length = length * 10;
                length += (str[index] - '0');
            }
            index++;
        }
        return length;
    }

    std::vector<MIME_TYPE> getContentType(const std::string& str)
    {
        int index = str.find("Content-Type");
        index += 14;

        index = str.find_first_of("text/html", index);

        return {MIME_TYPE::HTML_TEXT};
    }
};


