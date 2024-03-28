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
                key += ch;
            }

            if (!(is >> ch) || ch != ':') {
                return std::nullopt;  // Expected a colon
            }

            // Parse the value (only strings are supported in this example)
            if (!(is >> ch) || ch != '"') {
                return std::nullopt;  // Expected a string value
            }

            std::string value;
            while (is.get(ch) && ch != '"') {
                value += ch;
            }

            obj.items[key] = String{value};

            // Consume the comma if there is one
            is >> ch;
            if (ch != ',') {
                is.putback(ch);
            }
            if (ch == 'n') {
                char u, l1, l2;
                if (is.get(u) && is.get(l1) && is.get(l2) && u == 'u' && l1 == 'l' && l2 == 'l') {
                    return Value{Null{}};
                } else {
                    return std::nullopt;  // Invalid "null" value
                }
            }
        }

        return Value{obj};
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
