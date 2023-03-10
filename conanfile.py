from conan.tools.cmake import CMakeToolchain

from conan import ConanFile


class Recipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps", "VirtualRunEnv"

    def layout(self):
        self.folders.generators = "conan"

    def requirements(self):
        self.requires("openal/1.22.2")
        self.requires("ffmpeg/5.0")

    def configure(self):
        # https://www.ffmpeg.org/legal.html
        self.options["ffmpeg"].shared = False
        self.options["ffmpeg"].with_libx264 = False
        self.options["ffmpeg"].with_libx265 = False
        self.options["ffmpeg"].postproc = False
        self.options["ffmpeg"].with_libfdk_aac = False
        self.options["ffmpeg"].with_openjpeg = False
