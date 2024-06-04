#include "DiskSequence.h"

#include <mutex>
#include <sstream>
#include <stdexcept>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

using namespace std;
using nlohmann::json;

struct IndexRecord {
    size_t offset;
    size_t size;
    double timeStamp;
    uint64_t marker;
};

static string getErrno(int e) {
    char buf[256];
    char* ret = strerror_r(e, buf, sizeof(buf) - 1);
    if (buf == ret)
        return string(buf);
    else
        return string();
}

struct DiskWriter::Impl {
    mutex mtx;
    string index_path;
    string cbor_path;
    int index_fd;
    int cbor_fd;

    explicit Impl(const string& path) {
        index_path = path + ".index";
        cbor_path = path + ".cbor";

        index_fd = open(index_path.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (index_fd < 0) {
            stringstream ss;
            ss << "DiskWriter failed due to \"" << getErrno(errno) << "\" for \"" << index_path
               << '"';
            throw runtime_error(ss.str());
        }

        cbor_fd = open(cbor_path.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (cbor_fd < 0) {
            close(index_fd);
            stringstream ss;
            ss << "DiskWriter failed due to \"" << getErrno(errno) << "\" for \"" << cbor_path
               << '"';
            throw runtime_error(ss.str());
        }
    }

    ~Impl() {
        lock_guard<mutex> lock(mtx);
        close(index_fd);
        close(cbor_fd);
    }

    void commit(double timeStamp, uint64_t marker, const json& payload) {
        int ret;
        auto b = json::to_cbor(payload);

        lock_guard<mutex> lock(mtx);
        IndexRecord rec;
        rec.offset = lseek(cbor_fd, 0, SEEK_END);
        rec.size = b.size();
        rec.timeStamp = timeStamp;
        rec.marker = marker;
        ret = write(cbor_fd, b.data(), b.size());
        if (ret < 0) {
            stringstream ss;
            ss << "DiskWriter::commit failed due to \"" << getErrno(errno) << "\" for \""
               << index_path << '"';
            throw runtime_error(ss.str());
        }
        ret = write(index_fd, &rec, sizeof(rec));
        if (ret < 0) {
            stringstream ss;
            ss << "DiskWriter::commit failed due to \"" << getErrno(errno) << "\" for \""
               << cbor_path << '"';
            throw runtime_error(ss.str());
        }
    }
};

DiskWriter::DiskWriter(const string& path) : pImpl(new Impl(path)) {}

DiskWriter::~DiskWriter() {}

void DiskWriter::commit(double timeStamp, uint64_t marker, const nlohmann::json& payload) {
    pImpl->commit(timeStamp, marker, payload);
}

struct DiskReader::Impl {
    string index_path;
    string cbor_path;
    int index_fd;
    int cbor_fd;

    explicit Impl(const string& path) {
        index_path = path + ".index";
        cbor_path = path + ".cbor";

        index_fd = open(index_path.c_str(), O_RDONLY);
        if (index_fd < 0) {
            stringstream ss;
            ss << "DiskReader failed due to \"" << getErrno(errno) << "\" for \"" << index_path
               << '"';
            close(cbor_fd);
            throw runtime_error(ss.str());
        }

        cbor_fd = open(cbor_path.c_str(), O_RDONLY);
        if (cbor_fd < 0) {
            stringstream ss;
            ss << "DiskReader failed due to \"" << getErrno(errno) << "\" for \"" << cbor_path
               << '"';
            throw runtime_error(ss.str());
        }
    }

    ~Impl() {
        close(index_fd);
        close(cbor_fd);
    }

    json read(size_t index, double& timeStamp, int64_t& marker) {
        int ret;
        IndexRecord rec;

        ret = pread(index_fd, &rec, sizeof(rec), index * sizeof(rec));
        if (ret <= 0) {
            stringstream ss;
            ss << "DiskReader::read failed due to \"" << getErrno(errno) << "\" for \""
               << index_path << '"';
            throw runtime_error(ss.str());
        }
        vector<uint8_t> cbor_data;
        cbor_data.resize(rec.size);
        ret = pread(cbor_fd, cbor_data.data(), rec.size, rec.offset);
        if (ret <= 0) {
            stringstream ss;
            ss << "DiskReader::read failed due to \"" << getErrno(errno) << "\" for \"" << cbor_path
               << '"';
            throw runtime_error(ss.str());
        }

        timeStamp = rec.timeStamp;
        marker = rec.marker;
        return json::from_cbor(cbor_data);
    }
};

DiskReader::DiskReader(const string& path) : pImpl(new Impl(path)) {}

DiskReader::~DiskReader() {}

json DiskReader::read(size_t index, double& timeStamp, int64_t& marker) {
    return pImpl->read(index, timeStamp, marker);
}
