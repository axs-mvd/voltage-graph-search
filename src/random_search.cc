#include <iostream>
#include <algorithm>
#include <vector>

#include <cstdlib>
#include <exception>
#include <limits>
#include <ctime>
#include <cmath>
#include <pthread.h>

#include "lift.hh"
#include "zmxzn.hh"
#include "properties.hh"
#include "bfs_queue.hh"

#include "random_search_thread.hh"

using namespace std;

int main(int argc, char *argv[]) {

    properties p;
    if (argc < 2) {
        p = properties::load("random_search.properties");
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
    arguments.only_check_diameter = true;
    arguments.expected_diameter = p.get_unsigned("expected.diameter");
    

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
    uint32_t expected_delta    = p.get_unsigned("expected.delta");
    uint64_t expected_order    = p.get_unsigned_long("expected.order");
    uint64_t tries             = p.get_unsigned_long("max.tries");
    uint64_t search_threads    = p.get_unsigned_long("threads");

   bfs_queue_factory::file_prefix = p.get("bfs_queue.file_prefix", "/tmp/queue_data_");

    shared_ptr<rs_listener> listener(new default_rs_listener(arguments.g.m, arguments.g.n, arguments.g.r,
                                                                         expected_order, expected_delta, 
                                                                         expected_diameter));

    arguments.listener = listener;

    arguments.samples = (uint32_t)ceil((double)tries/(double)search_threads);

    random_search_thread::run(&arguments, search_threads);

}

