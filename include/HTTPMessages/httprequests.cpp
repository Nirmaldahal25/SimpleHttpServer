#include "httprequests.hpp"

inline bool isFloat(const std::string& s)
{
    std::istringstream iss(s);
    float dummy;
    iss >> std::noskipws >> dummy;
    return iss && iss.eof();
} 

static RequestParser::Status_Line matchStatusLine(const std::string& status)
{
    if(status == "GET")
    {
        return RequestParser::Status_Line::GET;
    }
    else if(status == "POST")
    {
        return RequestParser::Status_Line::POST;
    }
    else if(status == "HEAD")
    {
        return RequestParser::Status_Line::HEAD;
    }
    else
    {
        return RequestParser::Status_Line::UNIDENTIFIED;
    }
}

namespace RequestParser
{
    RequestType requestType(std::string_view& str)
    {
        float version = 0.0;
        Status_Line status_line;
        std::string path;
        std::string ver;

        size_t i = 0;
        std::string temp="";
        size_t ind = 0;
        size_t token_no = 0;

        while(str[i] != '\r')
        { 
            if(str[i] == '=' )
            { 
                i++;
                continue;
            }
            if(str[i] == ' ' )
            {
                if(temp.size() != 0)
                {
                    switch(token_no)
                    {
                        case 0:
                            status_line = matchStatusLine(temp);
                            break;
                        case 1:
                            path = temp.substr(1);
                            break;
                        default:
                            temp.clear();
                    }
                    token_no++;
                    temp.clear();
                }
                i++;
                continue;
            }
            else{
                temp += str[i];
            }
            i++;
        } 
        if(temp.size() > 0)
        {
            ind = temp.find('/');
            if(ind != std::string::npos)
            {
                ver = temp.substr(ind+1, temp.size());
                version = isFloat(ver) ? std::stof(ver) : 0.f;
            }
        }
        if(str.size() > i+1)
        {
            str = str.substr(i+1);
        }
        return {status_line, version,path};
    }
};
