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
        return std::nullopt;  // Stream is empty
    }

    if (ch == '{') {
        // Parse an object
        Object obj;
        while (is >> ch && ch != '}') {
            if (ch != '"') {
                return std::nullopt;  // Expected a string key
            }

            // Parse the key
            std::string key;
            while (is.get(ch) && ch != '"') {
                if (ch == '\\') {
                    if (!(is.get(ch))) {
                        return std::nullopt;  // Unexpected end of string
                    }
                }
                key += ch;
            }

            if (!(is >> ch) || ch != ':') {
                return std::nullopt;  // Expected a colon
            }

            // Parse the value
            auto value = parse_json(is);
            if (!value) {
                return std::nullopt;  // Invalid value
            }

            obj.items[key] = *value;

            // Consume the comma if there is one
            is >> ch;
            if (ch != ',') {
                is.putback(ch);
            }
        }

        return Value{obj};
    } else if (ch == '[') {
        // Parse an array
        Array arr;
        while (is >> ch && ch != ']') {
            is.putback(ch);

            // Parse the value
            auto value = parse_json(is);
            if (!value) {
                return std::nullopt;  // Invalid value
            }

            arr.items.push_back(*value);

            // Consume the comma if there is one
            is >> ch;
            if (ch != ',') {
                is.putback(ch);
            }
        }

        return Value{arr};
    } else if (ch == '"') {
        // Parse a string
        std::string value;
        while (is.get(ch) && ch != '"') {
            if (ch == '\\') {
                if (!(is.get(ch))) {
                    return std::nullopt;  // Unexpected end of string
                }
            }
            value += ch;
        }

        return Value{String{value}};
    } else if (ch == 'n') {
        // Parse a null
        char u, l1, l2;
        if (is.get(u) && is.get(l1) && is.get(l2) && u == 'u' && l1 == 'l' && l2 == 'l') {
            return Value{Null{}};
        } else {
            return std::nullopt;  // Invalid "null" value
        }
    } else if (ch == 't' || ch == 'f') {
        // Parse a boolean
        char rest[4];
        if (ch == 't') {
            if (!is.read(rest, 3) || std::strncmp(rest, "rue", 3) != 0) {
                return std::nullopt;  // Invalid "true" value
            }
            return Value{Boolean{true}};
        } else {
            if (!is.read(rest, 4) || std::strncmp(rest, "alse", 4) != 0) {
                return std::nullopt;  // Invalid "false" value
            }
            return Value{Boolean{false}};
        }
    } else if (std::isdigit(ch) || ch == '-') {
        // Parse a number
        std::string value(1, ch);
        while (is.get(ch) && (std::isdigit(ch) || ch == '.' || ch == 'e' || ch == 'E' || ch == '+' || ch == '-')) {
            value += ch;
        }
        is.putback(ch);
        return Value{Number{std::stod(value)}};
    }

    return std::nullopt;  // Unsupported value type
}

std::ostream &operator<<(std::ostream &os, const Value &value)
{
    // TODO: insert return statement here
}

std::vector<uint8_t> serialize(const Value &value)
{
    return {};
}

Value deserialize(const std::vector<uint8_t>& data) {
    return Null();
}
