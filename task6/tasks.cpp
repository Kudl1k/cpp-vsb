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
        [&os](const String &v) { 
            os << '"';
            for (char c : v.value) {
                if (c == '\\' || c == '"') {
                    os << '\\';
                }
                os << c;
            }
            os << '"';
        },
        [&os](const Number &v) { os << v.value; },
        [&os](const Boolean &v) { os << (v.value ? "true" : "false"); },
        [&os](const Null &) { os << "null"; },
        [&os](const Array &v) {
            os << '[';
            for (size_t i = 0; i < v.items.size(); ++i) {
                if (i != 0) os << ", ";
                os << v.items[i];
            }
            os << ']';
        },
        [&os](const Object &v) {
            os << '{';
            bool first = true;
            for (const auto& [key, val] : v.items) {
                if (!first) os << ", ";
                first = false;
                os << '"' << key << "\": " << val;
            }
            os << '}';
        }
    }, value);
    return os;
}

std::vector<uint8_t> serialize(const Value &value)
{
    std::vector<uint8_t> data;

    std::visit(overloaded{
        [&data](const Null &v) {
            data.push_back(0);
        },
        [&data](const Boolean &v) {
            data.push_back(1);
            data.push_back(v.value ? 1 : 0);
        },
        [&data](const Number &v) {
            data.push_back(2);
            uint64_t num;
            std::memcpy(&num, &v.value, sizeof num);
            for (int i = 0; i < 8; ++i) {
                data.push_back((num >> (i * 8)) & 0xFF);
            }
        },
        [&data](const String &v) {
            data.push_back(3);
            size_t size = v.value.size();
            for (int i = 0; i < 8; ++i) {
                data.push_back((size >> (i * 8)) & 0xFF);
            }
            for (char c : v.value) {
                data.push_back(c);
            }
        },
        [&data](const Array &v) {
            data.push_back(4);
            size_t size = v.items.size();
            for (int i = 0; i < 8; ++i) {
                data.push_back((size >> (i * 8)) & 0xFF);
            }
            for (const Value &item : v.items) {
                std::vector<uint8_t> item_data = serialize(item);
                data.insert(data.end(), item_data.begin(), item_data.end());
            }
        },
        [&data](const Object &v) {
            data.push_back(5);
            size_t size = v.items.size();
            for (int i = 0; i < 8; ++i) {
                data.push_back((size >> (i * 8)) & 0xFF);
            }
            for (const auto& [key, val] : v.items) {
                std::vector<uint8_t> key_data = serialize(String(key));
                data.insert(data.end(), key_data.begin() + 1, key_data.end());
                std::vector<uint8_t> val_data = serialize(val);
                data.insert(data.end(), val_data.begin(), val_data.end());
            }
        }
    }, value);

    return data;
}

Value deserialize(const std::vector<uint8_t>& data) {
    size_t index = 0;
    if (index >= data.size()) {
        throw std::runtime_error("Unexpected end of data during deserialization.");
    }

    auto readSize = [&data, &index]() -> size_t {
        size_t size = 0;
        for (int i = 0; i < 8; ++i) {
            size |= static_cast<size_t>(data[index++]) << (i * 8);
        }
        return size;
    };

    uint8_t type = data[index++];
    switch (type) {
        case 0: // Null
            return Null();
        case 1: { // Boolean
            bool value = data[index++] != 0;
            return Boolean{value};
        }
        case 2: { // Number
            uint64_t num = 0;
            for (int i = 0; i < 8; ++i) {
                num |= static_cast<uint64_t>(data[index++]) << (i * 8);
            }
            double value;
            std::memcpy(&value, &num, sizeof(double));
            return Number{value};
        }
        case 3: { // String
            size_t size = readSize();
            std::string value;
            value.resize(size); // Resize the string to the expected size
            for (size_t i = 0; i < size; ++i) {
                value[i] = data[index++];
            }
            return String{value};
        }
        case 4: { // Array
            size_t size = readSize();
            Array array;
            for (size_t i = 0; i < size; ++i) {
                array.items.push_back(std::get<String>(deserialize(data)));
            }
            return array;
        }
        case 5: { // Object
            size_t size = readSize();
            Object obj;
            for (size_t i = 0; i < size; ++i) {
                std::string key = std::get<String>(deserialize(data)).value;
                obj.items[key] = std::get<String>(deserialize(data));
            }
            return obj;
        }
        default:
            throw std::runtime_error("Unknown type during deserialization.");
    }
}