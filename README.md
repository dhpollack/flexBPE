# flexBPE - a flexible Byte Pair Encoder based on fastBPE

[![Build Status](https://travis-ci.org/dhpollack/flexBPE.svg?branch=master)](https://travis-ci.org/dhpollack/flexBPE)

## Description

Right now this is basically just a copy of [fastBPE](https://github.com/glample/fastBPE) with cmake files for compiling and no python interface.  Eventually the idea is to make a more flexible BPE library that's still fast like fastBPE which can easily be included in other cmake projects.

## Installation

```sh
mkdir build && cd build
cmake ..
make
make install # optional
```
