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



