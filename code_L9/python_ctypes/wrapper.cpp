// wrapper.cpp
#include "myclass.h"

extern "C" {
    MyClass* MyClass_new(int val) {
        return new MyClass(val);
    }
    int MyClass_get_value(MyClass* obj) {
        return obj->get_value();
    }
    void MyClass_set_value(MyClass* obj, int val) {
        obj->set_value(val);
    }
    void MyClass_delete(MyClass* obj) {
        delete obj;
    }
}
