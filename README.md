# Boost.Nowide

Branch      | Travis | Appveyor | Github | Documentation
------------|--------|----------|--------|--------------
[master](https://github.com/boostorg/nowide/tree/master)   | [![Build Status](https://travis-ci.com/boostorg/nowide.svg?branch=master)](https://travis-ci.com/boostorg/nowide)  | [![Build status](https://ci.appveyor.com/api/projects/status/w5sywrekwd66say4/branch/master?svg=true)](https://ci.appveyor.com/project/Flamefire/nowide-fr98b/branch/master)   | ![](https://github.com/boostorg/nowide/workflows/CI%20Tests/badge.svg?branch=master)  | [![Documentation](https://img.shields.io/badge/documentation-master-brightgreen.svg)](https://www.boost.org/doc/libs/master/libs/nowide/index.html)
[develop](https://github.com/boostorg/nowide/tree/develop) | [![Build Status](https://travis-ci.com/boostorg/nowide.svg?branch=develop)](https://travis-ci.com/boostorg/nowide) | [![Build status](https://ci.appveyor.com/api/projects/status/w5sywrekwd66say4/branch/develop?svg=true)](https://ci.appveyor.com/project/Flamefire/nowide-fr98b/branch/develop) | ![](https://github.com/boostorg/nowide/workflows/CI%20Tests/badge.svg?branch=develop) | [![Documentation](https://img.shields.io/badge/documentation-develop-brightgreen.svg)](https://www.boost.org/doc/libs/develop/libs/nowide/index.html)



Library for cross-platform, unicode aware programming.

The library provides an implementation of standard C and C++ library functions, such that their inputs are UTF-8 aware on Windows without requiring to use Wide API.

# Quickstart

Instead of using the standard library functions use the corresponding member of Boost.Nowide with the same name.
On Linux those are (mostly) aliases for the `std` ones, but on Windows they accept UTF-8 as input and use the wide API for the underlying functionality.

Examples:
- `std::ifstream -> boost::nowide::ifstream`
- `std::fopen -> boost::nowide::fopen`
- `std::fclose -> boost::nowide::fclose`
- `std::getenv -> boost::nowide::getenv`
- `std::putenv -> boost::nowide::putenv`
- `std::cout -> boost::nowide::cout`

To also convert your input arguments to UTF-8 on Windows use:

```
int main(int argc, char **argv)
{
    boost::nowide::args _(argc, argv); // Must use an instance!
    ...
}
```

See the [Documentation](https://www.boost.org/doc/libs/master/libs/nowide/index.html) for details.

# Compile

## With Boost

Compile and install the Boost super project the usual way via `./b2`.
The headers and library will then be available together with all other Boost libraries.
From within CMake you can then use `find_package(boost_nowide)` and link against `Boost::nowide`.

## With CMake

Boost.Nowide fully supports CMake.
So you can use `add_subdirectory("path-to-boost-nowide-repo")` and link your project against the target `Boost::nowide`.

You can also pre-compile and install Boost.Nowide via the usual workflow:
```
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local
make install
```

A CMake-Config file will be installed alongside Boost.Nowide so `find_package(boost_nowide)` does work out-of the box
(provided it was installed into a "standard" location or its `INSTALL_PREFIX` was added to `CMAKE_PREFIX_PATH`).

# Boost.Filesystem integration

Boost.Nowide integrates with Boost.Filesystem:
- Call `boost::nowide::nowide_filesystem()` to imbue UTF-8 into Boost.Filesystem (for use by `boost::filesystem::path`) such that narrow strings passed into Boost.Filesystem are treated as UTF-8 on Windows
