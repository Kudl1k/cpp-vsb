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

Value& Integer::operator[](int idx) const {
    throw std::runtime_error("Cannot use operator[] on an Integer");
}

Value& Integer::operator[](const std::string& key) const {
    throw std::runtime_error("Cannot use operator[] on an Integer");
}

void Integer::accept(Visitor& visitor) {
    visitor.visit(*this);
}

int Integer::get_value(){
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

}

Value& Array::operator[](int idx) const {

}

Value& Array::operator[](const std::string& key) const {

}

void Array::accept(Visitor& visitor) {

}

int Array::get_value(){

}

size_t Array::size() const{

}

void Array::apped(Value* value){

}



