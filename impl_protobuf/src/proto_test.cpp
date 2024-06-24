#include <iostream>
#include <sstream>
#include <functional>
#include <optional>
#include <algorithm>
#include <iterator>
#include <vector>
#include <variant>
#include <tuple>
#include <string_view>
#include "uprotocol/v1/umessage.pb.h"
#include "uprotocol/v1/uuid.pb.h"

struct StrmEnd {};

template <typename T>
using Expected = std::variant<StrmEnd, T>;

template <typename T>
bool isBad(T&& t) { return std::holds_alternative<StrmEnd>(t); }

template <typename T>
auto getGood(T&& t) { return std::get<1>(t); }

std::ostream& operator<<(std::ostream& os, const std::vector<uint8_t>& data)
{
    os << "[ ";
    for (auto x : data) {
        os << int(x) << ' ';
    }
    os << ']';
    return os;
}

template <typename T>
struct Tagged_t {
    T  data;

    Tagged_t(T&& t) : data(t) {}
    operator T& () { return data; }
};


class VarIntUnpackStream {
public:
    explicit VarIntUnpackStream(const uint8_t* start, size_t len)
    {
        initial = current = start;
        end = current + len;
    }

    explicit VarIntUnpackStream(const std::string& s) : VarIntUnpackStream((uint8_t*)(s.data()), s.size())
    {
    }

    explicit VarIntUnpackStream(const std::vector<uint8_t>& s) : VarIntUnpackStream(s.data(), s.size())
    {
    }

    Expected<uint64_t> operator()()
    {
        uint64_t accum = 0;
        size_t shift = 0;
        while (current < end) {
            auto value = *current;
            ++current;
            accum |= uint64_t(value & 0x7f) << shift;
            shift += 7;
            if (!(value & 0x80)) return accum;
        }
        return StrmEnd{};
    }

    Expected<std::vector<uint8_t>> operator()(size_t len)
    {
        using namespace std;
        if (current + len > end) return StrmEnd{};
        auto ret = std::vector<uint8_t>(current, current + len);
        current += len;
        return ret;
    }

    size_t offset() const
    {
        return current - initial;
    }

private:
    const uint8_t* initial;
    const uint8_t* current;
    const uint8_t* end;
};

template <typename T>
T packInto(const std::vector<uint8_t>& data)
{
    auto len = sizeof(T);
    T ret;
    for (auto i = 0; i < len; i++) {
        ret <<= 8;
        ret |= data[i];
    }
    return ret;
}

using VarInt_t = Tagged_t<size_t>;

using FieldVar = std::variant<VarInt_t, uint32_t, uint64_t, std::vector<uint8_t>>;

std::ostream& operator<<(std::ostream& os, const FieldVar& data)
{
    using namespace std;
    if (holds_alternative<VarInt_t>(data)) {
        size_t s = get<VarInt_t>(data).data;
        os << "size_t(" << s << ')';
    }
    else if (holds_alternative<uint32_t>(data))
        os << "uint32_t(" << get<uint32_t>(data) << ')';
    else if (holds_alternative<uint64_t>(data))
        os << "uint64_t(" << get<uint64_t>(data) << ')';
    else if (holds_alternative<vector<uint8_t>>(data)) {
        os << "vector<uint8_t>(" << get<vector<uint8_t>>(data) << ')';
    }
    return os;
}

Expected<std::tuple<size_t, FieldVar>>
    takeField(VarIntUnpackStream& it)
{
    using namespace std;
    auto vtag = it();
    if (isBad(vtag)) return StrmEnd{};
    auto tag = getGood(vtag);
    size_t fieldNumber = tag >> 3;
    tag &= 7;
    switch (tag) {
        case 0: {
            auto value = it();
            if (isBad(value)) return StrmEnd{};
            return make_tuple(fieldNumber, VarInt_t(getGood(value)));
        }
        case 1: {
            auto value = it(sizeof(uint64_t));
            if (isBad(value)) return StrmEnd{};
            return make_tuple(fieldNumber, packInto<uint64_t>(getGood(value)));
        }
        case 5: {
            auto value = it(sizeof(uint32_t));
            if (isBad(value)) return StrmEnd{};
            return make_tuple(fieldNumber, packInto<uint32_t>(getGood(value)));
        }
        case 2: {
            auto len = it();
            if (isBad(len)) return StrmEnd{};
            auto value = it(getGood(len));
            if (isBad(value)) return StrmEnd{};
            return make_tuple(fieldNumber, getGood(value));
        }
        default:
            cout << "tag incorrect" << endl;
            return StrmEnd{};
    }
}

int main(int argc, char *argv[])
{
    using namespace std;

    auto id = new uprotocol::v1::UUID();
    id->set_msb(2);
    id->set_lsb(3);
    auto attr = new uprotocol::v1::UAttributes();
    attr->set_token("hello_token");
    attr->set_traceparent("hello_traceparent");
    attr->set_allocated_id(id);

    auto msg = new uprotocol::v1::UMessage();
    msg->set_allocated_attributes(attr);
    msg->set_payload("hello_payload");

    cout << msg->DebugString() << endl;
    auto x = msg->SerializeAsString();
    // for (auto i = 0; i < x.size(); i++) cout << dec << i << ": " << hex << int(x[i]) << endl;

    auto it = VarIntUnpackStream(x);

    auto attrPair = takeField(it);
    if (isBad(attrPair)) {
        cout << "cannot get first field" << endl;
        exit(-1);
    }
    auto [attrField, attrData] = getGood(attrPair);
    auto attrDeser = new uprotocol::v1::UAttributes();
    auto realData = get<vector<uint8_t>>(attrData);
    attrDeser->ParseFromArray(realData.data(), realData.size());
    cout << "first field=" << attrField << endl;
    cout << attrDeser->DebugString();

    auto attrPair2 = takeField(it);
    if (isBad(attrPair2)) {
        cout << "cannot get second field" << endl;
        exit(-1);
    }
    auto [attrField2, attrData2] = getGood(attrPair2);
    auto realPayload = get<vector<uint8_t>>(attrData2);
    cout << "second field=" << attrField << ' ' << string_view((const char*)(realPayload.data()), realPayload.size()) << endl;
}
