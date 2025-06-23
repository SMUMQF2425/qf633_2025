import ctypes as ct

# value_1 variable stores an integer of type Ctypes (not a regular integer)
int_value = ct.c_int(10)
# printing the type and what int_value holds
print(int_value, int_value.value)

# storing a ctypes long value
long_value = ct.c_long(10)
print(long_value, long_value.value)

# creating a ctypes float variable
float_value = ct.c_float(15.25)
print(float_value, float_value.value)

# creating a ctypes double variable
double_value = ct.c_double(85.69845)
print(double_value, double_value.value)

# creating a ctypes char variable
str_value = ct.c_char(b'c')
print(str_value, str_value.value)

# creating a ctypes wchar variable
str_value1 = ct.c_wchar('c')
print(str_value1, str_value1.value)

# creating a ctypes char * variable
str_value2 = ct.c_char_p(b"cat")
print(str_value2, str_value2.value)

# creating a ctypes wchar * variable
str_value3 = ct.c_wchar_p("test")
print(str_value3, str_value3.value)

# using pointer() method we are pointing to the value_1 variable and storing it in ptr
value_1 = ct.c_int(20)
ptr = ct.pointer(value_1)
print("Contents of value_1 : ", value_1)
print("Real value stored in value_1 : ", value_1.value)
print("Address of value_1 : ", hex(id(value_1.value)))

# If we want to print the contents of a pointer type variable then need to use .contents
# otherwise only writing the variable is enough like above
print("Contents of ptr variable : ", ptr.contents)

# To print the value stored in the address pointed by that pointer variable
# we need to use .value after .contents
print("The value at which ptr points at : ", ptr.contents.value)

# Printing the address of the value to which the ptr variable points at
print(
    "Address of that value which is pointed and stored in ptr : ",
    id(ptr.contents.value),
)

value_2 = ct.c_long(10)
ptr = ct.pointer(value_2)
print("pointer address: ", id(ptr))
# Printing to see what the void pointer currently holds
print("address before Changing : ", hex(id(value_2)))
print("Value before Changing : ", value_2.value)

# storing a value in ptr variable
ptr.contents.value = 25

# If we print the value_1 variable it will
# now store the updated value
print("address after Changing : ", hex(id(value_2)))
print("Value after Changing : ", value_2.value)
