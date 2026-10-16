# latero

Low level driver library for the Latero device. See [OpenLatero.org](http://OpenLatero.org) for more information.

Automatically-generated documentation for the latest version on GitHub can be found [here](https://openlatero.github.io/latero/).

## Installation

### MacOS / Linux

#### Install dependencies

On MacOS, the dependencies can be installed through [MacPorts](https://www.macports.org) with the following command:

```
sudo port install cmake doxygen
```

or through [Homebrew](https://brew.sh/) with the following command:

```
brew install cmake doxygen
```

On Linux, the dependencies can be installed through:

```
sudo apt install cmake doxygen
```

#### Build and install the library

The library can be compiled and installed with the following commands:

```
cmake --preset default
cmake --build build
sudo cmake --install build
```

or in a single command:

```
cmake --preset default && cmake --build build && sudo cmake --install build
```

#### Build the documentation

Run this command to build and open the documentation:

```
cmake --build build --target doc && open docs/html/index.html
```

### Windows

On Windows, the library is built natively with the GCC toolchain of [MSYS2](https://www.msys2.org).

#### Install dependencies

Install MSYS2, then open the **MSYS2 UCRT64** shell and install the dependencies with the following command:

```
pacman -S --needed mingw-w64-ucrt-x86_64-toolchain mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-ninja mingw-w64-ucrt-x86_64-doxygen
```

Run all the commands below from the same UCRT64 shell.

#### Build and install the library

The library can be compiled and installed into the MSYS2 environment (no `sudo` needed) with the following commands:

```
cmake --preset default
cmake --build build
cmake --install build --prefix /ucrt64
```

or in a single command:

```
cmake --preset default && cmake --build build && cmake --install build --prefix /ucrt64
```

#### Build the documentation

Run this command to build the documentation, then open `docs/html/index.html` in a browser:

```
cmake --build build --target doc
```

## Examples

The [examples](examples) folder contains example programs using the library. Each one is built
separately, against the installed library; see [examples/README.md](examples/README.md).

## License

The library is released under the [MIT License](LICENSE). The code in the [examples](examples) folder is
released into the public domain under the [Unlicense](examples/LICENSE).

## Authors

OpenLatero is maintained by [Vincent Levesque](https://vlevesque.com) and his Haptic User Experience research group at [École de technologie supérieure](https://etsmtl.ca). It was originally developped as part of his PhD thesis at [McGill University](https://mcgill.ca) and prepared for release as as open source project by Jerome Pasquero (<jerome.pasquero@gmail.com>). Please see the git history for a full list of contributors.
