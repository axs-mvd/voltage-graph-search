////////////////////////////////////////////
//      Massive Parallel Gamma Lifter
///////////////////////////////////////////

#include "generator.hh"
#include "properties.hh"
#include "bfs_queue.hh"

#include <iostream>

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

    uint64_t m_a = p.get_unsigned_long("generator.ma");
    uint64_t m_b = p.get_unsigned_long("generator.mb");
    uint64_t n_a = p.get_unsigned_long("generator.na");
    uint64_t n_b = p.get_unsigned_long("generator.nb");

    uint64_t order_lower_bound = p.get_unsigned_long("expected.order");
    uint64_t target_delta      = p.get_unsigned("expected.delta");
    uint64_t target_diameter   = p.get_unsigned("expected.diameter");

    uint64_t evaluation_samples = p.get_unsigned("generator.samples");
    uint32_t generator_threads = p.get_unsigned("generator.threads");

    bfs_queue_factory::file_prefix = p.get("bfs_queue.file_prefix", "/tmp/queue_data_");

    quotient_parameters q;

    q.is_bouquet = (p.get("quotient.type") == "bouquet");

    if (q.is_bouquet) {

       q.s = p.get_unsigned("quotient.bouquet.s"); 
       q.l = p.get_unsigned("quotient.bouquet.l"); 

    } else {

       q.k = p.get_unsigned("quotient.complete.k"); 
       q.l = p.get_unsigned("quotient.complete.l"); 
       q.e = p.get_unsigned("quotient.complete.e"); 

    }
  
    uint32_t top = p.get_unsigned("mpgl.lift_top");
    uint32_t samples = p.get_unsigned("mpgl.samples");
 
    vector<tuple> tuples = generator::start(m_a, m_b, n_a, n_b, 
                                order_lower_bound, target_delta, 
                                target_diameter, q, evaluation_samples, 
                                generator_threads);

    for (vector<tuple>::iterator it = tuples.begin();
         it != tuples.end(); it++) {

        cout << it->tostring() << " tier: " << it->dominated_by << endl;

    }

    cout << "Output for srs" << endl;
    for (vector<tuple>::iterator it = tuples.begin();
         it != tuples.end(); it++) {
        cout << "#srs " << it->m << " " << it->n << " " << it->r << endl;
    }

    cout << "Starting evaluations for top " << top << " tuples " << endl; 
    uint32_t tried_tuples = 0;
    for (vector<tuple>::iterator it = tuples.begin();
         tried_tuples < top && it != tuples.end(); it++) {
        
        tuple t = *it;
        uint64_t expected_order = 0;
        if (q.is_bouquet) {
            expected_order = t.m * t.n;
        } else {
            expected_order = t.m * t.n * q.k;
        }
          
        cout << "\tevaluating: " << t.tostring() << " expected order: " << expected_order 
             << " target_delta: " << target_delta <<  " target_diameter: " << target_diameter << endl;

        rs_arguments arguments;
        arguments.g.m = t.m;
        arguments.g.n = t.n;
        arguments.g.r = t.r;

        arguments.q = q;

        arguments.only_check_diameter = true;
        arguments.expected_diameter = target_diameter; 

        shared_ptr<rs_listener> listener(new default_rs_listener(arguments.g.m, arguments.g.n, arguments.g.r,
                                                                         expected_order, target_delta, 
                                                                         target_diameter));
        arguments.listener = listener;

        arguments.samples = (uint32_t)ceil((double)samples/(double)generator_threads);

        random_search_thread::run(&arguments, generator_threads);

        tried_tuples++;
    }

    return 0;
}
