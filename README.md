Cute Edit
===

[![Build Status](https://travis-ci.org/pdigiglio/CuteEdit.svg?branch=master)](https://travis-ci.org/pdigiglio/CuteEdit)

A cross-platform Qt-based C++ text editor.

To get started, clone this repository to the `CuteEdit` folder and:
```sh
$ mkdir -p CuteEdit/build && cd CuteEdit/build
$ cmake ..
$ make -j
```

Directory structure
---

This project is structured as follows:

```sh
CuteEdit/ # main project folder
├── forms/ # .ui -- form files
├── include/ # .h -- includes
│   └── fwd/ # .h -- forward declaration
└── src/ # .cpp -- source files
```
