# latero

Low level driver library for the Latero device. See [OpenLatero.org](http://OpenLatero.org) for more information.

## Installation

### Linux

The following steps were tested on Ubuntu 16.04 LTS.

1. Make sure that your system is up to date:
   ```
   sudo apt update
   ```

2. Install dependencies:
   ```
   sudo apt install cmake libboost-all-dev
   ```

3. Compile and install the library:
   ```
   cmake .
   make
   sudo make install
   ```

### MacOS

Last tested in May 2026 on MacOS Tahoe 26.5 with MacPorts 2.12.5.

#### Install dependencies

The dependencies can be installed through [MacPorts](https://www.macports.org) with the following command:

```
sudo port install cmake boost
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

### Cygwin (Windows or Linux)

#### Step 1: Install Cygwin

If you already have Cygwin installed, skip to Step 2. Download Cygwin from https://www.cygwin.com/. You can see how to install it from https://cygwin.com/cygwin-ug-net/setup-net.html. After installing Cygwin, don't delete `setup-x86.exe` (or `setup-x86_64.exe`) — it will be used for Step 2.

#### Step 2: Install dependencies from the Cygwin installer

In order to build all of the latero programs, install the following packages using `setup-x86.exe` (or `setup-x86_64.exe`):

- cmake, cmake-doc, cmake-gui
- wget, tar
- doxygen
- gcc-core, gcc-g++, gcc-objc, gcc-objc++
- libMagick-devel, libMagickC++, libMagickCore, libMagickWand
- libgtkmm2.4-devel, libgtkmm2.4-doc, libgtkmm2.4_1
- libxml++2.6-devel, libxml++2.6_2, libxml2-devel, libxml2
- librsvg2-devel, librsvg2_2

#### Step 3: Install the Boost library

```
wget 'https://sourceforge.net/projects/boost/files/boost/1.60.0/boost_1_60_0.tar.gz'
tar xvzf boost_1_60_0.tar.gz
cd ./boost_1_60_0
./bootstrap.sh
./b2
cmake .
make
make install
```

#### Step 4: Install latero

```
cd {directory path to latero library}
cmake .
make
sudo make install
```

If the installation is successful, the library `liblatero.a` will now be in `/usr/local/lib/` and the include files in `/usr/local/include/latero`.

You can run the program `latero-gui` (another project) to test the library and Latero device.

## Authors

- Vincent Levesque <vincent@vlevesque.com>
- Jerome Pasquero <jerome.pasquero@gmail.com>
