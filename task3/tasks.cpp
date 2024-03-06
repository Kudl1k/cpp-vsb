#include "tasks.h"

// TODO: implement functions from tasks.h


Parser create_char_parser(char c){
    return [c] (const char* it) -> const char* {
        if (*it == c){
            return it + 1;
        } else {
            return nullptr;
        }
    };
}
//Use std::regex, std::cmatch and std::regex_search to implement this parser.
//Avoid using regex_match to implement other parsers when possible for performance reasons.
Parser regex_match(std::regex e){
    return [e] (const char* ie) -> const char* {
        std::cmatch match;
        bool res = regex_search(ie,match,e);
        if (res) {
            return "";
        } else {
            return nullptr;
        }
    };
}


Parser skip_ws(){
    return [] (std::string input) -> const char* {
        return input.erase(0,input.find_first_not_of(" \t\r\n")).c_str();
    };
}



Parser any_of(std::vector<Parser> p) {
    return[p] (const char* input) -> const char* {
        for (size_t i = 0; i < p.size(); i++)
        {
            const char* res = p[i](input);
            if (res != nullptr) {
                return res;
            }
        }
        return nullptr;
    };
}


Parser repeat(Parser fun,int n){
    return [fun,n](const char* input) -> const char* {
        if (input == nullptr)
        {
            return nullptr;
        }
        const char* value = input;
        for (size_t i = 0; i < n; i++)
        {
            
            const char* next_value = fun(value);
            if (next_value == nullptr)
            {
                return nullptr;
            }
            value = next_value;
        }
        return value;
    };
}


