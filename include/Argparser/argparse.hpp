#ifndef ARGPARSE
#define ARGPARSE 1

#include <array>
#include <utility>
#include <cctype>
#include <sstream>
#include <iostream>
#include <variant>
#include <optional>

enum DATA_TYPE{
    INTEGER,
    CHAR,
    FLOATING_POINT,
    STRING,
};

template <size_t value = 0>
struct Argparse{
    void setArgument(std::string _variable_name, std::string _help, DATA_TYPE datamode)noexcept
    {
        this->help[indexes[0]++] = std::move(_help);
        this->variable_name[indexes[1]++] = std::move(_variable_name);
        this->d_type[indexes[2]++] = datamode;
    }

    template<typename P>
    auto getArg(int index) const noexcept(false)
    {
        return std::get<std::optional<P>>(getArgument<P>(args[index], d_type[index]));
    }

    void parseArgument(int noofdata, char** argv)
    {
        if(noofdata == 0)return;
        std::string parsearguments = argv[1];
        for(int i =2 ; i <= noofdata; i++)
        {
            parsearguments += argv[i];
        }
        std::string arg;
        for(size_t i = 0; i < variable_name.size(); i++)
        {
            size_t index = 0;
            index = parsearguments.find(variable_name[i]);
            if(index != std::string::npos)
            {   
                for(auto argindex = index + variable_name[i].size(); argindex < parsearguments.size() ; argindex ++ )
                {   
                    if(parsearguments[argindex] == ' ' || parsearguments[argindex] == '-')break;
                    else
                    {
                        if(parsearguments[argindex] == '=') continue;
                        else{
                            arg += parsearguments[argindex];
                        }
                    }
                }
                if( checkPatterns(arg, d_type[i]))
                {
                    args[i] = std::move(arg);
                }
                else
                {
                    std::cerr << "Couldn't parse the string" << arg<<'\n';
                    helpDialog();
                    exit(EXIT_FAILURE);
                } 
                
            }
        }
    }
    
    void helpDialog()const
    {
        std::cout<<"Available commands\n";
        for(size_t i=0; i < value; i++)
        {
            std::cout<<variable_name[i]<<'\t'<<help[i]<<'\n';
        }
        std::cout<<std::endl;
    }

    private:
        int indexes[4] = {0 , 0 , 0 ,0};
        std::array<std::string, value> help{};
        std::array<std::string , value> variable_name{};
        std::array<DATA_TYPE, value> d_type{};
        std::array<std::string, value> args{};

        bool checkPatterns(std::string& str, DATA_TYPE type)const
        {
            switch(type)
            {
                case DATA_TYPE::INTEGER:
                    return is_number(str);
                    break;
                case DATA_TYPE::CHAR:
                    return (str.size() == 1 && ((str[0] >= 97 && str[0]<= 122) || (str[0] >= 65 && str[0]<= 90)));
                    break;
                case DATA_TYPE::FLOATING_POINT:
                    return is_floating_point(str);
                    break;
                case DATA_TYPE::STRING:
                    return true;
                    break;
                default:
                    return false;
            }
        }

        bool is_number(const std::string& s)const
        {
            std::string::const_iterator it = s.begin();
            while (it != s.end() && std::isdigit(*it)) ++it;
            return !s.empty() && it == s.end();
        }

        bool is_floating_point(const std::string& myString) const
        {
            std::istringstream iss(myString);
            float f;
            iss >> std::noskipws >> f; // noskipws considers leading whitespace invalid
            // Check the entire string was consumed and if either failbit or badbit is set
            return iss.eof() && !iss.fail(); 
        }

        typedef std::variant<std::optional<char>, std::optional<int>, std::optional<double>, std::optional<std::string>> Returntype;
        template <typename P>
        Returntype getArgument(const std::string& arg,DATA_TYPE datamode)const
        {
            if (arg.size()==0)
            {
                std::optional<P> nulltype = std::nullopt;
                return nulltype;
            }
            switch (datamode)
            {
                case DATA_TYPE::INTEGER:
                    return std::optional(std::stoi(arg));
                    break;
                case DATA_TYPE::CHAR:
                    return std::optional(arg[0]);
                    break;
                case DATA_TYPE::FLOATING_POINT:
                    return std::optional(std::stod(arg));
                    break;
                default:
                    return std::optional(arg);
            }
        }

};
#endif