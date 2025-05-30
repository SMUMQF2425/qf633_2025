#include <iostream>

void processData(const int *data, size_t size)
{
	std::cout << "Processing data: ";
	for (size_t i = 0; i < size; ++i)
	{
		std::cout << data[i] << " ";
	}
	std::cout << std::endl;

	// This function only reads the data. It does NOT:
	// 1. Store the pointer for later use.
	// 2. Deallocate the memory pointed to by 'data'.
	// 3. Modify the pointer 'data' to point to newly allocated memory.
}

// Function that takes ownership of the raw pointer
void processAndOwnData(int *data)
{
	if (data != nullptr)
	{
		std::cout << "Processing data (owner): " << *data << std::endl;
		// This function now owns the memory and is responsible for deleting it.
		delete data;
		data = nullptr; // Good practice to nullify the local pointer
		std::cout << "Memory owned by processAndOwnData has been deleted." << std::endl;
	}
	else
	{
		std::cout << "Received a null pointer." << std::endl;
	}
}

int main()
{
	int *myArray = new int[5];
	for (int i = 0; i < 5; ++i)
	{
		myArray[i] = i * 10;
	}

	std::cout << "Before processing:" << std::endl;
	for (int i = 0; i < 5; ++i)
	{
		std::cout << myArray[i] << " ";
	}
	std::cout << std::endl;

	// Passing the raw pointer 'myArray' to the function.
	// Ownership of the memory remains with 'main'.
	processData(myArray, 5);

	std::cout << "After processing:" << std::endl;
	for (int i = 0; i < 5; ++i)
	{
		std::cout << myArray[i] << " ";
	}
	std::cout << std::endl;

	// 'main' is responsible for deallocating the memory.
	delete[] myArray;
	myArray = nullptr; // Good practice to nullify after deleting

	int *myNumber = new int(42);
	std::cout << "Before passing ownership: " << *myNumber << std::endl;

	// Passing the raw pointer 'myNumber' to the function, transferring ownership.
	processAndOwnData(myNumber);
	myNumber = nullptr; // 'main' no longer owns the memory

	// Attempting to access 'myNumber' here would be dangerous as the memory has been freed.
	// std::cout << "After passing ownership: " << *myNumber << std::endl; // Potential crash

	// We don't delete 'myNumber' here because ownership was transferred.

	return 0;
}