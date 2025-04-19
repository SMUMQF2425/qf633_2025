#include <iostream>

class Base {
public:
    virtual void print() {
        std::cout << "Base class" << std::endl;
    }
};

class Derived : public Base {
public:
    void print() override {
        std::cout << "Derived class" << std::endl;
    }
};

int main() {

    //example 1: type safe cast
    if (true) {
        int num_int = 10;
        double num_double = static_cast<double>(num_int); // Convert integer to double
        std::cout << "Converted value: " << num_double << std::endl;
    }
    
    //example 2: type safe up cast along hierachy of class inheritance
    if (true) {
        Base* base_ptr = new Derived();
        Derived* derived_ptr = static_cast<Derived*>(base_ptr);
        derived_ptr->print();
        delete base_ptr; // Clean up memory
    }
    
    return 0;
}