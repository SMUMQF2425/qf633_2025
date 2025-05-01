#include <iostream>

using namespace std;

// Function that takes two integers by value and returns their sum
int addByValue(int a, int b) {
    cout << "a address: " << &a << endl;
    cout << "b address: " << &b << endl;
    return a + b;
};

int addByReference(const int& a, const int& b) {
    cout << "a address: " << &a << endl;
    cout << "b address: " << &b << endl;
    return a + b;
};

void addByReference2(int& a, int& b) {
    a += 2;
    b += 3;
};

// Function that takes a constant reference to a string and prints it
void printString(const std::string& str) {
    cout << "str address: " << &str << endl;
    std::cout << "Received string: " << str << std::endl;
    // Uncommenting the line below would result in a compilation error
    // str += " (modified)"; // Error: assignment of read-only reference 'str'
}


class Student {

public:
    //default constructor
    Student(){
        cout<< "calling student default construtor"<<endl;
    }
    
    //copy constructor
    Student(const Student& rhs) {
        cout<< "calling student copy construtor"<<endl;
        age = rhs.age;
        name = rhs.name;
        GPA = rhs.GPA;
    };

    Student(std::string _name, int _age, double _gpa) {
        name= _name;
        age = _age;
        GPA = _gpa;
    };
    
    void setName(std::string _name) { name = _name;};
    void setGPA(double mark) { GPA = mark;};
    
    //inline get function
    inline const std::string getName() const { return name;};
    inline const int getAge() const { return age;};
    inline const double getGpa() const { return GPA;};

    void displayInfo() {
        std::cout << "Student Information:" << std::endl;
        std::cout << "Name: " << name << std::endl;
        std::cout << "Age: " << age << " years" << std::endl;
        std::cout << "GPA: " << GPA << std::endl;
        
    }

private:
    // Public data members
    std::string name;
    int age;
    double GPA;
};

double getStudentGPA1(Student s1, Student s2) {
    cout << "s1 address in GPA1 " << &s1 << endl;
    cout << "s2 address in GPA1 " << &s2 << endl;
    return s1.getGpa() + s2.getGpa();
}
double getStudentGPA2(Student* s1, Student* s2) {
    cout << "s1 address in GPA2 " << &s1 << endl;
    cout << "s2 address in GPA2 " << &s2 << endl;
    return s1->getGpa() + s2->getGpa();
}
double getStudentGPA3(Student& s1,  Student& s2) {
    cout << "s1 address in GPA3 " << &s1 << endl;
    cout << "s2 address in GPA3 " << &s2 << endl;
    return s1.getGpa() + s2.getGpa();
}
double getStudentGPA4(const Student& s1, const Student& s2) {
    cout << "s1 address in GPA4 " << &s1 << endl;
    cout << "s2 address in GPA4 " << &s2 << endl;
    return s1.getGpa() + s2.getGpa();
}

int main() 
{
    //example 1
    if (false) {
        int x = 5, y = 7;
        cout << "x address: " << &x << endl;
        cout << "y address: " << &y << endl;

        // Passing values to the function by copy
        int result = addByValue(x, y);
        std::cout << "Result (Pass by Value): " << result << std::endl;
        // The original values of x and y remain unchanged
        std::cout << "Original x: " << x << ", y: " << y << std::endl;

        // Passing reference to the function
        int result2 = addByReference(x, y);
        std::cout << "Result (Pass by reference): " << result << std::endl;
        // The original values of x and y remain unchanged
        std::cout << "Original x: " << x << ", y: " << y << std::endl;

        // Passing reference to the function 2
        addByReference2(x, y);
        std::cout << "Result (Pass by reference2): " << std::endl;
        // The original values of x and y remain unchanged
        std::cout << "Original x: " << x << ", y: " << y << std::endl;

        
    }
    
    //example 2, in case of object
    if(true) {
        Student s1 = Student("mike", 18, 4.0);
        Student s2 = Student("paul", 19, 4.5);
        cout << "s1 address in main: " << &s1<< endl;
        cout << "s2 address in main: " << &s2 << endl;

        Student s3(s1);
        double total;
        cout<<"calling func1"<<endl;
        total = getStudentGPA1(s1, s2);
        cout<< total << endl;
        cout<<"calling func2"<<endl;
        total = getStudentGPA2(&s1, &s2);
        cout<<"calling func3"<<endl;
        total = getStudentGPA3(s1, s2);
        cout<<"calling func4"<<endl;
        total = getStudentGPA4(s1, s2);
        cout<< "Total GPA: " << total <<endl;
    }

    //example 3 const ref exmaple.

    std::string test = "this is a string";
    cout << "test string address: " << &test << endl;
    printString(test);

    return 0;
}

    