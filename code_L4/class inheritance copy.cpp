#include <iostream>

using namespace std;


class Bank {

public:
    void withdrawAtm() {
        cout << "Animal is eating" << endl;
    }

const std::string name;

};

class branch : public Bank {

private:
    void checkbalance() {
        cout << "Dog is barking" << endl;
    }
};

int main() {
    Dog myDog;
    myDog.eat();   // Inherited from Animal class
    myDog.bark();

    return 0;
}