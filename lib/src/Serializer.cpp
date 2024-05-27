#include "HiddenTransport.hpp"

namespace UpAbstractTransport
{
    using namespace std;

    using EnumMap = map<int, string>;
    using OneofMap = map<string, string>;

    string as_string(const any &a)
    {
        if (a.type() == typeid(double))
            return to_string(any_cast<double>(a));
        if (a.type() == typeid(float))
            return to_string(any_cast<float>(a));
        if (a.type() == typeid(bool))
            return to_string(any_cast<bool>(a));
        if (a.type() == typeid(int8_t))
            return to_string(any_cast<int8_t>(a));
        if (a.type() == typeid(int16_t))
            return to_string(any_cast<int16_t>(a));
        if (a.type() == typeid(int32_t))
            return to_string(any_cast<int32_t>(a));
        if (a.type() == typeid(int64_t))
            return to_string(any_cast<int64_t>(a));
        if (a.type() == typeid(uint8_t))
            return to_string(any_cast<uint8_t>(a));
        if (a.type() == typeid(uint16_t))
            return to_string(any_cast<uint16_t>(a));
        if (a.type() == typeid(uint32_t))
            return to_string(any_cast<uint32_t>(a));
        if (a.type() == typeid(uint64_t))
            return to_string(any_cast<uint64_t>(a));
        if (a.type() == typeid(const char *))
            return string("\"") + string(any_cast<const char *>(a)) + '"';
        if (a.type() == typeid(string))
            return string("\"") + any_cast<string>(a) + '"';
        if (a.type() == typeid(pair<int, string>))
        {
            auto p = any_cast<pair<int, string>>(a);
            stringstream ss;
            ss << "Enum(" << p.first << '=' << p.second << ')';
            return ss.str();
        }
        if (a.type() == typeid(EnumMap))
        {
            auto em = any_cast<EnumMap>(a);
            string s;
            if (em.size() > 0)
            {
                stringstream ss;
                ss << "EnumMap(";
                for (const auto &[k, v] : em)
                {
                    ss << k << '=' << v << ", ";
                }
                s = ss.str();
                s = s.substr(0, s.size() - 2);
                s += ')';
            }
            else
            {
                s = "EnumMap()";
            }
            return s;
        }
        if (a.type() == typeid(OneofMap))
        {
            auto om = any_cast<OneofMap>(a);
            string s;
            if (om.size() > 0)
            {
                stringstream ss;
                ss << "Oneof(";
                for (const auto &[k, v] : om)
                {
                    ss << k << '=' << v << ", ";
                }
                s = ss.str();
                s = s.substr(0, s.size() - 2);
                s += ')';
            }
            else
            {
                s = "Oneof()";
            }
            return s;
        }
        return string("[[ ") + a.type().name() + " ]]";
    }

    string padX(const string &pad, size_t x)
    {
        string ret;
        while (x > 0)
        {
            ret += pad;
            x--;
        }
        return ret;
    }

    void anymapFormat(ostream &os, const AnyMap &m, const string &pad, size_t depth, bool with_type)
    {
        if (depth == 0)
            os << padX(pad, depth) << "{\n";
        for (const auto &[k, v] : m)
        {
            if (v.type() == typeid(AnyMap))
            {
                os << padX(pad, depth + 1) << k << " = " << "{\n";
                anymapFormat(os, any_cast<AnyMap>(v), pad, depth + 2);
                os << padX(pad, depth + 1) << "}\n";
            }
            else
            {
                os << padX(pad, depth + 1) << k << " = " << as_string(v);
                if (with_type)
                {
                    os << ", " << v.type().name();
                }
                os << '\n';
            }
        }
        if (depth == 0)
            os << padX(pad, depth) << "}\n";
    }

    string Serializer::messageName()
    {
        return pImpl->messageName();
    }

    string Serializer::debugString()
    {
        return pImpl->debugString();
    }

    string Serializer::serialize()
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

    AnyMap Serializer::fetch(bool describe)
    {
        return pImpl->fetch(describe);
    }

}; // namespace UpAbstractTransport