#pragma once

#include <cstdint>
#include <string>
// MurmurHash3 实现
class MurmurHash3 {
public:
    static uint32_t hash32(const void* key, int len, uint32_t seed = 0) {
        const uint8_t* data = static_cast<const uint8_t*>(key);
        const int nblocks = len / 4;
        
        uint32_t h1 = seed;
        const uint32_t c1 = 0xcc9e2d51;
        const uint32_t c2 = 0x1b873593;
        
        // 处理4字节块
        const uint32_t* blocks = reinterpret_cast<const uint32_t*>(data + nblocks * 4);
        for (int i = -nblocks; i; i++) {
            uint32_t k1 = blocks[i];
            
            k1 *= c1;
            k1 = rotl32(k1, 15);
            k1 *= c2;
            
            h1 ^= k1;
            h1 = rotl32(h1, 13);
            h1 = h1 * 5 + 0xe6546b64;
        }
        
        // 处理剩余字节
        const uint8_t* tail = data + nblocks * 4;
        uint32_t k1 = 0;
        
        switch (len & 3) {
            case 3: k1 ^= tail[2] << 16;
            case 2: k1 ^= tail[1] << 8;
            case 1: k1 ^= tail[0];
                k1 *= c1;
                k1 = rotl32(k1, 15);
                k1 *= c2;
                h1 ^= k1;
        }
        
        // 最终化
        h1 ^= len;
        h1 = fmix32(h1);
        
        return h1;
    }
    
    // 针对不同类型的特化
    static uint32_t hash(int32_t key) {
        return hash32(&key, sizeof(key));
    }
    
    static uint32_t hash(uint32_t key) {
        return hash32(&key, sizeof(key));
    }
    
    static uint32_t hash(int64_t key) {
        return hash32(&key, sizeof(key));
    }
    
    static uint32_t hash(uint64_t key) {
        return hash32(&key, sizeof(key));
    }
    
    static uint32_t hash(const std::string& key) {
        return hash32(key.data(), static_cast<int>(key.size()));
    }
    
private:
    static uint32_t rotl32(uint32_t x, int8_t r) {
        return (x << r) | (x >> (32 - r));
    }
    
    static uint32_t fmix32(uint32_t h) {
        h ^= h >> 16;
        h *= 0x85ebca6b;
        h ^= h >> 13;
        h *= 0xc2b2ae35;
        h ^= h >> 16;
        return h;
    }
};