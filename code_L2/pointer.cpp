#include <iostream>

using namespace std;
int main() {

    //example of pointer
    if (0) {
        // Declare a variable
    int number = 42;

    // Declare a pointer to an integer and initialize it with the address of 'number'
    int *pointerToNumber = &number;

    // Display the value of 'number' and the memory address it occupies
    std::cout << "Value of 'number': " << number << std::endl;
    std::cout << "Memory address of 'number': " << &number << std::endl;

    // Display the value of 'pointerToNumber' (memory address)
    std::cout << "Value of 'pointerToNumber' (memory address): " << pointerToNumber << std::endl;

    // Access the value of 'number' through the pointer and display it
    std::cout << "Value of 'number' accessed through the pointer: " << *pointerToNumber << std::endl;

    }
    
    //example of reference
    if (0) {
    
        // Declare a variable
        int number = 42;

        // Declare a reference to an integer and initialize it with the variable 'number'
        int& referenceToNumber = number;

        // Display the value of 'number' and the value through the reference
        std::cout << "Value of 'number': " << number << std::endl;
        std::cout << "Address of number: " << &number << std::endl;
        std::cout << "Value through the reference: " << referenceToNumber << std::endl;
        std::cout << "Address of the reference: " << &referenceToNumber << std::endl;

        // Modify the value through the reference
        referenceToNumber = 99;
       
        // Display the modified value of 'number'
        std::cout << "Modified value of 'number': " << number << std::endl;

    }
    
    //pointer vs reference
    if(1){
        // re-assign reference to another variable leads to error
        int num = 40;
        int otherNum = 20;
        std::cout << "address of number: " << &num << std::endl;
        std::cout << "address of otherNum: " << &otherNum << std::endl;
        int& referenceToNumber = num;
        std::cout << "address of reference: " << &referenceToNumber << std::endl;

        referenceToNumber = otherNum;
        std::cout << "address of reference: " << &referenceToNumber << std::endl;
        referenceToNumber = 15;
        std::cout << "Modified value of 'number': " << num << std::endl;
        otherNum = referenceToNumber;
        std::cout << "Modified value of 'other number': " << otherNum << std::endl;

        referenceToNumber = otherNum;
        std::cout << "address of reference: " << &referenceToNumber << std::endl;

        int* ptr = &num;
        
        *ptr = 25;
        std::cout << "Modified value of 'number': " << num << std::endl;
        std::cout << "address of 'number': " << ptr << std::endl;
        ptr++;
        std::cout << "Modified address of ptr: " << ptr << std::endl;
        std::cout << "Modified value of ptr: " << *ptr << std::endl;
        ptr = &otherNum;
        std::cout << "Modified address of ptr: " << ptr << std::endl;
        
    }

    return 0;
}
    

