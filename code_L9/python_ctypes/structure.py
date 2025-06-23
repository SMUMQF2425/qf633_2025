import os
import ctypes as ct

clib = ct.CDLL(os.getcwd() + "/structure.so")

class Point(ct.Structure):
    _fields_ = [("x", ct.c_int), 
                ("y", ct.c_int)]
# Create a Point Object and pass to C/C++
p1 = Point(10, 20)  
clib.printPoint(p1)
clib.printPoint2(p1)

# get structure from C/C++
clib.getPoint2.restype = ct.c_void_p
p2 = Point.from_address(clib.getPoint2())
print(p2.x, p2.y)


# free memory
clib.free_point(ct.byref(p2))


