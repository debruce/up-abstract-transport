#include <iostream>
#include <sstream>
#include <vector>
#include <tuple>
#include <string_view>
#include <type_traits>
#include <utility>
#include "Expected.h"
#include "uprotocol/v1/umessage.pb.h"
#include "uprotocol/v1/uuid.pb.h"


std::ostream& operator<<(std::ostream& os, const std::vector<uint8_t>& data)
{
    os << "[ ";
    for (auto x : data) {
        os << int(x) << ' ';
    }
    os << ']';
    return os;
}

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
        return ErrorTag{};
    }

    Expected<std::vector<uint8_t>> operator()(size_t len)
    {
        using namespace std;
        if (current + len > end) return ErrorTag{};
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


class VarIntPackStream {
public:
    explicit VarIntPackStream(size_t reserved_len = 4096)
    {
        buffer.reserve(reserved_len);
    }

    void operator()(uint64_t data)
    {
        using namespace std;
        while (data > 127) {
            auto x = uint8_t((data & 0x7f) | 0x80);
            buffer.push_back(x);
            data >>= 7;
        }
        buffer.push_back(uint8_t(data));
    }

    void operator()(const uint8_t* data, size_t len)
    {
        using namespace std;
        for (auto i = 0; i < len; i++) buffer.push_back(data[i]);
    }

    const uint8_t* data() const { return buffer.data(); }
    const size_t size() const { return buffer.size(); }
private:
    std::vector<uint8_t>    buffer;
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
        size_t s = get<VarInt_t>(data);
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
    getField(VarIntUnpackStream& stream)
{
    using namespace std;
    auto vtag = stream();
    if (!vtag) return ErrorTag{"getField cannot fetch tag"};
    auto tag = *vtag;
    size_t fieldNumber = tag >> 3;
    tag &= 7;
    switch (tag) {
        case 0: {
            auto value = stream();
            if (!value) return ErrorTag{"getField cannot fetch varint"};
            return make_tuple(fieldNumber, VarInt_t(*value));
        }
        case 1: {
            auto value = stream(sizeof(uint64_t));
            if (!value) return ErrorTag{"getField cannot fetch uint64_t"};
            return make_tuple(fieldNumber, packInto<uint64_t>(*value));
        }
        case 5: {
            auto value = stream(sizeof(uint32_t));
            if (!value) return ErrorTag{"getField cannot fetch uint32_t"};
            return make_tuple(fieldNumber, packInto<uint32_t>(*value));
        }
        case 2: {
            auto len = stream();
            if (!len) return ErrorTag{"getField cannot fetch length for array"};
            auto value = stream(*len);
            if (!value) return ErrorTag{"getField cannot fetch array bytes"};
            return make_tuple(fieldNumber, *value);
        }
        default:
            return ErrorTag{"getField got invalid tag"};
    }
}

void putField(VarIntPackStream& stream, size_t fieldNumber, const uint8_t* data, size_t len)
{
    stream(uint64_t((fieldNumber << 3) | 2));
    stream(len);
    stream(data, len);
}

void putField(VarIntPackStream& stream, size_t fieldNumber, const std::string& data)
{
    putField(stream, fieldNumber, (const uint8_t*)data.data(), data.size());
}

void putField(VarIntPackStream& stream, size_t fieldNumber, const char* data)
{
    putField(stream, fieldNumber, (const uint8_t*)data, strlen(data)-1);
}

void putField(VarIntPackStream& stream, size_t fieldNumber, const VarInt_t& data)
{
    stream(uint64_t((fieldNumber << 3) | 0));
    stream(data);
}

void putField(VarIntPackStream& stream, size_t fieldNumber, const uint32_t& data)
{
    stream(uint64_t((fieldNumber << 3) | 5));
    stream(sizeof(data));
    stream((uint8_t*)&data, sizeof(data));
}

void putField(VarIntPackStream& stream, size_t fieldNumber, const uint64_t& data)
{
    stream(uint64_t((fieldNumber << 3) | 1));
    stream(sizeof(data));
    stream((uint8_t*)&data, sizeof(data));
}

using MsgDef = std::tuple<
    std::pair<std::integral_constant<size_t, 1>, std::vector<uint8_t>>,
    std::pair<std::integral_constant<size_t, 2>, std::vector<uint8_t>>
>;

template <typename Tuple>
struct SecondElementsTuple;

template <>
struct SecondElementsTuple<std::tuple<>> {
    using type = std::tuple<>;  // Resulting tuple is empty
};

template <typename First, typename Second, typename... Rest>
struct SecondElementsTuple<std::tuple<std::pair<First, Second>, Rest...>> {
    // Recursively call SecondElementsTuple for the remaining tuple elements
    using type = decltype(std::tuple_cat(
        std::tuple<Second>(),   // Include the second element of the current pair
        typename SecondElementsTuple<std::tuple<Rest...>>::type()  // Recursively process the rest
    ));
};

template <typename Tuple>
using SecondElementsTuple_t = typename SecondElementsTuple<Tuple>::type;

using Filtered = SecondElementsTuple_t<MsgDef>;


int main(int argc, char *argv[])
{
    using namespace std;

    cout << "MsgDef = " << typeid(MsgDef).name() << endl;
    cout << "Filtered = " << typeid(Filtered).name() << endl;

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
    // // for (auto i = 0; i < x.size(); i++) cout << dec << i << ": " << hex << int(x[i]) << endl;

#if 0
    string payload;
    for (auto i = 0; i < 10; i++) {
        for (auto j = 0; j < 26; j++) {
            payload += char('a'+j);
        }
        payload += '\n';
    }
    VarIntPackStream outStream;
    putField(outStream, 1, attr->SerializeAsString());
    putField(outStream, 2, payload);

    auto msg = new uprotocol::v1::UMessage();
    msg->ParseFromArray(outStream.data(), outStream.size());
    cout << "debug string #############" << endl;
    cout << msg->DebugString() << endl;
    cout << msg->payload() << endl;
#endif

#if 1
    auto it = VarIntUnpackStream(x);

    auto attrPair = getField(it);
    if (!attrPair) {
        cout << "cannot get first field" << endl;
        exit(-1);
    }
    auto [attrField, attrData] = *attrPair;
    auto attrDeser = new uprotocol::v1::UAttributes();
    auto realData = get<vector<uint8_t>>(attrData);
    attrDeser->ParseFromArray(realData.data(), realData.size());
    cout << "first field=" << attrField << endl;
    cout << attrDeser->DebugString();

    auto attrPair2 = getField(it);
    cout << "attrPair2 = " << bool(attrPair2) << endl;
    if (!attrPair2) {
        cout << "cannot get second field" << endl;
        exit(-1);
    }
    auto [attrField2, attrData2] = *attrPair2;
    auto realPayload = get<vector<uint8_t>>(attrData2);
    cout << "second field=" << attrField << ' ' << string_view((const char*)(realPayload.data()), realPayload.size()) << endl;

#endif
}
