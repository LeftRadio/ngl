# What is NGL?
NGL is a small embedded graphics library writen in C for STM32.
Now support HX8352, R61581, SSD1289 controllers, work in GPIO 8 bit, GPIO 16 bit, FSMC 16 bit with different colors per point.
You can easily build graphic interfaces for all your embedded projects with minimum system requirements, small memory footprint and increase speed used hight/mid/low level interface of library.

If you need more information, please follow these links:
* [Home page](http://hobby-research.at.ua).

## Types and API
* NGL_types.h - used typedefs in the library
* NGL.h - is 'API'

## Build
For build with arm-none-eabi-gcc toolchain run 'build.py', in console:

>> python build.py

optional arguments for build.py script:

> -h, --help <> show this help message and exit

> -V, --version <> version

> -o [path], --outpath [path] <> out path for *.a library file

> -v, --verbose <> increase output verbosity [default: 'False']


*GCC arm compiler option locate in build.py:63 - build_otions(),
*change it if need another ARM Mx Core, FPU, optimization level, etc.
*You can also build NGL with CoIDE, EM::Blocks, Eclipse+ARM toolchain.

## Utilites
For simply and fast build graphics interfaces you can used QtDesigner with ngl_utils python package, more info:
* [ngl utils github](https://github.com/LeftRadio/ngl_utils).
* [ngl utils pypi page](https://pypi.python.org/pypi/ngl_utils).
