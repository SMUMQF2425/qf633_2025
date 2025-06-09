// C++ program to illustrate rvalue
#include <iostream>

using namespace std;

// lvalue reference to the lvalue passed as the parameter
void printReferenceValue(int& x)
{
	cout << x << endl;
}
// rvalue reference to the rvalue passed as the parameter
void printReferenceValue2(int&& x)
{
	cout << x << endl;
}

int main()
{
	// Given value
	int a{10};

	// Function call is made lvalue & can be assigned to lvalue reference
	printReferenceValue(a);

    // Works fine as the function is called with rvalue
    printReferenceValue2(100);
    //printReferenceValue2(a) //this line will be wrong, cannot bind lvaue with rvalue ref
 
	return 0;
}