
// myclass.h
#pragma once

class MyClass {
public:
    MyClass(int val);
    int get_value() const;
    void set_value(int val);
private:
    int value;
};
