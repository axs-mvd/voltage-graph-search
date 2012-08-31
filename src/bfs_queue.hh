#pragma once

#include <exception>
#include <iostream>
#include <sstream>
#include <string>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdint.h>

using namespace std;

class bfs_queue_full_exception  : public exception {};
class bfs_queue_empty_exception : public exception {};
class bfs_queue_write_fail_exception : public exception {};


class bfs_queue_element {
private:
    uint64_t  _lift_vertex_index;
    uint16_t _distance;

public:

    bfs_queue_element() {
        _lift_vertex_index = 0;
        _distance = 0;
    }

    bfs_queue_element(const uint64_t  &lift_vertex_index,
                      const uint16_t &distance) {
        _lift_vertex_index = lift_vertex_index;
        _distance = distance;
    }

    virtual ~bfs_queue_element() {
    }

    bfs_queue_element(const bfs_queue_element &e) {
        _lift_vertex_index = e._lift_vertex_index;
        _distance = e._distance;
    }

    bfs_queue_element &operator=(const bfs_queue_element &e) {
        if (this != &e) {
            _lift_vertex_index = e._lift_vertex_index;
            _distance = e._distance;
        }
        return *this;
    }

    void set_lift_vertex_index(uint64_t lift_vertex_index) {
        _lift_vertex_index = lift_vertex_index;
    }

    void set_distance(uint16_t distance) {
        _distance = distance;
    }

    uint64_t lift_vertex_index() const {
        return _lift_vertex_index;
    }

    uint16_t distance() const {
        return _distance;
    }

    void serialize(char *bytes, long start) const {
        bcopy(&_lift_vertex_index, bytes + start, sizeof(uint64_t)); 
        bcopy(&_distance, bytes + start + sizeof(uint64_t), sizeof(uint16_t)); 
    }

    static bfs_queue_element deserialize(char *bytes, long start) {
        uint64_t  lift_vertex_index;
        uint16_t distance;
        
        bcopy(bytes + start, &lift_vertex_index, sizeof(uint64_t));
        bcopy(bytes + start + sizeof(uint64_t), &distance, sizeof(uint16_t));

        bfs_queue_element e(lift_vertex_index, distance);
        return e;
    }

    static size_t data_size() {
        return sizeof(uint64_t) + sizeof(uint16_t);
    }
};

class bfs_queue {

public:

    bfs_queue() {
    }

    virtual ~bfs_queue() {
    }

    virtual bfs_queue_element pop() = 0;

    virtual void push(const bfs_queue_element &e) = 0;

    virtual uint64_t capacity() const = 0;

    virtual uint64_t elements() const = 0;

    virtual bool empty() const {
        return elements() == 0;
    } 

    virtual bool full() const {
        return elements() == capacity();
    }
};


class bfs_in_memory_queue : public bfs_queue {

private:

    bfs_queue_element *_data;
    uint64_t _size;
    uint64_t _head;
    uint64_t _tail;
    uint64_t _count;

public:    

    bfs_in_memory_queue(uint64_t max_size) {
        _data = new bfs_queue_element[max_size];
        _count = _head = _tail = 0;
        _size = max_size;
    }

    virtual ~bfs_in_memory_queue() {
        delete [] _data;
    }

    bfs_queue_element pop() {

        if (_count == 0) throw bfs_queue_empty_exception();

        uint64_t point = _head;
        _head = (_head + 1) % _size;
        _count--;
        return _data[point];
    }

    void push(const bfs_queue_element &e) {
        if (_count == _size) throw bfs_queue_full_exception();

        _data[_tail] = e;
        _tail = (_tail + 1) % _size;
        _count++;
    }

    uint64_t capacity() const {
        return _size;
    }

    uint64_t elements() const {
        return _count;
    }
};

class bfs_in_disk_queue : public bfs_queue {

private:
    
    FILE *_file;
    string _file_path;
    bfs_in_memory_queue _head_queue;
    bfs_in_memory_queue _tail_queue;
    uint64_t _size;
    uint64_t _count;
    uint64_t _head_pointer;

public:

    bfs_in_disk_queue(uint64_t head_size,
                      uint64_t tail_size,
                      uint64_t total_size,
                      FILE *file,
                      string file_path) : _head_queue(head_size), 
                                  _tail_queue(tail_size) {
        _file = file;
        _file_path = file_path;
        _size = total_size;
        _count = 0;
        _head_pointer = 0;
    }

    virtual ~bfs_in_disk_queue(){
        fclose(_file); 
        remove(_file_path.c_str());
    }

    bfs_queue_element pop() {
        if (_head_queue.empty()) {
            load_head();
        } 

        _count--;
        if (_head_queue.empty()) {
            return _tail_queue.pop();
        } else {
            return _head_queue.pop();
        }
    }

    void push(const bfs_queue_element &e) {
        if (_tail_queue.full()) {
            save_tail();
        }

        _count++;    
        _tail_queue.push(e);
    }

    uint64_t capacity() const {
        return _size;
    }

    uint64_t elements() const {
        return _count;
    }


private:
    void save_tail();
    void load_head();
};

class bfs_queue_factory {
public:
    static uint64_t threshold; 
    static string file_prefix;
    static uint64_t head_queue_size;
    static uint64_t tail_queue_size;

    static bfs_queue *create_queue(unsigned size);   

private:

    static bool file_exists(const string &path); 
    static FILE *create_file(string &file_name);

};

