Embedded graphics library writen in C for STM32.
Now support HX8352, R61581, SSD1289 controllers, work in GPIO 8 bit, GPIO 16 bit, FSMC 16 bit with different colors per point.
-- NGL_types.h is all used typedefs in the library
-- NGL.h is 'API'


For build with arm-none-eabi-gcc toolchain run 'build.py', in console:

>> python build.py

optional arguments for build.py script:

>> -h, --help <> show this help message and exit

>> -V, --version <> version

>> -o [path], --outpath [path] <> out path for *.a library file

>> -v, --verbose <> increase output verbosity [default: 'False']


GCC arm compiler option locate in build.py:63 - build_otions(),

change it if need another ARM Mx Core, FPU, optimization level, etc.
