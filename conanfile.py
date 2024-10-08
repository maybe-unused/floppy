import os
from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout, CMakeDeps
from conan.tools.build import check_min_cppstd
from conan.tools.files import rmdir
try:
    from termcolor import colored
except ImportError:
    def colored(text, color, on_color=None, attrs=None):
        return text


class FloppyRecipe(ConanFile):
    name = "floppy"
    version = "1.4.0"
    description = "Library that augments and extends C++ standard library"
    author = "whs31 <whs31@github.io>"
    topics = ("logging", "coreutils", "utility")

    settings = "os", "arch", "compiler", "build_type"
    options = {
        "shared": [True, False],
        "test": [True, False]
    }
    default_options = {
        "shared": True,
        "test": False
    }

    exports_sources = "*"

    @property
    def _min_cppstd(self):
        return "20" 

    def requirements(self):
        self.requires("spdlog/1.13.0", transitive_headers=True, transitive_libs=True)
        if self.settings.os != "Windows":
            self.requires("elfutils/0.190", transitive_headers=True, transitive_libs=True)
        if self.options.test:
            self.requires("gtest/1.14.0")
            self.requires("tomlplusplus/[^3.0.0]", transitive_headers=True, transitive_libs=True)

    def layout(self):
        cmake_layout(self)

    def validate(self):
        if self.settings.get_safe("compiler.cppstd"):
            check_min_cppstd(self, self._min_cppstd)

    def configure(self):
        self.options["spdlog/*"].shared = True
        self.options["fmt/*"].shared = True

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.cache_variables["BUILD_SHARED_LIBS"] = self.options.shared
        tc.cache_variables["TESTS"] = self.options.test
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()
        rmdir(self, os.path.join(self.package_folder, "lib", "cmake"))
        rmdir(self, os.path.join(self.package_folder, "lib", "pkgconfig"))
        rmdir(self, os.path.join(self.package_folder, "res"))
        rmdir(self, os.path.join(self.package_folder, "share"))

    def package_info(self):
        self.cpp_info.set_property("cmake_file_name", "floppy")
        self.cpp_info.set_property("cmake_target_name", "floppy::floppy")
        self.cpp_info.libs = ["floppy"]
        self.cpp_info.requires = ["spdlog::spdlog"]
        self.cpp_info.defines.append(f"FLOPPY_TARGET_NAME={self.name}")
        self.cpp_info.defines.append(f"FLOPPY_PROJECT_VERSION_MAJOR={self.version.split('.')[0]}")
        self.cpp_info.defines.append(f"FLOPPY_PROJECT_VERSION_MINOR={self.version.split('.')[1]}")
        self.cpp_info.defines.append(f"FLOPPY_PROJECT_VERSION_PATCH={self.version.split('.')[2]}")
        if self.options.test:
            print(colored("▶ testing enabled. following libraries will be added to deps: gtest, tomlplusplus", "green"))
            self.cpp_info.requires.append("gtest::gtest")
            self.cpp_info.requires.append("tomlplusplus::tomlplusplus")
        if self.settings.os == "Windows":
            self.cpp_info.system_libs.extend(["psapi", "dbghelp"])
        if self.settings.os in ["Linux", "FreeBSD", "Android"]:
            self.cpp_info.system_libs.extend(["dl", "m"])
            self.cpp_info.requires.append("elfutils::elfutils")
