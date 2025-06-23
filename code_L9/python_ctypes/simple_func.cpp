#include <iostream>
#include <stdio.h>

using namespace std;

extern "C" int c_sum_int(int arg1, int arg2)
{
    // Return the sum
    int result = arg1 + arg2;
    cout<< "sum of two int is " << result <<endl;
    return result;
}

extern "C" double c_sum_double(double arg1, double arg2)
{
    // Return the sum
    double result = arg1 + arg2;
    cout << "sum of two double is " << result << endl;
    return result;
}

extern "C" void display(char *str)
{
    printf("%s\n", str);
}

extern "C" void char_increment(char *str)
{
    for (int i = 0; str[i] != 0; i++)
    {
        str[i] = str[i] + 1;
    }
    printf("char increment is %s\n", str);
}

extern "C" int sum_arr(int *arr, int n)
{
    int sum = 0;
    for (int i = 0; i < n; i++)
    {
        sum += arr[i];
    }
    printf("sum arr is %d\n", sum);
    return sum;
}