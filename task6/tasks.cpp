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
    return std::nullopt;
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
        [&data](const Null) {
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

    auto read_size = [&data, &index](size_t& value) {
        std::memcpy(&value, &data[index], sizeof(size_t));
        index += sizeof(size_t);
    };

    auto read_double = [&data, &index]() -> double {
        double value;
        std::memcpy(&value, &data[index], sizeof(double));
        index += sizeof(double);
        return value;
    };

    auto read_string = [&data, &index]() -> std::string {
        size_t size;
        std::memcpy(&size, &data[index], sizeof(size_t));
        index += sizeof(size_t);
        std::string str(data.begin() + index, data.begin() + index + size);
        index += size;
        return str;
    };

    auto read_value = [&data, &index, &read_size, &read_double, &read_string]() -> Value {
        std::function<Value()> read_recursive_value;
        read_recursive_value = [&]() -> Value {
            uint8_t type_id = data[index++];
            switch (type_id) {
                case 0: return Null();
                case 1: return Boolean(data[index++] != 0);
                case 2: return Number(read_double());
                case 3: return String(read_string());
                case 4: {
                    size_t size;
                    read_size(size);
                    Array array;
                    for (size_t i = 0; i < size; ++i) {
                        Value val = read_recursive_value(); 
                        array.items.push_back(val);
                    }
                    return array;
                }
                case 5: {
                    size_t size;
                    read_size(size);
                    Object obj;
                    for (size_t i = 0; i < size; ++i) {
                        std::string key = read_string();
                        Value val = read_recursive_value();
                        obj.items[key] = val;
                    }
                    return obj;
                }
                default: throw std::runtime_error("Invalid type ID");
            }
        };
        return read_recursive_value();
    };
    return read_value();
}
