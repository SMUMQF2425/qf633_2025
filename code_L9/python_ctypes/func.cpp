#include <iostream>
using namespace std;

int function_inside_static_lib(int arg1, int arg2)
{
    cout << "In function_inside_static_lib()" << endl;

    // Return the sum
    int result = arg1 + arg2;
    return result;
}