#pragma once

#include <cstdlib>
#include <memory>
#include <cxxabi.h>

template <typename T>
std::string demangler()
{
    int status = -4; // some arbitrary value to eliminate the compiler warning

    std::unique_ptr<char, void(*)(void*)> res {
        abi::__cxa_demangle(typeid(T).name(), NULL, NULL, &status),
        std::free
    };

    return (status==0) ? res.get() : typeid(T).name() ;
}
