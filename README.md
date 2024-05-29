rm -rf build/*
conan install . --output-folder=build --build=missing
(cd build; cmake ..  -DCMAKE_BUILD_TYPE=Release )
(cd build; cmake --build . --config Release )
