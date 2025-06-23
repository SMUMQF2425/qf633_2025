import ctypes

# Load the shared library
lib = ctypes.CDLL('./python_ctypes/libmyclass.so')

# Set return and argument types
lib.MyClass_new.argtypes = [ctypes.c_int]
lib.MyClass_new.restype = ctypes.c_void_p
lib.MyClass_get_value.argtypes = [ctypes.c_void_p]
lib.MyClass_get_value.restype = ctypes.c_int
lib.MyClass_set_value.argtypes = [ctypes.c_void_p, ctypes.c_int]
lib.MyClass_delete.argtypes = [ctypes.c_void_p]

# Python wrapper class
class MyClass:
    def __init__(self, val):
        self.obj = lib.MyClass_new(val)
    def get_value(self):
        return lib.MyClass_get_value(self.obj)
    def set_value(self, val):
        lib.MyClass_set_value(self.obj, val)
    def __del__(self):
        lib.MyClass_delete(self.obj)

# 🧪 Usage
mc = MyClass(42)
print(mc.get_value())  # 42
mc.set_value(100)
print(mc.get_value())  # 100
