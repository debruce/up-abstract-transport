#include "HiddenTransport.hpp"

namespace UpAbstractTransport
{
    using namespace std;

    std::string Serializer::hello(const string &arg)
    {
        return pImpl->hello(arg);
    }
}; // namespace UpAbstractTransport