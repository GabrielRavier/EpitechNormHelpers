## Epitech Norm Helper

The Epitech Norm Helper is a helper for obeying the coding norms of the Epitech.

## Dependencies

* A C++17 compiler
* POSIX
* fmt
* cxxopts
* Boost.Regex

## Building

This project uses CMake, allowing it to be built on most systems. You should be able to build the program with `cmake`. You can produce files for your build system with :

```
cmake -B build -DCMAKE_BUILD_TYPE=Release
```

You can also add the following flags :

Name | Function
--------|--------
`-DLTO=ON` | Enable link-time optimisation

You can pass your own compiler flags with `-DCMAKE_C_FLAGS` and `-DCMAKE_CXX_FLAGS`.

You can then compile with this command :

```
cmake --build build --config Release
```

## Licensing

This directory contains free software. See [LICENSES/GPL-3.0](https://github.com/GabrielRavier/EpitechNormHelpers/blob/master/LICENSES/GPL-3.0) for copying permission.
