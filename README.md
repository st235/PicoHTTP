# HttpServer implementation for Raspberry Pi Pico W

## Dependecies 

The project relies on `CMake` and `GCC` cross-compiler.
To install them follow the steps below:

```bash
brew install cmake
brew install armmbed/formulae/arm-none-eabi-gcc
```

Alternatively there is a cask for Apple Sillicon if the previous one does not work

```bash
brew install --cask gcc-arm-embedded
```

## Build

Initialise all submodules

```bash
git submodule update --init --recursive
```

Create a build dir

```bash
mkdir out
cd out
```

Build the project

```
cmake ..
make
```
