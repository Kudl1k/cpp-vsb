#include "tasks.h"

// TODO: implement functions from tasks.h


Parser create_char_parser(const char c){
    return [c] (const char* it) -> const char* {
        if (it == nullptr)
        {
            return nullptr;
        }
        
        if (*it == c){
            return it + 1;
        } else {
            return nullptr;
        }
    };
}
//Use std::regex, std::cmatch and std::regex_search to implement this parser.
//Avoid using regex_match to implement other parsers when possible for performance reasons.
Parser regex_match(const char* e){
    return [e] (const char* ie) -> const char* {
        std::cmatch match;
        bool res = regex_search(ie,match,std::regex(e));
        if (res) {    
            return ie + match.length();
        } else {
            return nullptr;
        }
    };
}


Parser skip_ws(){
    return [] (const char* input) -> const char* {
        if (input == nullptr)
        {
            return nullptr;
        }
        while (*input != '\0' && isspace(*input))
        {
            ++input;
        }
        return input;  
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


Parser sequence(std::vector<Parser> p){
    return [p] (const char* input) -> const char* {
        const char* cur = input;
        for (size_t i = 0; i < p.size(); i++)
        {
            const char* next = p[i](cur);
            if (next == nullptr)
            {
                return nullptr;
            }
            cur = next;
        }
        return cur;
    };
}


Parser repeat(Parser fun,size_t n){
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


Parser create_word_parser(const char* w){
    return [w] (const char* it) -> const char*{
        if (it == nullptr)
        {
            return nullptr;
        }
        size_t word_len = strlen(w);
        size_t it_len = strlen(it);
        if (it_len < word_len) {
            return nullptr;
        }
        for (size_t i = 0; i < (it_len - word_len); i++)
        {
            if (strncmp(it + i,w, word_len) == 0)
            {
                return it + i + word_len;
            }
        }
        return nullptr;
    };
}




