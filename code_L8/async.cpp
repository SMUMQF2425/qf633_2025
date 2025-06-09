// Example of checking the number is even or not using async
#include <iostream>
#include <future>
#include <vector>

// function for checking the number is even or not
bool check_even (int num) {
    std::cout << "Hello I am inside the function!! \n";
    //checking the divisibility of number by 2 and returning a bool value
    if (num %2 == 0)
        return true;
    else
        return false;
}

int main () {
    
    //case of synchronous program
    if (true) {
        std::vector<int> number_list({1,2,3,4,5});
        for (auto & num: number_list) {
            std::cout << "Checking whether the number:" << num << " is even or not" <<std::endl;
            bool rs = check_even(num);
            if (rs) {
                std::cout << "Number mentioned by you is even \n";
            }
            else {
                std::cout << "Sorry the number is odd \n";
            }
        }
    }
    
    //example 2, demo using std::async
    if (true){
        std::vector<int> number_list({1,2,3,4,5});
        // calling the above function check_even asynchronously and storing the result in future object
        for (auto & num: number_list) {
            std::cout << "Checking whether the number:" << num << " is even or not" <<std::endl;
            std::future<bool> ft = std::async(check_even, num);
            // std::future<bool> ft = std::async(std::launch::async, check_even, num);
            // retrieving the exact value from future object and waiting for check_even to return
            bool rs = ft.get();
            if (rs) {
                std::cout << "Number mentioned by you is even \n";
            }
            else {
                std::cout << "Sorry the number is odd \n";
            }
        }
    }
    
    // example 3, turn it into a real mult-threading async program
    if (true) {
        std::vector<int> number_list({1,2,3,4,5});
        std::vector<std::future<bool>> _futures;
        // calling the above function check_even asynchronously and storing the result in future object
        for (auto & num: number_list) {
            std::cout << "Checking whether the number:" << num << " is even or not" <<std::endl;
            //you can play around of using differet launch policy
            _futures.push_back(std::async(std::launch::deferred, check_even, num)); 
        }
            
        for (auto && fut: _futures) {
            bool rs = fut.get();
            if (rs) 
                std::cout << "Number mentioned by you is even \n";
            else 
                std::cout << "Sorry the number is odd \n";
        }        
        
    }

    return 0;
}