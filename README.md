# AxonEngine

Advanced eXchange Over Network

Work in progress, there's no stable version yet

## Repository health status

[![Linux Build](https://github.com/kbrddestroyer/AxonEngine/actions/workflows/linux-build.yml/badge.svg)](https://github.com/kbrddestroyer/AxonEngine/actions/workflows/linux-build.yml)

[![Windows Build](https://github.com/kbrddestroyer/AxonEngine/actions/workflows/windows-build.yml/badge.svg)](https://github.com/kbrddestroyer/AxonEngine/actions/workflows/windows-build.yml)

[![MacOS Build](https://github.com/kbrddestroyer/AxonEngine/actions/workflows/macos-build.yml/badge.svg)](https://github.com/kbrddestroyer/AxonEngine/actions/workflows/macos-build.yml)

## Before you start

- [Discord server](https://discord.gg/FQvZhSeHrr)
- [CONTRIBUTING.md](CONTRIBUTING.md)
- [Wiki](https://github.com/kbrddestroyer/AxonEngine/wiki)

## Overview

## Requirements

### Conan

```bash
cd conan/
./install-deps.sh Debug
./install-deps.sh Release
```

## Building

1. Manual build from command prompt

```bash
mkdir cmake-build && cd cmake-build
cmake .. -DCMAKE-TOOLCHAIN-FILE=../conan/build/Debug/conan_toolchain.cmake -DBUILD_TESTS=ON -DBUILD_EXAMPLES=ON
```
