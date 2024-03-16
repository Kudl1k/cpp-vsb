#include "tasks.h"




// * RemoveNullVisitor class implementation


void RemoveNullVisitor::visit(Integer *)
{
}

void RemoveNullVisitor::visit(Array* array) {
    std::cout << "Before" << std::endl;
    for (size_t i = 0; i < array->size(); ++i) {
        Value* value = (*array)[i];
        if (dynamic_cast<Null*>(value) != nullptr) {
            array->remove(i--);
        } else {
            value->accept(*this);
        }
    }
    std::cout << "After" << std::endl;
}

void RemoveNullVisitor::visit(Object *object)
{

}

void RemoveNullVisitor::visit(Null*)
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
    for (size_t i = 0; i < keys.size(); ++i) {
        if (i != 0) {
            stream << ", ";
        }
        stream << keys[i] << ": ";
        object->operator[](keys[i])->accept(*this);
    }
    stream << "}";
}

void PrintVisitor::visit(const Null*) 
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

Value* Integer::operator[](size_t) const {
    throw std::runtime_error("Cannot use operator[] on an Integer");
}

Value* Integer::operator[](const std::string& ) const {
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

Value* Array::operator[](size_t idx) const {
    if (idx >= values.size())
    {
        return nullptr;
    }
    return this->values[idx];
}

Value* Array::operator[](const std::string&) const {
    throw std::invalid_argument("Cannot use string key to access array element");
}

void Array::accept(Visitor& visitor) const {
    std::cout << "accepted" << std::endl;
    for (size_t i = 0; i < values.size(); ++i) {
        values[i]->accept(visitor);
    }
}

int Array::get_value() {
    throw std::logic_error("get_value is not supported for Array");
}

void Array::remove_nulls(){
    std::cout << "Called remove nulls" << std::endl;
    for (size_t i = 0; i < values.size(); i++)
    {
        auto value = values[i];
        if(dynamic_cast<Null*>(value) != nullptr) {
            delete value;
            values.erase(values.begin() + i);
            --i;
        } else if(auto arrayValue = dynamic_cast<Array*>(value)) {
            arrayValue->remove_nulls();
        }
    }
    
}

int Array::size() const {
    return this->values.size();
}

void Array::append(Value* value){
    values.push_back(value);
}

void Array::remove(size_t idx) {
    if (idx < values.size()) {
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

Value *Object::operator[](size_t) const{
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


Value* Null::operator[](size_t) const  {
    throw std::runtime_error("Cannot use operator[] on an Null");
}

Value* Null::operator[](const std::string& ) const  {
    throw std::runtime_error("Cannot use operator[] on an Null");

}

void Null::accept( Visitor& visitor) const {
    visitor.visit(this);
}


