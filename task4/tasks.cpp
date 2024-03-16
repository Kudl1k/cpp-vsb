#include "tasks.h"




// * RemoveNullVisitor class implementation


void RemoveNullVisitor::visit(Integer *integer)
{
}

void RemoveNullVisitor::visit(Array* array) {
    for (size_t i = 0; i < array->size(); ++i) {
        array->operator[](i)->accept(*this);
    }
}

void RemoveNullVisitor::visit(Object *object)
{
    std::vector<std::string> keysToRemove;
    for (const auto& pair : object->get_values()) {
        if (dynamic_cast<Null*>(pair.second) != nullptr) {
            keysToRemove.push_back(pair.first);
        } else {
            pair.second->accept(*this);
        }
    }

    // Remove the Null values after visiting all the values
    for (const auto& key : keysToRemove) {
        object->remove(key);
    }

    // Print keys after removal for debugging
    std::cout << "Keys after removal: ";
    for (const auto& key : object->keys()) {
        std::cout << key << ", ";
    }
    std::cout << std::endl;
}

void RemoveNullVisitor::visit(Null *null)
{

}




// * PrintVisitor class implementation


void PrintVisitor::visit(const Integer *integer) 
{
    stream << integer->get_value();
}

void PrintVisitor::visit(const Array *array) 
{
    stream << "[";
    for (int i = 0; i < array->size(); ++i) {
        if (i != 0) {
            stream << ", ";
        }
        array->operator[](i)->accept(*this);
    }
    stream << "]";
}

void PrintVisitor::visit(const Object *object) 
{
    stream << "{";
    std::vector<std::string> keys = object->keys();
    for (int i = 0; i < keys.size(); ++i) {
        if (i != 0) {
            stream << ", ";
        }
        stream << keys[i] << ": ";
        object->operator[](keys[i])->accept(*this);
    }
    stream << "}";
}

void PrintVisitor::visit(const Null *null) 
{
    stream << "null";
}





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

void Integer::accept(Visitor& visitor) const{
    visitor.visit(this);
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
    for (auto* value: values){
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

void Array::accept( Visitor& visitor) const {
    visitor.visit(this);
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

void Array::remove(int idx) {
    if (idx >= 0 && idx < values.size()) {
        values.erase(values.begin() + idx);
    }
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

void Object::accept( Visitor &visitor) const{
    visitor.visit(this);
}

std::unordered_map<std::string, Value *> Object::get_values() const
{
    return values;
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
    auto it = values.find(key);
    if (it != values.end()) {
        delete it->second; // delete the Value object
        values.erase(it);
    }
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

void Null::accept( Visitor& visitor) const {
    visitor.visit(this);
}


