#include "bfs_queue.hh"

#include <iostream>

#include <cstdio>
#include <cstdlib>
#include <sys/stat.h>
#include <sys/types.h>
#include <sstream>

using namespace std;

bool file_exists(const string &path) {
    struct stat file_info;
    int stat_ret;
    stat_ret = stat(path.c_str(), &file_info);

    if (stat_ret == 0) return true;
    else return false;
}

void delete_file(const string &path) {
    remove(path.c_str());
}

FILE *create_a_file(const string &prefix, string &file_name) {

    do {
        stringstream stream;
        stream << prefix << rand();
        file_name = stream.str();
    } while (file_exists(file_name));

    FILE *file = fopen(file_name.c_str(), "w+");

    cout << __FILE__ << ":" << __LINE__ << " file: " << file << endl;
    return file;
}

void bfs_queue_disk_test() {
    int size = 1000*1000;
    int tail_size = 40;
    int head_size = 40;

    FILE *file = 0; 
    string file_name;
    string prefix = "/home/axs/temp-queue-files/queue_data_file_";

    file = create_a_file(prefix, file_name);

    cout << __FILE__ << ":" << __LINE__ << " file: " << file << endl;
    bfs_in_disk_queue q(head_size, tail_size, size, file, file_name);

    bfs_queue_element e; 
    for (int i = 0; i < size; i++) {
        e.set_lift_vertex_index(i);
        e.set_distance(i+1);
        q.push(e);
/*
        if (i % 1000 == 0) {
            cout << i << " pushes" << endl;
        }
*/
    }

    cout << "finished pushing q.elements() " << q.elements() << endl;    

    for (int i = 0; i < size; i++) {
        e = q.pop();
        if (e.lift_vertex_index() != (uint64_t)i) {
            cout << "error lift vertex" << endl;
        }
        if (e.distance() != (uint16_t)(i + 1)) {
            cout << "error distance" << endl;
        }
/*
        if (i % 1000 == 0) {
            cout << i << " pops" << endl;
        }
*/
    }

    for (int i = 0; i < size; i++) {
        e.set_lift_vertex_index(i);
        e.set_distance(i+1);
        q.push(e);
/*
        if (i % 1000 == 0) {
            cout << i << " pushes" << endl;
        }
*/ 
    }

    for (int i = 0; i < size; i++) {
        e = q.pop();
        if (e.lift_vertex_index() != (uint64_t)i) {
            cout << "error lift vertex" << endl;
        }
        if (e.distance() != (uint16_t)(i + 1)) {
            cout << "error distance" << endl;
        }
/*
        if (i % 1000 == 0) {
            cout << i << " pops" << endl;
        }
*/
    }
   
}

void bfs_factory_test() {
    int size = 100*1000*1000;
    bfs_queue *q = bfs_queue_factory::create_queue(size);

    bfs_queue_element e; 
    for (int i = 0; i < size; i++) {
        e.set_lift_vertex_index(i);
        e.set_distance(i+1);
        q->push(e);
    }

    for (int i = 0; i < size; i++) {
        e = q->pop();
        if (e.lift_vertex_index() != (uint64_t)i) {
            cout << "error lift vertex" << endl;
        }
        if (e.distance() != (uint16_t)(i + 1)) {
            cout << "error distance" << endl;
        }
    }

    for (int i = 0; i < size; i++) {
        e.set_lift_vertex_index(i);
        e.set_distance(i+1);
        q->push(e);
    }

    for (int i = 0; i < size; i++) {
        e = q->pop();
        if (e.lift_vertex_index() != (uint64_t)i) {
            cout << "error lift vertex" << endl;
        }
        if (e.distance() != (uint16_t)(i + 1)) {
            cout << "error distance" << endl;
        }
    }

    delete q;
}

void bfs_queue_memory_test() {
    int size = 20*1000;
    bfs_in_memory_queue q(size);
    
    bfs_queue_element e; 
    for (int i = 0; i < size; i++) {
        e.set_lift_vertex_index(i);
        e.set_distance(i+1);
        q.push(e);
    }

    for (int i = 0; i < size; i++) {
        e = q.pop();
        if (e.lift_vertex_index() != (uint64_t)i) {
            cout << "error lift vertex" << endl;
        }
        if (e.distance() != (uint16_t)(i + 1)) {
            cout << "error distance" << endl;
        }
    }

    for (int i = 0; i < size; i++) {
        e.set_lift_vertex_index(i);
        e.set_distance(i+1);
        q.push(e);
    }

    for (int i = 0; i < size; i++) {
        e = q.pop();
        if (e.lift_vertex_index() != (uint64_t)i) {
            cout << "error lift vertex" << endl;
        }
        if (e.distance() != (uint16_t)(i + 1)) {
            cout << "error distance" << endl;
        }
    }
}


int main() {
    bfs_queue_memory_test();
//    bfs_queue_disk_test();
    bfs_factory_test();

    return 0;
}

