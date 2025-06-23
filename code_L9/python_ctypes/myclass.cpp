// myclass.cpp
#include "myclass.h"

MyClass::MyClass(int val) : value(val) {}

int MyClass::get_value() const 
{ 
    return value; 
}

void MyClass::set_value(int val)
{ 
    value = val; 
}
