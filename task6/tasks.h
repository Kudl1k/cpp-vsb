#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <variant>
#include <map>
#include <optional>
#include <cstring>
#include <functional>

// JSON document: implement parsing.
// JSON is implemented using algebraic data types here, it should be more practical than working
// with the previous OOP hierarchy and pointers.
struct Boolean {
    bool value;

    Boolean() = default;

    Boolean(bool value): value(value) {};

    bool operator==(const Boolean& other) const;
};

struct Number {
    double value;

    Number() = default;

    Number(double value): value(value) {};

    bool operator==(const Number& other) const;
};

struct Null {
    bool operator==(const Null&) const;
};

struct String {
    std::string value;

    String() = default;

    String(std::string value): value(value) {};

    bool operator==(const String& other) const;
};

struct Array;
struct Object;


using Value = std::variant<Boolean, Number, String, Null, Array, Object>;

struct Array {
    std::vector<Value> items;

    Array() = default;

    bool operator==(const Array& other) const;
};

struct Object {
    std::map<std::string, Value> items;

    bool operator==(const Object& other) const;

    Object() = default;

    Object(const std::map<std::string, Value>& items) : items(items) {}

    [[nodiscard]] std::vector<std::string> keys() const {
        std::vector<std::string> keys;
        keys.reserve(this->items.size());
        for (const auto& key: this->items) {
            keys.push_back(key.first);
        }
        return keys;
    }
};

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

// TODO: Impl<ement the following functions
std::optional<Value> parse_json(std::istream& is);
std::ostream& operator<<(std::ostream& os, const Value& value);

std::vector<uint8_t> serialize(const Value& value);
Value deserialize(const std::vector<uint8_t>& data);
