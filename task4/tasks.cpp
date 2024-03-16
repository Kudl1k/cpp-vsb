#include "tasks.h"


// * Integer class implementation


Integer::Integer(int value) {
    val = value;
}

Integer::~Integer() {
    std::cout<< "Destuctor called" << std::endl;
}

Value* Integer::clone() const {
    return new Integer{this->val};
}

Value* Integer::operator[](int idx) const {
    throw std::runtime_error("Cannot use operator[] on an Integer");
}

Value* Integer::operator[](const std::string& key) const {
    throw std::runtime_error("Cannot use operator[] on an Integer");
}

void Integer::accept(Visitor& visitor) {
    //visitor.visit(*this);
}

int Integer::get_value() const{
    return val;
}




// * Array class implementation


Array::Array(){
    // * Can be empty
}

Array::Array(std::initializer_list<Value*> init){
    values = init;
}

Array::~Array(){
    for (auto value: values){
        delete value;
    }
}

Value* Array::clone() const {
    auto newarr = new Array{};
    for (size_t i = 0; i < this->values.size(); i++)
    {
        newarr->append(this->values[i]->clone());
    }
    return newarr;
}

Value* Array::operator[](int idx) const {
    return this->values[idx];
}

Value* Array::operator[](const std::string& key) const {
    throw std::invalid_argument("Cannot use string key to access array element");
}

void Array::accept(Visitor& visitor) {

}

int Array::get_value() {
    throw std::logic_error("get_value is not supported for Array");
}

size_t Array::size() const{
    return this->values.size();
}

void Array::append(Value* value){
    values.push_back(value);
}




// * Object class implementation


Object::Object(){
}

Object::Object(std::initializer_list<std::pair<std::string, Value *>> init){
}

Object::~Object(){
}

Value *Object::clone() const{
    return nullptr;
}

Value *Object::operator[](const std::string &key) const{
    return nullptr;
}

void Object::accept(Visitor &visitor){
}

size_t Object::size() const{
    return size_t();
}

void Object::insert(const std::string &key, Value *value){
}
