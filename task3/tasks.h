#include <functional>
#include <iostream>
#include <regex>
#include <vector>

using Parser = std::function<const char*(const char*)>;


Parser create_char_parser(char c);

Parser regex_match(std::regex e);

Parser skip_ws();

Parser any_of(std::vector<Parser> p);

Parser repeat(Parser fun,int n);