#include "tasks.h"

bool Array::operator==(const Array& other) const {
    return other.items == this->items;
}

bool Object::operator==(const Object& other) const {
    return other.items == this->items;
}

bool String::operator==(const String& other) const {
    return other.value == this->value;
}

bool Null::operator==(const Null&) const {
    return true;
}

bool Number::operator==(const Number& other) const {
    return other.value == this->value;
}

bool Boolean::operator==(const Boolean& other) const {
    return other.value == this->value;
}

std::optional<Value> parse_json(std::istream& is) {
    char ch;
    if (!(is >> ch)) {
        return std::nullopt;
    }

    if (ch == 'n') {
        if (is.get() == 'u' && is.get() == 'l' && is.get() == 'l') {
            return Null();
        } else {
            return std::nullopt;
        }
    } else if (ch == 't' || ch == 'f') {
        if (ch == 't') {
            if (is.get() == 'r' && is.get() == 'u' && is.get() == 'e') {
                return Boolean(true);
            } else {
                return std::nullopt;
            }
        } else if (ch == 'f') {
            if (is.get() == 'a' && is.get() == 'l' && is.get() == 's' && is.get() == 'e') {
                return Boolean(false);
            } else {
                return std::nullopt;
            }
        }
    } else if (ch == '\"') {
        std::string value;
        char c;
        while (is.get(c)) {
            if (c == '\"') {
                return String(value);
            } else if (c == '\\') {
                if (!is.get(c)) {
                    return std::nullopt;
                }
                switch (c) {
                    case '\"':
                        value += '\"';
                        break;
                    case '\\':
                        value += '\\';
                        break;
                    default:
                        return std::nullopt;
                }
            } else {
                value += c;
            }
        }
        return std::nullopt;
    } else if (std::isdigit(ch) || ch == '-') {
        std::string number_str;
        number_str += ch;
        bool decimal_point_encountered = false;

        while (is.get(ch)) {
            if (std::isdigit(ch)) {
                number_str += ch;
            } else if (ch == '.') {
                if (decimal_point_encountered) {
                    is.unget();
                    break;
                } else {
                    number_str += ch;
                    decimal_point_encountered = true;
                }
            } else {
                is.unget();
                break;
            }
        }

        try {
            double number = std::stod(number_str);
            return Number(number);
        } catch (const std::invalid_argument& e) {
            return std::nullopt;
        } catch (const std::out_of_range& e) {
            return std::nullopt;
        }
    } else if (ch == '[') {
        // Parse array
        Array array;
        while (std::isspace(is.peek())) {
            is.get();
        }

        if (is.peek() == ']') {
            is.get();
            return array;
        }

        while (true) {
            auto value = parse_json(is);
            if (!value) {
                return std::nullopt;
            }
            array.items.push_back(*value);

            while (std::isspace(is.peek())) {
                is.get();
            }
            ch = is.get();
            if (ch == ']') {
                break;
            }
            else if (ch != ',') {
                return std::nullopt;
            }
            while (std::isspace(is.peek())) {
                is.get();
            }
        }

        return array;
    } else if (ch == '{') {
        Object object;
        while (std::isspace(is.peek())) {
            is.get();
        }
        if (is.peek() == '}') {
            is.get();
            return object;
        }
        while (true) {
            if (is.get() != '\"') {
                return std::nullopt;
            }
            std::string key;
            char c;
            while (is.get(c)) {
                if (c == '\"') {
                    break;
                } else if (c == '\\') {
                    if (!is.get(c)) {
                        return std::nullopt;
                    }
                    switch (c) {
                        case '\"':
                            key += '\"';
                            break;
                        case '\\':
                            key += '\\';
                            break;
                        default:
                            return std::nullopt;
                    }
                } else {
                    key += c;
                }
            }
            while (std::isspace(is.peek())) {
                is.get();
            }
            if (is.get() != ':') {
                return std::nullopt;
            }
            while (std::isspace(is.peek())) {
                is.get();
            }
            auto value = parse_json(is);
            if (!value) {
                return std::nullopt;
            }
            object.items[key] = *value;
            while (std::isspace(is.peek())) {
                is.get();
            }
            ch = is.get();
            if (ch == '}') {
                break;
            }
            else if (ch != ',') {
                return std::nullopt;
            }
            while (std::isspace(is.peek())) {
                is.get();
            }
        }
        return object;
    } else {
        // Invalid JSON
        return std::nullopt;
    }
}

std::ostream &operator<<(std::ostream &os, const Value &value)
{
    std::visit(overloaded{
        [&os](const String &v) { os << '"' << v.value << '"'; },
        [&os](const Number &v) { os << v.value; },
        [&os](const Boolean &v) { os << (v.value ? "true" : "false"); },
        [&os](const Null &) { os << "null"; },
        [&os](const Array &v) {
            os << '[';
            for (size_t i = 0; i < v.items.size(); ++i) {
                if (i != 0) os << ',';
                os << v.items[i];
            }
            os << ']';
        },
        [&os](const Object &v) {
            os << '{';
            bool first = true;
            for (const auto& [key, val] : v.items) {
                if (!first) os << ',';
                first = false;
                os << '"' << key << "\":" << val;
            }
            os << '}';
        }
    }, value);
    return os;
}

std::vector<uint8_t> serialize(const Value &value)
{
    return {};
}

Value deserialize(const std::vector<uint8_t>& data) {
    return Null();
}
