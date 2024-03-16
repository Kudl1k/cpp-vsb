#include "tasks.h"


// * Integer class implementation


Integer::Integer(int value) {
    val = value;
}

Integer::~Integer() = default;

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


Array::Array() = default;

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
    if (idx >= values.size())
    {
        return nullptr;
    }
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

int Array::size() const {
    return this->values.size();
}

void Array::append(Value* value){
    values.push_back(value);
}




// * Object class implementation


Object::Object(){
}

Object::Object(std::initializer_list<std::pair<std::string, Value *>> init){
    for (auto pair : init){
        values[pair.first] = pair.second;
    }
}

Object::~Object(){
    for (auto pair : values){
        delete pair.second;
    }
}

Value *Object::clone() const{
    Object *newobj = new Object{};

    for(auto &pair : values){
        newobj->insert(pair.first,pair.second->clone());
    }
    return newobj;
}

Value *Object::operator[](int idx) const{
        throw std::invalid_argument("Cannot use indexing to access Object element");
}

Value *Object::operator[](const std::string &key) const{
    if (values.find(key) == values.end())
    {
        return nullptr;
    } else {
        return values.find(key)->second;
    }
}

void Object::accept(Visitor &visitor){
}

int Object::size() const {
    return values.size();
}

void Object::insert(const std::string &key, Value *value){
    if (values.find(key) != values.end()) {
        delete values.find(key)->second;
    }
    values[key] = value;
}

void Object::remove(const std::string &key){
    values.erase(key);
}

std::vector<std::string> Object::keys() const{
    std::vector<std::string> keys;
    for (auto key: values){
        keys.push_back(key.first);
    }
    std::sort(keys.begin(),keys.end());
    return keys;
}




// * Null class implementation


Value* Null::clone() const  {
    return new Null();
}


Value* Null::operator[](int idx) const  {
    throw std::runtime_error("Cannot use operator[] on an Null");
}

Value* Null::operator[](const std::string& key) const  {
        throw std::runtime_error("Cannot use operator[] on an Null");

}

void Null::accept(Visitor& visitor)  {
    // Do nothing
}