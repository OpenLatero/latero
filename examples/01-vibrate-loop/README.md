# 01-VIBRATE-LOOP

This example shows the use of the Latero library to make the pins of the Latero tactile display vibrate. The pins vibrate at 1 Hz with adjacent rows having a different phase.

## MacOS / Linux

#### Install OpenLatero libraries

Install the [latero](https://github.com/OpenLatero/latero) library.

#### Build and run the app

The app can be compiled with the following commands:

```
cmake -B build
cmake --build build
```

and run with this command:

```
build/latero-example-vibrate-loop
```

or in a single command:

```
cmake -B build && cmake --build build && build/latero-example-vibrate-loop
```

## Authors

OpenLatero is maintained by [Vincent Levesque](https://vlevesque.com) and his Haptic User Experience research group at [École de technologie supérieure](https://etsmtl.ca). It was originally developed as part of his PhD thesis at [McGill University](https://mcgill.ca) and prepared for release as an open source project by Jerome Pasquero (<jerome.pasquero@gmail.com>). Please see the git history for a full list of contributors.
