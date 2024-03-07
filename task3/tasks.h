#include <functional>
#include <iostream>
#include <regex>
#include <vector>

using Parser = std::function<const char*(const char*)>;


Parser create_char_parser(const char c);

Parser regex_match(const char* e);

Parser skip_ws();

Parser any_of(std::vector<Parser> p);

Parser sequence(std::vector<Parser> p);

Parser repeat(Parser fun,size_t n);

Parser create_word_parser(const char* w);
