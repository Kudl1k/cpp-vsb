#pragma once

#include <iostream>
#include <string>


class Visitor{
    
};


class Value {
public:
    virtual ~Value(){};

    virtual Value* clone() const = 0;

    virtual Value& operator[](int idx) const = 0;

    virtual Value& operator[](const std::string& key) const = 0;

    virtual void accept(Visitor& visitor) = 0;

    virtual int get_value() = 0;

};


class Integer: public Value{
public:
    Integer(int value);
    ~Integer() override;
    Value* clone() const override;
    Value& operator[](int idx) const override;
    Value& operator[](const std::string& key) const override;
    void accept(Visitor& visitor) override;
    int get_value();
private:
    int val;




};

