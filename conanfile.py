from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout
from conan.tools.files import apply_conandata_patches, copy, export_conandata_patches, get, replace_in_file, rm, rmdir
import os

class UpAbstractTransport(ConanFile):
    name = "up-abstract-transport"
    package_type = "library"
    license = "Apache-2.0 license"
    homepage = "https://github.com/eclipse-uprotocol"
    url = "https://github.com/conan-io/conan-center-index"
    description = "C++ abstract transport library"
    topics = ("ulink client", "transport")
    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    conan_version = None
    generators = "CMakeDeps"
    version = "0.0.1-dev"
    exports_sources = "CMakeLists.txt", "lib/*", "test/*"

    options = {
        "shared": [True, False],
        "fPIC": [True, False],
        "build_testing": [True, False],
        "build_unbundled": [True, False],
        "zenoh_package": [True, False],
        "build_cross_compiling": [True, False],
    }

    default_options = {
        "shared": False,
        "fPIC": False,
        "build_testing": True,
        "build_unbundled": True,
        "zenoh_package": False,
        "build_cross_compiling": False,
    }

    def requirements(self):
        self.requires("openssl/3.2.1")
        self.requires("up-core-api/1.5.8")
        self.requires("protobuf/3.21.12")
        self.requires("nlohmann_json/3.11.2")

    def generate(self):
        tc = CMakeToolchain(self)
        tc.variables["BUILD_TESTING"] = self.options.build_testing
        tc.variables["CROSS_COMPILE"] = self.options.build_cross_compiling
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["up-abstract-transport"]

