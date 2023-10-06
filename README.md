## Arithmetic expression evaluator

`kalcy` is a simple expression parsing library built using C++20. In addition to numeric literals and operators, it supports an environment with customizable functions and constants.

**Sample output**

```
./kalcy-quickstart
usage: ./kalcy-quickstart [-v] "<expression>"

./kalcy-quickstart 42
42

./kalcy-quickstart -v "1 + 2 * 3 ^ 2"
19
expression      : 1 + 2 * 3 ^ 2
AST             : (1 + (2 * (3 ^ 2)))

./kalcy-quickstart -v "-2.5 * sqrt(pi)"
-4.43113
expression      : -2.5 * sqrt(pi)
AST             : (-2.5 * sqrt(pi))
```

### Requirements

- CMake 3.24+
- C++20 compiler and standard library (+ runtime).

### CMake

Acquire the source and add it to your project's build tree via CMake `FetchContent` or `git clone` / `git submodules` + `add_subdirectory`.

Then link to `kalcy`:

```cmake
target_link_libraries(your-target PRIVATE kalcy::kalcy)
```

All required headers are included in:

```cpp
#include <kalcy/kalcy.hpp>
```

### Quickstart

Check out [examples/quickstart](examples/quickstart/quickstart.cpp).
