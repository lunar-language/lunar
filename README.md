# Lunar
Lunar programming language

This repository contains source code of Lunarite and examples for Lunar

## Development Progress
- OS Support
- - [ ] Windows
- - [x] macOS
- - [x] Linux (not tested yet)
- Syntaxes
- - [x] Exit with specified error code
- - [ ] Print text
- - [ ] If...else
- - Note that more syntaxes are coming soon but they are not listed here

## Requirements for macOS
- Clang (which it can be installed with `xcode-select --install`)

## Requirements for Linux
- `as` (An assembler for Linux (which I heard it is installed on Linux systems by default))

## Installation
No install guide yet.

## Build from source
To build Lunarite from source, you need these requirements:

- As same as requirements for using it ([macOS](#requirements-for-macos), [Linux](#requirements-for-linux))
- [CMake](https://cmake.org/)
- Git

First clone it with Git:
```shell
git clone https://github.com/lunar-lang/lunar.git
```
Run this command to build it:
```shell
cmake -S . -B build/
```
Finally, you can use it:
```shell
./build/lrite <ARGUMENT>
```
