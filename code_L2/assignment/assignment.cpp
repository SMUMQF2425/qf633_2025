#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void displayDataSizes() {
    cout << "Basic Data Type Sizes:\n";
    cout << "int: " << sizeof(int) << " bytes\n";
    cout << "float: " << sizeof(float) << " bytes\n";
    cout << "double: " << sizeof(double) << " bytes\n";
    cout << "char: " << sizeof(char) << " bytes\n";
    cout << "bool: " << sizeof(bool) << " bytes\n";
}

bool writeToFile(const string& filename) {
    ofstream fout(filename);
    if (!fout.is_open()) {
        cerr << "Error: Failed to open " << filename << " for writing\n";
        return false;
    }
    
    fout << "Basic Data Type Sizes:\n";
    fout << "int: " << sizeof(int) << " bytes\n";
    fout << "float: " << sizeof(float) << " bytes\n";
    fout << "double: " << sizeof(double) << " bytes\n";
    fout << "char: " << sizeof(char) << " bytes\n";
    fout << "bool: " << sizeof(bool) << " bytes\n";
    
    fout.close();
    return true;
}

bool readFromFile(const string& filename) {
    ifstream fin(filename);
    if (!fin.is_open()) {
        cerr << "Error: Failed to open " << filename << " for reading\n";
        return false;
    }
    
    cout << "\nFile Contents:\n";
    string line;
    while (getline(fin, line)) {
        cout << line << '\n';
    }
    
    if (fin.bad()) {
        cerr << "Error: Critical read error occurred\n";
        fin.close();
        return false;
    }
    
    if (fin.fail() && !fin.eof()) {
        cerr << "Error: Data read mismatch occurred\n";
        fin.close();
        return false;
    }
    
    fin.close();
    return true;
}

int main() {
    // Part 1: Display data type sizes
    displayDataSizes();
    
    // Part 2: File operations
    const string filename = "output.txt";
    
    // Write to file with error handling
    if (!writeToFile(filename)) {
        return 1;
    }
    
    // Read from file with error handling
    if (!readFromFile(filename)) {
        return 1;
    }
    
    return 0;
}