#include "tasks.h"

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
    // Implementation...
}

Value& Integer::operator[](const std::string& key) const {
    // Implementation...
}

void Integer::accept(Visitor& visitor) {
    // Implementation...
}

int Integer::get_value(){
    return val;
}
