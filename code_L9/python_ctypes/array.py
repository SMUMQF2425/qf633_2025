import os
import ctypes as ct

clib = ct.CDLL(os.getcwd() + "/simple_func.so")

c_sum_arr = clib.sum_arr
c_sum_arr.argtypes = [ct.POINTER(ct.c_int32), ct.c_int]
c_sum_arr.restype = ct.c_int

# exampe sum of int array
array = (ct.c_int * 10)()
for i in range(10):
    array[i] = i
sum = c_sum_arr(array, len(array))
print(sum)
