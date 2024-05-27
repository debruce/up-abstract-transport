#include "HiddenTransport.hpp"

namespace UpAbstractTransport
{
    using namespace std;

    Serializer::Serializer(Transport transport, const string &kind)
    {
        pImpl = transport.get_serializer(kind);
    }

    std::string Serializer::hello(const string &arg)
    {
        return pImpl->hello(arg);
    }
}; // namespace UpAbstractTransport