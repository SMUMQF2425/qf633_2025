#include <stdio.h>

//use below command to compile 
//g++ -shared -o testlib.so -fPIC hello_world.cpp

extern "C" void hello_world()
{
    printf("Hello World ...\n");
}

