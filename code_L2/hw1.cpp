#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {
    // Step 1: Gather data type sizes
    string output;
    output += "The size of int: " + to_string(sizeof(int)) + " bytes\n";
    output += "The size of char: " + to_string(sizeof(char)) + " bytes\n";
    output += "The size of bool: " + to_string(sizeof(bool)) + " bytes\n";
    output += "The size of short: " + to_string(sizeof(short)) + " bytes\n";
    output += "The size of long: " + to_string(sizeof(long)) + " bytes\n";
    output += "The size of unsigned long: " + to_string(sizeof(unsigned long)) + " bytes\n";
    output += "The size of size_t: " + to_string(sizeof(size_t)) + " bytes\n";
    output += "The size of float: " + to_string(sizeof(float)) + " bytes\n";
    output += "The size of double: " + to_string(sizeof(double)) + " bytes\n";

    // Print to console
    cout << "=== Data Type Sizes ===" << endl;
    cout << output << endl;

    // Step 2: Write to file with error handling
    ofstream outFile("output.txt");
    if (!outFile.is_open()) {
        cerr << "Error: Could not open 'output.txt' for writing." << endl;
        return 1;
    }
    outFile << output;
    outFile.close();

    // Step 3: Read from file with error handling
    ifstream inFile("output.txt");
    if (!inFile.is_open()) {
        cerr << "Error: Could not open 'output.txt' for reading." << endl;
        return 1;
    }
    cout << "=== Reading from 'output.txt' ===" << endl;
    string line;
    while (getline(inFile, line)) {
        cout << line << endl;
    }
    inFile.close();

    return 0;
}