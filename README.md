rm -rf build/*
conan install . --output-folder=build --build=missing

# This is a horrible hack.
# There must be a way to get find_package(OpenSSL) in up-abstract-transport to find openssl impl in conan cache.
tpath=`echo ~/.conan2/p/opens*/p/include/openssl`
tpath=`find ~/.conan2/p -path '*/opens*/p/include/openssl'`
export OPENSSL_ROOT_DIR=${tpath%%/include/openssl}

(cd build; cmake ..  -DCMAKE_BUILD_TYPE=Release)
(cd build; cmake --build . --config Release )
