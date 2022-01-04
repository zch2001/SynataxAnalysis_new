#include <list>
#include <functional>
#include <bitset>
#include <iostream>

#include "hash/extendible_hash.h"
#include "page/page.h"

namespace scudb {

/*
 * constructor
 * array_size: fixed array size for each bucket
 */
template <typename K, typename V>
ExtendibleHash<K, V>::ExtendibleHash(size_t size) : bucket_size(size), 
bucket_count(0), pair_count(0), global_depth(0) {
    buckets.emplace_back(new Bucket(0, 0));
    bucket_count = 1;
}

/*
 * helper function to calculate the hashing address of input key
 */
template <typename K, typename V>
size_t ExtendibleHash<K, V>::HashKey(const K &key) {
    return std::hash<K>()(key);
}

/*
 * helper function to return global depth of hash table
 * NOTE: you must implement this function in order to pass test
 */
template <typename K, typename V>
int ExtendibleHash<K, V>::GetGlobalDepth() const {
    std::lock_guard<std::mutex> lock(mutex_w);
    return global_depth;
}

/*
 * helper function to return local depth of one specific bucket
 * NOTE: you must implement this function in order to pass test
 */
template <typename K, typename V>
int ExtendibleHash<K, V>::GetLocalDepth(int bucket_id) const {
    std::lock_guard<std::mutex> lock(mutex_w);
    if (buckets[bucket_id]) {
        return buckets[bucket_id]->local_depth;
    }
    else return -1;
}

/*
 * helper function to return current number of bucket in hash table
 */
template <typename K, typename V>
int ExtendibleHash<K, V>::GetNumBuckets() const {
    std::lock_guard<std::mutex> lock(mutex_w);
    return bucket_count;
}

/*
 * lookup function to find value associate with input key
 */
template <typename K, typename V>
bool ExtendibleHash<K, V>::Find(const K &key, V &value) {
    std::lock_guard<std::mutex> lock(mutex_w);
    auto hk = HashKey(key);
    size_t pos_in_hash = hk & ((1 << global_depth) - 1);

    if (buckets[pos_in_hash]) {
        if (buckets[pos_in_hash]->items.find(key) != buckets[pos_in_hash]->items.end()) {
            value = buckets[pos_in_hash]->items[key];
            return true;
        }
    }
    return false;
}

/*
 * delete <key,value> entry in hash table
 * Shrink & Combination is not required for this project
 */
template <typename K, typename V>
bool ExtendibleHash<K, V>::Remove(const K &key) {
    std::lock_guard<std::mutex> lock(mutex_w);
    size_t pos_in_hash = HashKey(key) & ((1 << global_depth) - 1);
    size_t flag = 0;

    if (buckets[pos_in_hash]) {
        auto tmp_bucket = buckets[pos_in_hash];
        flag = tmp_bucket->items.erase(key);         // erase返回0 or 1
        pair_count -= flag;
    }
    return flag != 0;
}

/*
 * insert <key,value> entry in hash table
 * Split & Redistribute bucket when there is overflow and if necessary increase
 * global depth
 */
template <typename K, typename V>
void ExtendibleHash<K, V>::Insert(const K &key, const V &value) {
    std::lock_guard<std::mutex> lock(mutex_w);
    size_t bucket_id = HashKey(key) & ((1 << global_depth) - 1);
    //如果散列表中指针没有创建对应桶则新建桶
    if (buckets[bucket_id] == nullptr) {
        buckets[bucket_id] = std::make_shared<Bucket>(bucket_id, global_depth);
        bucket_count++;
    }
    
    auto bucket = buckets[bucket_id];
    //如果插入位置有值则覆盖
    if (bucket->items.find(key) != bucket->items.end()) {
        bucket->items[key] = value;
        return;
    }
    //插入记录
    bucket->items.insert({ key, value });
    pair_count++;

    if (bucket->items.size() > bucket_size) {
        auto old_index = bucket->id;
        auto old_depth = bucket->local_depth;

        std::shared_ptr<Bucket> new_bucket = split(bucket);

        if (bucket->local_depth > global_depth) {
            auto size = buckets.size();
            auto factor = (1 << (bucket->local_depth - global_depth));

            global_depth = bucket->local_depth;
            buckets.resize(buckets.size() * factor);

            // 修改和添加要插入的桶和新建的桶
            buckets[bucket->id] = bucket;
            buckets[new_bucket->id] = new_bucket;

            // 遍历桶数组
            for (size_t i = 0; i < size; ++i) {
                if (buckets[i]) {
                    if(i >= buckets[i]->id) {
                        auto step = 1 << buckets[i]->local_depth;
                        for (size_t j = i + step; j < buckets.size(); j += step) {
                            buckets[j] = buckets[i];
                        }
                    }
                }
            }
        }
        else {
            for (size_t i = old_index; i < buckets.size(); i += (1 << old_depth)) {
                buckets[i].reset();
            }

            buckets[bucket->id] = bucket;
            buckets[new_bucket->id] = new_bucket;

            auto step = 1 << bucket->local_depth;
            for (size_t i = bucket->id + step; i < buckets.size(); i += step) {
                buckets[i] = bucket;
            }
            for (size_t i = new_bucket->id + step; i < buckets.size(); i += step) {
                buckets[i] = new_bucket;
            }
        }
    }
}

// 分裂桶
template <typename K, typename V>
std::shared_ptr<typename ExtendibleHash<K, V>::Bucket>
ExtendibleHash<K, V>::split(std::shared_ptr<Bucket>& b) {
    // 先创建一个新桶
    auto res = std::make_shared<Bucket>(0, b->local_depth);
    while (res->items.empty()) {
        // 先将深度加一
        b->local_depth++;
        res->local_depth++;
        // for循环实现两个桶的重新分配
        for (auto it = b->items.begin(); it != b->items.end();) {
            if (HashKey(it->first) & (1 << (b->local_depth - 1))) {
                res->items.insert(*it);
                res->id = HashKey(it->first) & ((1 << b->local_depth) - 1);
                it = b->items.erase(it);
            }
            else {
                ++it;
            }
        }

        // 如果b桶的map为空，则清空当前桶继续遍历
        if (b->items.empty()) {
            b->items.swap(res->items);
            b->id = res->id;
        }
    }

    ++bucket_count;
    return res;
}

template class ExtendibleHash<page_id_t, Page *>;
template class ExtendibleHash<Page *, std::list<Page *>::iterator>;
// test purpose
template class ExtendibleHash<int, std::string>;
template class ExtendibleHash<int, std::list<int>::iterator>;
template class ExtendibleHash<int, int>;
} // namespace scudb
