#include "bfs_queue.hh"

void bfs_in_disk_queue::save_tail() {
    unsigned buffer_size = bfs_queue_element::data_size() * _tail_queue.elements();
    char *buffer = new char[buffer_size];

    bfs_queue_element e;
    uint64_t offset = 0;

    while (!_tail_queue.empty()) {
        e = _tail_queue.pop();
        e.serialize(buffer, offset);
        offset += bfs_queue_element::data_size();
    }

    fseek(_file, 0, SEEK_END);
    size_t written_elements = fwrite(buffer, sizeof(char), buffer_size, _file);
    if (written_elements != buffer_size) {
        throw bfs_queue_write_fail_exception();
    }
    delete [] buffer;
}

void bfs_in_disk_queue::load_head() {
    unsigned buffer_size = bfs_queue_element::data_size() * _head_queue.capacity();
    char *buffer = new char[buffer_size];
    
    fseek(_file, _head_pointer, SEEK_SET);
    unsigned read_bytes = fread(buffer, sizeof(char), buffer_size, _file);
   
    bfs_queue_element e; 
    unsigned buffer_pointer = 0;

    while (buffer_pointer < read_bytes) {
        e = bfs_queue_element::deserialize(buffer, buffer_pointer);
        _head_queue.push(e);
        buffer_pointer += bfs_queue_element::data_size();
    }
    _head_pointer += read_bytes;
    delete [] buffer;
}

uint64_t bfs_queue_factory::threshold = 40*1000*1000; 
string bfs_queue_factory::file_prefix = "/tmp/queue_data_";
uint64_t bfs_queue_factory::head_queue_size = 5*1000*1000;
uint64_t bfs_queue_factory::tail_queue_size = 5*1000*1000;

bool bfs_queue_factory::file_exists(const string &path) {
    struct stat file_info;
    int stat_ret;
    stat_ret = stat(path.c_str(), &file_info);

    if (stat_ret == 0) return true;
    else return false;
}

FILE *bfs_queue_factory::create_file(string &file_name) {
    do {
        stringstream stream;
        stream << file_prefix << rand();
        file_name = stream.str();
    } while (file_exists(file_name));

    FILE *file = fopen(file_name.c_str(), "w+");

    return file;
}

bfs_queue *bfs_queue_factory::create_queue(unsigned size) {
    if (size < threshold) {
        return new bfs_in_memory_queue(size);
    } else {
        string file_name;
        FILE *file = create_file(file_name);
        return new bfs_in_disk_queue(head_queue_size, 
                                     tail_queue_size,
                                     size,
                                     file,
                                     file_name);
    }
}  

