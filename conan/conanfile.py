from conan import ConanFile
from conan.tools.cmake import CMakeDeps, CMakeToolchain, CMake


class AxonConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    requires = "gtest/1.14.0"

    def layout(self):
        self.folders.build = "build"
        self.folders.generators = "build"

    def generate(self):
        tc = CMakeToolchain(self, generator="Ninja")
        tc.generate()

        deps = CMakeDeps(self)
        deps.generate()
