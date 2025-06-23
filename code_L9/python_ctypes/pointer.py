import os
import ctypes as ct

clib = ct.CDLL(os.getcwd() + "/pointer.so")

# example 1
if True:
    c_strfree = clib.free_memory
    c_strfree.argtypes = [ct.c_char_p]
    c_strfree.restype = ct.c_void_p
    #str = ct.c_char_p(b"this ")
    #c_strfree(str)

    alloc_func = clib.alloc_memory
    alloc_func.restype = ct.POINTER(ct.c_char)
    str_ptr = alloc_func()
    str = ct.c_char_p.from_buffer(str_ptr)
    print(str.value)
    c_strfree(str_ptr)

# exampe 2
if True:
    c_strcon = clib.str_concatenate
    c_strcon.argtypes = [ct.c_char_p, ct.c_char_p]
    c_strcon.restype = ct.POINTER(ct.c_char)

    str1 = ct.c_char_p(b"this ")
    str2 = ct.c_char_p(b"a dog")
    result_ptr = c_strcon(str1, str2)
    print(result_ptr, hex(id(result_ptr)), result_ptr.contents.value)
    str = ct.c_char_p.from_buffer(result_ptr)
    print(str.value)
    c_strfree(result_ptr)


