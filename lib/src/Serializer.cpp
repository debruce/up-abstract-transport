#include "HiddenTransport.hpp"

namespace UpAbstractTransport
{
    using namespace std;

    string Serializer::messageName() const
    {
        return pImpl->messageName();
    }

    string Serializer::debugString() const
    {
        return pImpl->debugString();
    }

    string Serializer::serialize() const
    {
        return pImpl->serialize();
    }

    bool Serializer::deserialize(const std::string &arg)
    {
        return pImpl->deserialize(arg);
    }

    bool Serializer::assign(const AnyMap &arg)
    {
        return pImpl->assign(arg);
    }

    AnyMap Serializer::fetch(bool describe) const
    {
        return pImpl->fetch(describe);
    }

}; // namespace UpAbstractTransport