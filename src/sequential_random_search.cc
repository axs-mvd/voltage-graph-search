#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <exception>
#include <limits>

#include <cstdlib>
#include <ctime>
#include <cmath>
#include <cstring>

#include <pthread.h>

#include "lift.hh"
#include "zmxzn.hh"
#include "properties.hh"
#include "bfs_queue.hh"

#include "random_search_thread.hh"

using namespace std;

const int BUFFER_SIZE=2048;

void do_random_search(rs_arguments &arguments, 
                      const uint64_t &max_tries, 
                      const uint32_t &expected_diameter,
                      const uint64_t &search_threads) {
   
    uint32_t expected_delta = 0;
    uint64_t expected_order = 0;

    if (arguments.q.is_bouquet) {
        expected_delta = arguments.q.l * 2 + arguments.q.s;
        expected_order = arguments.g.m * arguments.g.n;
    } else {
        expected_delta = arguments.q.l * 2 + arguments.q.e;
        expected_order = arguments.g.m * arguments.g.n * arguments.q.k;
    }

    shared_ptr<rs_listener> listener(new default_rs_listener(arguments.g.m, arguments.g.n, arguments.g.r,
                                                                         expected_order, expected_delta, 
                                                                         expected_diameter));
    arguments.listener = listener;

    arguments.samples = (uint32_t)ceil((double)max_tries/(double)search_threads);

    random_search_thread::run(&arguments, search_threads);
}

vector<group_parameters> load_group_parameters(string file_name) {
    ifstream stream(file_name.c_str(), ifstream::in);
    vector<group_parameters> groups;
    char buffer[BUFFER_SIZE];

    while (stream.good()) {
        memset(buffer, 0, sizeof(char) * BUFFER_SIZE);
        stream.getline(buffer, BUFFER_SIZE);

        if (buffer[0] != '#') {

            stringstream sstream;
            sstream.str(buffer);

            group_parameters g; 

            if ((sstream >> g.m) &&
                (sstream >> g.n) &&
                (sstream >> g.r)) {

                cout << "[adding] m: " << g.m << " n: " << g.n << " r: " << g.r << endl;

                groups.push_back(g);
            }
        }
    }

    return groups;
}


int main(int argc, char *argv[]) {

    properties p;
    if (argc < 2) {
        p = properties::load("srs.properties");
    } else {
        p = properties::load(argv[1]);
    }

    vector<string> keys = p.get_keys(); 
    cout << "using properties:" << endl;
    for (vector<string>::iterator it = keys.begin(); it != keys.end(); it++) {
        cout << "\t" << (*it) << ": " << p.get(*it) << endl;
    }
    cout << endl;

    srand(time(0));

    rs_arguments arguments;
    arguments.g.m = p.get_unsigned_long("group.zmxzn.m");
    arguments.g.n = p.get_unsigned_long("group.zmxzn.n");
    arguments.g.r = p.get_unsigned_long("group.zmxzn.r");
    

    if (p.get("quotient.type") == "bouquet") {
        arguments.q.is_bouquet = true;
        arguments.q.s = p.get_unsigned_long("quotient.bouquet.s");    
        arguments.q.l = p.get_unsigned_long("quotient.bouquet.l");    
    } else {
        arguments.q.is_bouquet = false;
        arguments.q.k = p.get_unsigned_long("quotient.complete.k");    
        arguments.q.l = p.get_unsigned_long("quotient.complete.l");
        arguments.q.e = p.get_unsigned_long("quotient.complete.e");
    }

    uint32_t expected_diameter = p.get_unsigned("expected.diameter");
    uint64_t tries             = p.get_unsigned_long("max.tries");
    uint64_t search_threads    = p.get_unsigned_long("threads");


    arguments.only_check_diameter = true;
    arguments.expected_diameter = expected_diameter;

    vector<group_parameters> groups = load_group_parameters(p.get("group.file"));

    bfs_queue_factory::file_prefix = p.get("bfs_queue.file_prefix", "/tmp/queue_data_");

    for (vector<group_parameters>::iterator g_it = groups.begin(); g_it != groups.end(); g_it++) {
        arguments.g = *g_it;
        
        cout << "[group parameters] m: " << g_it->m << " n: " << g_it->n << " r: " << g_it->r << endl; 
        do_random_search(arguments, tries, expected_diameter, search_threads);
    }
}

