#pragma once

#include <memory>
#include <cstdint>
#include <string>
#include <cstdint>
#include <functional>
#include <iostream>
#include <utility>
#include <cstring>
#include <optional>
#include <vector>

/// UTF-8 string (reuse your previous implementation and modify it)

using CodePoint = uint32_t;

class UTF8String;

class ByteIterator {
public:
    ByteIterator(const UTF8String& s, int i) : str(s), index(i) {}
    uint8_t operator*() const;
    ByteIterator& operator++();
    ByteIterator& operator+=(int n);
    ByteIterator& operator-=(int n);
    ByteIterator operator+(int n) const;
    ByteIterator operator-(int n) const;
    bool operator==(const ByteIterator& other) const;
    bool operator!=(const ByteIterator& other) const;
    ByteIterator begin() const;
    ByteIterator end() const;
private:
    const UTF8String& str;
    int index;
};

class CodePointIterator {
public:
    CodePointIterator(const UTF8String& s, int i) : str(s), index(i) {}
    CodePoint operator*() const;
    CodePointIterator& operator++();
    CodePointIterator& operator--();
    bool operator==(const CodePointIterator& other) const;
    bool operator!=(const CodePointIterator& other) const;
    CodePointIterator begin() const;
    CodePointIterator end() const;
private:
    const UTF8String& str;
    int index;
};

class UTF8String {
public:
    UTF8String() = default;
    UTF8String(const std::string& s);
    UTF8String(const std::vector<CodePoint>&ptr);
    UTF8String(const std::vector<uint8_t>& ptr);
    UTF8String(const UTF8String &ptr);
    UTF8String(UTF8String&& other);

    int get_number_of_bytes(CodePoint ptr);

    int get_byte_count() const;
    int get_point_count() const;
    int get_reserved_count() const;

    void append(const char c);
    void append(const CodePoint cp);

    std::optional<uint8_t> operator[](const size_t idx) const;
    std::optional<CodePoint> nth_code_point(const size_t idx) const;
    friend UTF8String operator+(const UTF8String &lhs,const UTF8String &rhs);
    UTF8String& operator=(const UTF8String &ptr);
    UTF8String& operator+=(const UTF8String& str);
    UTF8String& operator=(UTF8String&& other);
    friend bool operator==(const UTF8String &lhs,const UTF8String &rhs);
    friend bool operator!=(const UTF8String &lhs,const UTF8String &rhs);

    operator std::string() const {
        std::string result;
        for (size_t i = 0; i < points; i++)
        {
            result.push_back(static_cast<char>(buffer.at(i)));
        }
        return result;
    }
    
    ByteIterator bytes() const {
        return ByteIterator(*this, 0);
    }
    
    CodePointIterator codepoints() const {
        return CodePointIterator(*this, 0);
    }


    ~UTF8String() = default;
private:
    size_t points = 0;
    size_t bytes_count = 0;

    std::vector<uint8_t> buffer;
};

class TreeIterator;
class Tree;

/// Binary tree
// Big data that we cannot afford to copy
struct BigData {
    explicit BigData(int value): value(value) {}

    BigData(const BigData&) = delete;
    BigData& operator=(const BigData&) = delete;

    int value;
};

class Tree {
public:
    Tree(std::shared_ptr<BigData> value): value(value) {}
    Tree(int value): value(std::make_shared<BigData>(value)) {}
    BigData& get_value() const { return *value; }
    Tree* get_parent() const { return parent; }
    Tree* get_left_child() const { return leftChild.get(); }
    Tree* get_right_child() const { return rightChild.get(); }
    Tree* get_root() const;
    std::unique_ptr<Tree> set_left_child(std::unique_ptr<Tree> child);
    std::unique_ptr<Tree> set_right_child(std::unique_ptr<Tree> child);
    std::unique_ptr<Tree> take_left_child();
    std::unique_ptr<Tree> take_right_child();
    std::unique_ptr<Tree> take_child(const Tree& child);
    void swap_children();
    void replace_value(std::shared_ptr<BigData> newValue);
    bool has_parent() const { return parent != nullptr; }
    bool is_same_tree_as(const Tree* other) const;
    TreeIterator begin();
    TreeIterator end();
private:
    std::shared_ptr<BigData> value;
    Tree* parent = nullptr;
    std::unique_ptr<Tree> leftChild;
    std::unique_ptr<Tree> rightChild;
};

class TreeIterator {
    Tree* current;
public:
    TreeIterator(Tree* root);
    Tree& operator*();
    TreeIterator& operator++();
    bool operator!=(const TreeIterator& other) const;
};
