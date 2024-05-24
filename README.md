conan install . --output-folder=build --build=missing
cmake ..  -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
