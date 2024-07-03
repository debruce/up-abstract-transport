#pragma once

#include "tuple_of_optionals.h"
#include <unordered_map>
#include <memory>
#include <mutex>
#include <vector>

template <typename KEY, typename VALUE>
class SafeTupleMap {
    std::unordered_map<KEY, std::shared_ptr<VALUE>, tuple_of_optionals::hash<KEY>>  map_;
    std::mutex  mtx;

public:
    SafeTupleMap() = default;

    void insert(const KEY& key, std::shared_ptr<VALUE> ptr)
    {
        std::unique_lock<std::mutex>    lock(mtx);
        map_.emplace(key, ptr);
    }

    void insert(const std::vector<KEY>& keys, std::shared_ptr<VALUE> ptr)
    {
        std::unique_lock<std::mutex>    lock(mtx);
        for (const auto& key : keys)
            map_.emplace(key, ptr);
    }

    std::shared_ptr<VALUE> find(const KEY& key)
    {
        std::unique_lock<std::mutex>    lock(mtx);
        auto it = map_.find(key);
        if (it == map_.end()) return nullptr;
        return it->second;        
    }

    void dump(std::ostream& os)
    {
        for (const auto& [k, v] : map_) {
            os << k << " = " << *v << std::endl;
        }
    }
};
