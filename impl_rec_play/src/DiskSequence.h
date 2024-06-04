#pragma once
#define PYBIND11_NO_ASSERT_GIL_HELD_INCREF_DECREF
#include <memory>
#include <nlohmann/json.hpp>
#include <string>

class DiskWriter {
public:
    explicit DiskWriter(const std::string&);
    ~DiskWriter();
    void commit(double timeStamp, uint64_t marker, const nlohmann::json& payload);

private:
    struct Impl;
    std::unique_ptr<Impl> pImpl;
};

class DiskReader {
public:
    explicit DiskReader(const std::string&);
    ~DiskReader();
    nlohmann::json read(size_t index, double& timeStamp, int64_t& marker);

private:
    struct Impl;
    std::unique_ptr<Impl> pImpl;
};
