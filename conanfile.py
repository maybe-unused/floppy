import os
import re
from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout, CMakeDeps
from conan.tools.build import check_min_cppstd
from conan.tools.files import rmdir

try:
    from termcolor import colored
except ModuleNotFoundError:
    def colored(text, color=None):
        return text


def cmake_version(directory: str) -> str:
    print(colored(f'▶ searching for CMakeLists.txt in {directory}', "magenta"))
    try:
        with open(os.path.join(directory, "CMakeLists.txt"), "r") as f:
            lines = f.readlines()
            # use regex: VERSION (\d+\.)?(\d+\.)?(\*|\d+)
            for i, line in enumerate(lines):
                if re.search(r"VERSION (\d+\.)?(\d+\.)?(\*|\d+)", line) and not line.startswith("cmake_"):
                    version = re.search(r"VERSION (\d+\.)?(\d+\.)?(\*|\d+)", line).group(0)
                    version = version.replace("VERSION ", "")
                    print(colored(f"▶ found CMakeLists.txt version: {version}", "green"))
                    return version
            print(colored("▶ could not find CMakeLists.txt version", "red"))
            return None
    except FileNotFoundError:
        raise FileNotFoundError(f"CMakeLists.txt not found in {directory}")


class FloppyRecipe(ConanFile):
    name = "floppy"
    version = cmake_version(os.path.dirname(__file__))
    description = "Library that augments and extends C++ standard library"
    author = "whs31 <whs31@github.io>"
    topics = ("logging", "coreutils", "utility")

    settings = "os", "arch", "compiler", "build_type"
    options = {
        "shared": [True, False],
        "test": [True, False],
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
        self.requires("fmt/[^10.1.0]", transitive_headers=True, transitive_libs=True)
        self.requires("spdlog/1.13.0", transitive_headers=True, transitive_libs=True)
        if self.settings.os == "Windows":
            print(colored("▶ targeting windows. following libraries will be added to deps: winapi20", "green"))
            self.requires("winapi20/0.0.2")
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
        tc.cache_variables["NO_SUBMODULES"] = True       # always set to true if invoked via conan
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
        self.cpp_info.set_property("cmake_file_name", "leaf")
        self.cpp_info.set_property("cmake_target_name", "leaf::leaf")
        self.cpp_info.libs = ["leaf"]
        self.cpp_info.requires = ["fmt::fmt", "spdlog::spdlog"]
        if self.settings.os == "Windows":
            self.cpp_info.requires.append("winapi20::winapi20")
