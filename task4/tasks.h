#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

class Value;
class Integer;
class Array;
class Object;
class Null;


class Visitor {
public:
    virtual ~Visitor() = default;
    virtual void visit(const Integer*) {};
    virtual void visit(const Array* ) {};
    virtual void visit(const Object* ) {};
    virtual void visit(const Null*) {};
};

class MutatingVisitor : public Visitor {
public:
    virtual ~MutatingVisitor() = default;
    virtual void visit(Integer*) {};
    virtual void visit(Array*) {};
    virtual void visit(Object*) {};
    virtual void visit(Null*) {};
};

class RemoveNullVisitor : public MutatingVisitor {
public:
    RemoveNullVisitor() = default;
    ~RemoveNullVisitor() = default;
    void visit(Integer* integer) override;
    void visit(Array* array) override;
    void visit(Object* object) override;
    void visit(Null* null) override;
};

class PrintVisitor : public Visitor {
public:
    PrintVisitor(std::ostream& stream) : stream(stream) {}
    void visit(const Integer* integer)  override;
    void visit(const Array* array)  override;
    void visit(const Object* object)  override;
    void visit(const Null* null)  override;
private:
    std::ostream& stream;
};


class Value {
public:
    virtual ~Value(){};

    virtual Value* clone() const = 0;

    virtual Value* operator[](size_t idx) const = 0;

    virtual Value* operator[](const std::string& key) const = 0;

    virtual void accept( Visitor& visitor) const = 0;

    virtual int get_value() const {return -1;}

    virtual int size() const {return -1;}

    virtual void insert(const std::string& , Value* ){};

};


class Integer : public Value{
public:
    Integer(int value);
    ~Integer() override;
    Value* clone() const override;
    Value* operator[](size_t idx) const override;
    Value* operator[](const std::string& key) const override;
    void accept( Visitor& visitor) const override;
    int get_value() const override;
private:
    int val;
};


class Array : public Value {
public:
    Array();
    Array(std::initializer_list<Value*> init);
    ~Array() override;

    Value* clone() const override;
    Value* operator[](size_t idx) const override;
    Value* operator[](const std::string& key) const override;
    void accept( Visitor& visitor) const override;
    int get_value();

    void remove_nulls();

    int size() const ;
    void append(Value* value);
    void remove(size_t idx);

private:
    std::vector<Value*> values;
};


class Object : public Value {
public:
    Object();
    Object(std::initializer_list<std::pair<std::string, Value*>> init);
    ~Object() override;

    Value* clone() const override;
    Value* operator[](size_t idx) const override;
    Value* operator[](const std::string& key) const override;
    void accept( Visitor& visitor) const override;
    std::unordered_map<std::string, Value*> get_values() const;

    int size() const override;
    void insert(const std::string& key, Value* value) override;
    void remove(const std::string& key);
    std::vector<std::string> keys() const;
private:
    std::unordered_map<std::string, Value*> values;
};


class Null : public Value {
public:
    Null() = default;
    ~Null() override = default;

    Value* clone() const override;

    Value* operator[](size_t idx) const override;

    Value* operator[](const std::string& key) const override;

    void accept( Visitor& visitor) const override;
};