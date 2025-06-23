from ctypes import *

# we change the value of p_s object, and we noticed the address changed also, this is due to python object is immutable
s = "Hello, World"
print("before change: ", id(s), s)
print("before change: ", hex(id(s)), s)
s = "Hello, World Changed!"
print("after change: ", id(s), s)
print("after change: ", hex(id(s)), s)

p_s = c_wchar_p(s)
print("before change: ", p_s, p_s.value)
p_s.value = "Hi, there"
print("after change: ", p_s, p_s.value)

# to achieve mutable behaviour we need to use below function. 
# This is quite essential when we like to receive C/C++ function return result

# create a 3 byte buffer, initialized to NUL bytes
p = create_string_buffer(3)            
print(sizeof(p), repr(p.raw), repr(p.value))

# create a buffer containing a NUL terminated string
p = create_string_buffer(b"Hello")     
print(sizeof(p), repr(p.raw), repr(p.value))

p = create_string_buffer(b"Hello", 10) # create a 10 byte buffer
print(hex(id(p)), repr(p.raw), repr(p.value))
p.value = b"Hi"
print(hex(id(p)), repr(p.raw),repr(p.value))