from conan import ConanFile
from conan.tools.cmake import cmake_layout, CMake

class Recipe(ConanFile):
    name = "BlockToC"
    version = "1.0"
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"
    options = {"build_tests": [True, False]}
    default_options = {"build_tests": True}

    def layout(self):
        cmake_layout(self)

    def requirements(self):
        self.requires("pugixml/1.15")
        self.requires("cxxopts/3.3.1")
        if self.options.build_tests:
            self.requires("catch2/3.11.0")

    def build_requirements(self):
        self.tool_requires("cmake/[>=3.15]")

    def build(self):
        cmake = CMake(self)
        cmake.configure(variables={"BUILD_TESTS": "ON" if self.options.build_tests else "OFF"})
        cmake.build()