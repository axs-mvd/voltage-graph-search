#pragma once

#include <iostream>
#include <cstring>
#include <stdint.h>

using namespace std;

class big_bitset {
private:
    uint64_t *_data;
    uint64_t _data_size;
    uint64_t _size;
public:
    big_bitset(uint64_t bits) {
        _data_size = bits / (sizeof(uint64_t)*8) + 1;
        _data = new uint64_t[_data_size];
        _size = bits;
        memset(_data, 0, _data_size * sizeof(uint64_t));
    }

    big_bitset(const big_bitset &b) {
        _data_size = b._data_size;
        _size = b._size;
        _data = new uint64_t[_data_size];
        memcpy(_data, b._data, sizeof(uint64_t) * _data_size);
    }
   
    virtual ~big_bitset() {
        delete [] _data;
        _data = 0;
    }

    big_bitset &operator=(const big_bitset &b) {
        if (this != &b) {
            delete [] _data;
            _data_size = b._data_size;
            _size = b._size;
            _data = new uint64_t[_data_size];

            memcpy(_data, b._data, sizeof(uint64_t) * _data_size);
        }
        return *this;
    }

    uint64_t size() {
        return _size;
    }

    bool get(uint64_t pos) const {
        uint64_t array_position = pos / (sizeof(uint64_t) * 8);
        uint16_t in_position = pos % (sizeof(uint64_t) * 8);
        return get_from_a_long(_data[array_position], in_position);
    }

    void set(uint64_t pos, bool value = true) {
        uint64_t array_position = pos / (sizeof(uint64_t) * 8);
        uint16_t in_position = pos % (sizeof(uint64_t) * 8);
        return set_in_a_long(_data[array_position], in_position, value);
    }

private:

    static bool get_from_a_long(const uint64_t &data, uint16_t pos) {
        return data & (1l << pos);
    }

    static void set_in_a_long(uint64_t &data, uint16_t pos, bool value) {
        if (value) {
            data |= (1l << pos);
        } else {
            data &= (1l << pos);
        }
    }
};


