from conan import ConanFile
from conan.tools.cmake import cmake_layout, CMake

class Recipe(ConanFile):
    name = "BlockToC"
    version = "1.0"
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    def layout(self):
        cmake_layout(self)

    def requirements(self):
        self.requires("pugixml/1.15")
        self.requires("cxxopts/3.3.1")

    def build_requirements(self):
        self.tool_requires("cmake/[>=3.15]")

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()