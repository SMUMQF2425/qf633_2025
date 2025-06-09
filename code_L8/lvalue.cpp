// C++ program to illustrate the lvalue and rvalue
#include <iostream>
using namespace std;

// Creating the references of the parameter passed to the function
void swap(int& x, int& y)
{
	int temp = x;
	x = y;
	y = temp;
}

int main()
{
	//example1: basic use of lvalue and rvalue
    if (false) {
        int a = 10;

        // Declaring lvalue reference (i.e variable a)
        int& lref = a;

        // Declaring rvalue reference
        int&& rref = 20;

        // Print the values
        cout << "lref = " << lref << endl;
        cout << "rref = " << rref << endl;

        // Value of both a and lref is changed
        lref = 30;
        cout << "a = " << a <<endl;

        // Value of rref is changed
        rref = 40;
        cout << "lref = " << lref << endl;
        cout << "rref = " << rref << endl;

        // This line will generate an error as l-value cannot be assigned to the r-value references
        //int &&ref = a;
    }
    
    if (true) {
        // Given values
	    int a{10}, b{20};
	    cout << "a = " << a << " b = " << b << endl;

        // Call by Reference, using lvalue reference
        swap(a, b);
        cout << "a = " << a << " b = " << b << endl;

    }
    
	
    return 0;
}
