from conan import ConanFile

class CompressorRecipe(ConanFile):
    settings = ("arch", "compiler", "build_type", "os")
    generators = ("CMakeDeps", "CMakeToolchain")

    def requirements(self):

    def build_requirements(self):
        self.tool_requires("cmake/3.22.6")
