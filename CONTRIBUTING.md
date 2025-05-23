# Contributing Guide [WIP]

First of all, thank you for your interest in the project!

## Before you start

- [CPP Reference](https://en.cppreference.com/w/)
- [Getting started with CMake](https://cmake.org/cmake/help/latest/guide/tutorial/A%20Basic%20Starting%20Point.html)
- [Getting started with Conan](https://docs.conan.io/1/getting_started.html)
- [Project wiki page](https://github.com/kbrddestroyer/AxonEngine/wiki)
- - [Project building step-by-step guide](https://github.com/kbrddestroyer/AxonEngine/wiki/Building-with-CMake)

## Getting involved

The process of contribution requires you to modify your fork repository and opening a pull-request into central repo.
That's can be done by following this steps:

1. Fork central repository. Clone your fork repo.
2. Consider adding central repository as upstream remote. `git remote add upstream https://github.com/kbrddestroyer/AxonEngine.git`
3. `git checkout main && git pull`

Remember to keep your fork synchronized. This will minimise merge conflicts and allow you to operate the latest source 
code. You can do `git pull -u upstream main` to achieve this or synchronize fork directly on GitHub repo page.

Make some changes. Push them to your fork repository.
After the job is done, open a pull-request to the central repository. It can be done [here](https://github.com/kbrddestroyer/AxonEngine/compare).

Pull-request in `main`, `release/*`, or `feature/*` branch will trigger **status checks** to run. Currently, those include
[linux](https://github.com/kbrddestroyer/AxonEngine/actions/workflows/linux-build.yml),
[windows](https://github.com/kbrddestroyer/AxonEngine/actions/workflows/windows-build.yml) and
[macos](https://github.com/kbrddestroyer/AxonEngine/actions/workflows/macos-build.yml) builds. Both linux 
and macos builds include **gcc** and **clang** compilation checks.

Status checks will also run **CTest** on each platform and build configuration.

If your branch passes all status checks and code review, it will be merged into central repository.

# Development process

## Building from sources

> A detailed build guide is available on Wiki page.

Briefly, you'll need **Python 3**, **Conan** and **CMake** to successfully build project from the sources.
Please, refer to those tools **install guides**, our **Wiki page**, **Discussions section** or our Discord if you have 
any issues. Also consider [creating an issue](https://github.com/kbrddestroyer/AxonEngine/issues/new) if you've faced
some errors in build scripts, source code, etc.

Tests can be executed by running `ctest` command inside your build directory. 

## Codestyle

### General

- All header files are required to contain [include guard](https://en.wikipedia.org/wiki/Include_guard). 
`#pragma once` is preferred as include guard. 
- It is preferable to name C++ header files with `.hpp` extension, C headers with `.h`.
- It's better not to use `TODO` or alike comments in code. Instead, consider opening an issue. 
- Files are primarily named with PascalCase, it is allowed to use snake_case for singular headers.
- `using namespace std` is definitely not allowed.
- `#include` corresponding header in your cpp file first. 

### Making custom types

- Classes, structures, enums and namespaces are named in PascalCase, as well as containing file.
- `typedef` should use lowercase or camelCase for name, containing `_t` suffix and optionally type size in bits if 
speaking of numerics.
- Preprocessor macro should be named in CAPITAL_CASE 
- Constant fields and template parameters should be names in CAPITAL_CASE, using underscore as word separator.
- - Templates parameters can be named with  single character, e.g. `typename T`, `class U`, etc.
- Static fields should be named with PascalCase;
- Variables and functions inside a class should be named with camelCase

It is better to use class if you plan to implement methods, otherwise use structure.

An example of class containment order variant:

1. Constructors
   1. Default constructor
   2. Parameterized constructor
   3. Converting constructors
   4. Copy constructors
   5. Move constructors
2. Destructor
3. Getter and setter methods
4. Custom methods
5. Operators
6. Class fields
7. Friends declaration

```cpp
// file: MyCustomClass.hpp

namespace MyCustomNamespace {
    class MyCystomClass {
    public:
        MyCustomClass();
    };
}
```

It is better not to use `using namespace` in your code. Instead, manually specify namespace in implementation, for example:

```cpp
// file; MyCustomClass.cpp

MyCustomNamespace::MyCustomClass::MyCustomClass() {
    // Some code goes here
}

```

or 

```cpp
namespace MyCustomNamespace {
    MyCustomClass::MyCustomClass() {
        // Some code goes here
    }
}
```

> Please, never use multiple inheritance, it can lead to serious issues.

#### Getter methods

All getter methods should be marked as `const` and `noexcept` if it's possible.

> If getter can't be marked as `const` consider making separate method, that won't be associated with getter methods.

#### Memory

Dynamic memory allocation in C++ code is not welcomed, `malloc`, `calloc`, `free` are completely forbidden in C++ code
and considered insecure. If you *really need* memory allocation, use `new` and `delete`.

Instead of dynamic allocation, use smart pointers. That's more secure way.

---

If you encountered a piece of code, that's not following this codestyle, please open an issue with `refactoring` label.
