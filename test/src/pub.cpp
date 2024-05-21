#include <iostream>
#include <fstream>
#include "UpAbstractTransport.hpp"

void full_doc_pattern_for_zenoh()
{
    auto init_doc = UpAbstractTransport::Doc::parse(
R"(
{
    "transport": "Zenoh",
    "version": "*",
    "params": { "x": 1, "y": 2, "z": 3 }
}
)");

    auto transport = UpAbstractTransport::Transport(init_doc);
}

void string_pattern_for_zenoh()
{
    auto transport = UpAbstractTransport::Transport(
R"(
{
    "transport": "Zenoh",
    "version": 1,
    "params": { "x": 1, "y": 2, "z": 3 }
}
)");
}

void builder_pattern_for_zenoh()
{
    UpAbstractTransport::Doc doc;
    doc["transport"] = "Zenoh";
    doc["version"] = 2;
    doc["params"] = UpAbstractTransport::Doc::parse("{\"x\": 1, \"y\": 2, \"z\": 3 }");
    auto transport = UpAbstractTransport::Transport(doc);
}

void yaml_pattern()
{
    UpAbstractTransport::Doc doc;
    {
        std::ifstream ifs("../zenoh_connect.yaml");
        doc = UpAbstractTransport::Doc::parse(ifs);
    }
    auto transport = UpAbstractTransport::Transport(doc);
}

int main(int argc, char *argv[])
{
    full_doc_pattern_for_zenoh();
    string_pattern_for_zenoh();
    builder_pattern_for_zenoh();
    yaml_pattern();
}