## Contributing Guide \[WIP]

First of all, thank you for your interest in the project!

## Before You Start

First things first: if you're not sure how to format your code correctly, just do your best and mention in the pull request that you're open to suggestions. We're here to help.
Our primary goal and guiding philosophy is to build a friendly community of developers, support each other, and grow together as we improve our coding skills.

Think of this guide as a reference, not a rulebook - it's here to help, not to limit you.

* [C++ Reference](https://en.cppreference.com/w/)
* [Getting Started with CMake](https://cmake.org/cmake/help/latest/guide/tutorial/A%20Basic%20Starting%20Point.html)
* [Getting Started with Conan](https://docs.conan.io/1/getting_started.html)
* [Project Wiki Page](https://github.com/kbrddestroyer/AxonEngine/wiki)
* [Step-by-Step Build Guide](https://github.com/kbrddestroyer/AxonEngine/wiki/Building-with-CMake)

## Getting Involved

To contribute, fork the central repository and submit a pull request from your fork. Follow these steps:

1. Fork the central repository and clone your fork.
2. Add the central repository as an upstream remote:

   ```bash
   git remote add upstream https://github.com/kbrddestroyer/AxonEngine.git
   ```
3. Pull the latest changes from upstream:

   ```bash
   git checkout main
   git pull upstream main
   ```

   Remember to keep your fork synchronized with the central repository. This minimizes merge conflicts and ensures you're working with the latest code.
4. Make your changes and push them to your fork.
5. Open a pull request to the central repository [here](https://github.com/kbrddestroyer/AxonEngine/compare).

Pull requests to `main`, `release/*`, or `feature/*` branches trigger **status checks**:

* [Linux Build](https://github.com/kbrddestroyer/AxonEngine/actions/workflows/linux-build.yml)
* [Windows Build](https://github.com/kbrddestroyer/AxonEngine/actions/workflows/windows-build.yml)
* [macOS Build](https://github.com/kbrddestroyer/AxonEngine/actions/workflows/macos-build.yml)

Status checks include **GCC** and **Clang** builds (Linux/macOS) and **CTest** on all platforms. If your branch passes checks and review, it will be merged.

## Development Process

### Building from Sources

> A detailed build guide is available on the Wiki page.

You will need **Python 3**, **Conan**, and **CMake** to build the project.
Refer to installation guides, the Wiki, Discussions section, or Discord if you encounter issues. [Create an issue](https://github.com/kbrddestroyer/AxonEngine/issues/new) if you find errors in code or scripts.

Run tests with:

```bash
ctest
```

inside your build directory.

## Codestyle

### General Guidelines

* Use `#pragma once` for include guards.
* Use `.hpp` for C++ headers, `.h` for C headers.
* Avoid comments like `TODO` in code; instead, open an issue.
* Use PascalCase for filenames, snake\_case allowed for singular headers.
* Do not use `using namespace std;`
* Always include the corresponding header first in `.cpp` files.

### Custom Types

* Use PascalCase for class, struct, enum, and namespace names.
* `typedef` names: camelCase or snake\_case, end with `_t`, optionally include bit-size.
* Macro names: UPPER\_SNAKE\_CASE
* Constants and template parameters: UPPER\_SNAKE\_CASE
* Template parameters may be single letters (e.g. `T`, `U`).
* Static class fields: PascalCase
* Member variables and methods: camelCase

> Prefer `class` if methods are needed; otherwise, use `struct`.

#### Recommended Class Layout

1. Constructors
   * Default
   * Parameterized
   * Conversion
   * Copy
   * Move
2. Destructor
3. Getters and Setters
4. Custom Methods
5. Operators
6. Fields
7. Friend Declarations

```cpp
// MyCustomClass.hpp
namespace MyNamespace {
    class MyCustomClass {
    public:
        MyCustomClass();
    };
}
```

```cpp
// MyCustomClass.cpp
MyNamespace::MyCustomClass::MyCustomClass() {
    // Implementation
}
```

Or:

```cpp
namespace MyNamespace {
    MyCustomClass::MyCustomClass() {
        // Implementation
    }
}
```

### Forbidden Practices

* Never use `using namespace std;`
* Avoid multiple inheritance
* Do not use `malloc`, `calloc`, `free` in C++
* Prefer smart pointers over `new` and `delete`

### Getter Methods

* Must be marked `const` and `noexcept` when possible
* If `const` is not possible, consider using a different name, so that your method won't be associated with getters.

### Memory

As said above, avoid raw memory allocation in C++ code. Use smart pointers instead. 
Functions like `malloc`, `calloc`, and `free` are forbidden in C++ code.

Speaking of class layout, [*the rule of zero*](https://en.cppreference.com/w/cpp/language/rule_of_three.html) is preferred 

### Code documenting

Comments in code are preferred, but not always necessary. There are some basic rules of proper code documentation, 
which can be found [here](https://developer.lsst.io/cpp/api-docs.html).

---

If you find code that doesn’t follow these guidelines, [open an issue](https://github.com/kbrddestroyer/AxonEngine/issues/new) with the `refactoring` label.
