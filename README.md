# flexBPE - a flexible Byte Pair Encoder based on fastBPE

[![Build Status](https://travis-ci.org/dhpollack/flexBPE.svg?branch=master)](https://travis-ci.org/dhpollack/flexBPE)  [![codecov](https://codecov.io/gh/dhpollack/flexBPE/branch/master/graph/badge.svg)](https://codecov.io/gh/dhpollack/flexBPE)

## Description

Right now this is basically just a copy of [fastBPE](https://github.com/glample/fastBPE) with cmake files for compiling and no python interface.  Eventually the idea is to make a more flexible BPE library that's still fast like fastBPE which can easily be included in other cmake projects.

## Installation

```sh
# build
mkdir build && cd build
cmake ..
make
# install
make install # optional
```

## Run Tests
```sh
mkdir build && cd build
cmake -DCODE_COVERAGE=ON -DBUILD_TEST=ON -DCMAKE_BUILD_TYPE=Debug ..
make
ctest -VV
../scripts/run_lcov.sh
gcovr -r .. --html --html-details -o coverage.html
# open coverage.html in your browser, it's in the build folder
```
