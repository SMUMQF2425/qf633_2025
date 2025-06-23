from ctypes import *

# this example can be run in windows only
if False:
    libc = cdll.msvcrt
    print(libc.printf)
    printf = libc.printf
    printf(b"Hello, %s\n", b"World!")
    printf(b"Hello, %S\n", "World!")
    printf(b"%d bottles of beer\n", 42)
    printf(b"%f bottles of beer\n", c_double(42.5))

p = create_string_buffer(3)  # create a 3 byte buffer, initialized to NUL bytes
print(sizeof(p), repr(p.raw))

p = create_string_buffer(b"Hello")  # create a buffer containing a NUL terminated string
print(sizeof(p), repr(p.raw))

print(repr(p.value))

p = create_string_buffer(b"Hello", 10)  # create a 10 byte buffer
print(sizeof(p), repr(p.raw))

p.value = b"Hi"
print(sizeof(p), repr(p.raw))

if False: #windows only
    strchr = libc.strchr
    strchr.restype = c_char_p
    strchr.argtypes = [c_char_p, c_char]
    strchr(b"abcdef", b"d")
    # strchr(b"abcdef", b"def")
    print(strchr(b"abcdef", b"x"))
    ptr = strchr(b"abcdef", b"d")
    print(ptr)
