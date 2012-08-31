#include "random_search_thread.hh"
#include "quotient_factory.hh"

string get_date() {
    char *temp = new char[200];
    time_t rawtime;
    struct tm * timeinfo;

    rawtime = time (0);
    timeinfo = localtime(&rawtime);

    strftime(temp, 200, "%a %H:%M:%S", timeinfo);        

    string t = string(temp);
    delete [] temp;
    return t;
}

void *random_search_thread::routine(void *a) {
    rs_arguments *args = (rs_arguments *) a;
    
    shared_ptr<rs_listener> listener = args->listener;

    shared_ptr<group> g(new zmxzn(args->g.m, args->g.n, args->g.r));
    
    listener->on_begin();

    for (uint32_t i = 0; i < args->samples; i++) {

        bool is_bouquet = args->q.is_bouquet;
        uint32_t s      = args->q.s;
        uint32_t l      = args->q.l;
        uint32_t e      = args->q.e;
        uint32_t k      = args->q.k;

        try {
            shared_ptr<graph> gamma = quotient_factory::create_random_quotient(is_bouquet, k, s, l, e, g);
            lift gamma_alpha(gamma, g);

            uint64_t order    = gamma_alpha.size();
            uint32_t delta    = gamma_alpha.delta();

            if (args->only_check_diameter) {

                bool is_valid = gamma_alpha.check_diameter(args->expected_diameter);
                listener->on_check(gamma, order, delta, is_valid, 
                                   args->expected_diameter, i);

            } else {

                uint32_t diameter = gamma_alpha.diameter();
                listener->on_evaluation(gamma, order, delta, diameter, i);
            }
        } catch (involutives_exception &) {
        }
    }

    listener->on_end();
    return 0;
}

pthread_t random_search_thread::start(rs_arguments *args) {
   pthread_t thread;
   pthread_create(&thread, 0, random_search_thread::routine, args);
   return thread;
}

void random_search_thread::run(rs_arguments *args, const uint32_t &thread_count) {
    vector<pthread_t> threads;

    for (uint32_t i = 0; i < thread_count; i++) {
        pthread_t thread = random_search_thread::start(args);
        threads.push_back(thread);
    }

    for (uint32_t i = 0; i < threads.size(); i++) {
        pthread_join(threads[i], 0);
    }
}

