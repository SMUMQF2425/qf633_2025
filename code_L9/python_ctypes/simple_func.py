import os
import ctypes

clib = ctypes.CDLL(os.getcwd() + "/simple_func.so")

# parse int
sum_int = clib.c_sum_int
sum_int.argtypes = [ctypes.c_int, ctypes.c_int]
sum_int.restype = ctypes.c_int
print("Sum of two int numbers is :", sum_int(20, 10))

# parse double
sum_d = clib.c_sum_double
sum_d.argtypes = [ctypes.c_double, ctypes.c_double]
sum_d.restype = ctypes.c_double
print("Sum of two double numbers is :", sum_d(20, 10))

# parse char* into c
clib.display("John")
clib.display(b"John")  # change python str into byte obejct which C recon
name = "John"
clib.display(name.encode())  # change python str into string c knows, utc-8 raw string

# parse a char * into c and change value in c and return back to python
string = "Hello"
print("Before:", string)
clib.char_increment(string)
print("After: ", string)

# to fix this
cstring = ctypes.c_char_p(b"Hello")
print("Before:", cstring, cstring.value)
clib.char_increment(cstring)
print("After: ", cstring, cstring.value)
