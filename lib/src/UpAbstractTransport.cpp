#include "UpAbstractTransport.hpp"
#include <iostream>

namespace UpAbstractTransport {

using namespace std;

struct Transport::Impl {
    Impl(const Doc& init_doc) {
        auto transport_style = init_doc["transport"].get<string>();
        cout << "transport_style = " << transport_style << endl;
        if (init_doc["version"].type() == nlohmann::json::value_t::string) {
            cout << "version is a string" << endl;
        }
        else if (init_doc["version"].type() == nlohmann::json::value_t::number_unsigned) {
            cout << "version is an integer" << endl;
        }
        // if 
        // if (init_doc["transport"])
    }

    ~Impl() {
        cout << "~Impl" << endl;
    }
};


Transport::Transport(const Doc& init_doc)
    : pImpl(new Impl(init_doc))
{
}

Transport::Transport(const char* init_string)
    : pImpl(new Impl(Doc::parse(init_string)))
{
}


// struct Publisher::Impl {
//     Impl() {

//     }

//     ~Impl() {

//     }
// };

// Publisher::Publisher(
//     Transport transport,
//     const UUri& topic,
//     const PayloadFormat& format,
//     std::optional<Priority> priority = {},
//     std::optional<Ttl> ttl = {}
//     );
    
// Status publish(const PayloadView&);

}; // namespace UpAbstractTransport