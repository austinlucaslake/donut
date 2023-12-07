from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps, cmake_layout

required_conan_version = ">=2.0"

class Astrosight(ConanFile):
    name = "quaternion"
    version = "1.1.2"
    license = "Apache-2.0"
    author = "Austin Lake (53884490+austinlucaslake@users.noreply.github.com)"
    url = "https://github.com/austinlucaslake/donut"
    description = "Quaternion rotations rendered using ASCII characters."
    topics = ("quaternion", "render", "ASCII")
    settings = "arch", "compiler", "build_type", "os"
    exports_sources = "CMakeLists.txt", "src/*", "include/*"

    def build_requirements(self):
        self.tool_requires("cmake/[>3.23.5]")
        self.test_requires("cppcheck/2.12.1")
        self.test_requires("uncrustify/0.78.0")

    def generate(self):
        toolchain = CMakeToolchain(self)
        toolchain.generate()
        dependancies = CMakeDeps(self)
        dependancies.check_components_exist = True
        dependancies.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        # cmake.test()

    def layout(self):
        cmake_layout(self, src_folder="src", build_folder="build")
