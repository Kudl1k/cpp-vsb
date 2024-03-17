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

class MutatingVisitor {
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
    void visit(Integer*) {};
    void visit(Array* array) override;
    void visit(Object* object) override;
    void visit(Null*) {};
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
    virtual void accept( MutatingVisitor& visitor) = 0;
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
    void accept( MutatingVisitor& visitor) override;
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
    void accept( MutatingVisitor& visitor) override;
    int get_value();
    int size() const ;

    void append(Value* value);
    void remove(size_t idx);
private:
    std::vector<Value*> values;
};


class Object : public Value {
public:
    Object(){};
    Object(std::initializer_list<std::pair<std::string, Value*>> init);
    ~Object() override;
    Value* clone() const override;
    Value* operator[](size_t idx) const override;
    Value* operator[](const std::string& key) const override;
    void accept( Visitor& visitor) const override;
    void accept( MutatingVisitor& visitor) override;
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
    void accept( MutatingVisitor& visitor) override;
};


/*
* Questions
? Can individual elements be shared among different JSON containers (array/object) with the mentioned
? design? Can you take one specific element (e.g. an integer) allocated on the heap and place it inside
? an array and also inside an object?

Nemůžou, protože každý ten JSON container má své unikátní elementy.
Nemůžu, musím si vytvořit jeho kopii.


? Could references (or non-owned pointers) be used for storing the elements in JSON containers?
? How would it affect the usability of arrays and objects? Try it and see how easy or hard it is :)

Myslím si, že kdyby se to ukládalo referenci, tak by to přidalo na složitosti co se týče kontrolování paměti,
kde by jsme museli dávat pozor na jejich lifetime.
Tak byla by tam větší šance na memory leaky a omezovalo by to jeho flexibilnost.


? Think about this interface. What are its benefits or disadvantages? Is it better to put the
? indexers into the root `Value` element? Or should it only be supported by types that actually
? implement indexing (arrays/objects)? Think about the trade-offs (compile-time safety vs ergonomics).
? Think about the return type. What should it be? Is `std::optional` required here?

 Tak výhoda je to že tě to nechá zkompilovat, ale pak ti to hodí exception při runtime.
 std::optional by se hodil v případě kdy klíč nebo index není.



? How can you implement copying for a polymorphic object hierarchy? What should be the return type
? of the `clone` method? If you are interested, look for "C++ covariance".

Clone metoda bude vrace pointer na base classu Value


? Think about the constness of the `accept` method and of the methods in the visitor. What
? parameter type should they take? Should it be const or not?

Pokud je immutable visitor tak to bude const pokud to je mutable tak nebude const.
Parametr bere classu kterou posilám ig.


? How would the implementation look like if you have used e.g. algebraic data types instead?
? Would you need the Visitor design pattern in that case?

Implementace by asi určitě vypadala jinak, a tím pádem by jsme nejspíš visitora nepotřebovali.


*/