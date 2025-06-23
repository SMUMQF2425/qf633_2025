from ctypes import cdll, c_char_p

# this example can be run in windows only

if False:
    libc = cdll.msvcrt
    print(libc.printf)
    printf = libc.printf
    printf(b"Hello, %s\n", b"World!")
    printf(b"%d bottles of beer\n", 42)


if True:
    # Load the standard C library on macOS
    libc = cdll.LoadLibrary('libc.dylib')

    # Example: Call the printf function
    message = b"Hello from macOS C library!"
    libc.printf(c_char_p(message))
