#include "generator.hh"

uint64_t generator::power_modulus(uint64_t base, uint64_t exponent, uint64_t modulus) {
    uint64_t result = 1;
    while (exponent > 0) {
        if ((exponent & 1) == 1) {
            result = (result * base) % modulus;
        }
        exponent = exponent >> 1;
        base = (base * base) % modulus;
    }
    return result;
}

//Stein algorithm from: http://en.wikipedia.org/wiki/Binary_GCD_algorithm
uint64_t generator::gcd(uint64_t u, uint64_t v) {
    int shift;

    /* GCD(0,x) := x */
    if (u == 0 || v == 0)
        return u | v;

    /* Let shift := lg K, where K is the greatest power of 2
       dividing both u and v. */
    for (shift = 0; ((u | v) & 1) == 0; ++shift) {
        u >>= 1;
        v >>= 1;
    }

    while ((u & 1) == 0)
        u >>= 1;

    /* From here on, u is always odd. */
    do {
        while ((v & 1) == 0)  /* Loop X */
            v >>= 1;

        /* Now u and v are both odd, so diff(u, v) is even.
           Let u = min(u, v), v = diff(u, v)/2. */
        if (u < v) {
            v -= u;
        } else {
            uint64_t diff = u - v;
            u = v;
            v = diff;
        }
        v >>= 1;
    } while (v != 0);
    return u << shift;
}

//Naive aproximation, should work
uint64_t generator::phi(const uint64_t &m) {
    uint64_t count = 0;
    for (uint64_t i = 1; i < m; i++) {
        if (gcd(i, m) == 1) count++;
    }
    return count;
}

vector<tuple> generator::start(const uint64_t &m_a, const uint64_t &m_b, const uint64_t &n_a, const uint64_t &n_b,
                      const uint64_t &order_lower_bound, const uint32_t &target_delta, 
                      const uint32_t &target_diameter, const quotient_parameters &q, 
                      const uint32_t &eval_samples, const uint32_t &threads) {

    vector<tuple> tuples;

    for (uint64_t m = m_a; m <= m_b; m++) {
        for (uint64_t n = n_a; n <= n_b; n++) {
            uint64_t max_generated = 0;
            
            vector<uint64_t> rs = maximal_r(m, n, max_generated);


            for (vector<uint64_t>::const_iterator it = rs.begin(); it != rs.end(); it++) {
            
                tuple t;
                t.m = m;
                t.n = n;
                t.r = *it;
                
                bool is_valid = evaluate_tuple(t, q, order_lower_bound, 
                               target_delta, target_diameter, eval_samples, threads);

                if (is_valid) {
                    cout << "{" << get_date() << "} " 
                        << " adding m: " << t.m << " n: " << t.n << " r: " << t.r 
                        << " dga: " << t.diameter_green_achievement 
                        << " da: " << t.delta_achievement << endl;
                    tuples.push_back(t);
                } 
            }
        }
    }
    cout << "sorting..." << endl;
    return pareto_sort(tuples);
}

bool generator::evaluate_tuple(tuple &t, const quotient_parameters &q, 
                               const uint64_t &order_lower_bound, const uint32_t &target_delta, 
                               const uint32_t &target_diameter, const uint32_t &eval_samples,
                               const uint32_t &threads) {

    uint64_t order;
    uint64_t delta_hipotesis;
    if (q.is_bouquet) {
        order = t.m * t.n; 
        delta_hipotesis = q.l*2 + q.s;
    } else {
        order = t.m * t.n * q.k;
        delta_hipotesis = q.l*2 + q.e*(q.k-1);
    }

    if (order < order_lower_bound) return false;
    if (delta_hipotesis != target_delta) return false;

    t.m_phi = t.m - phi(t.m);

   shared_ptr<evaluation_rs_listener> listener(new evaluation_rs_listener(t.m, t.n, t.r, 
                                       order_lower_bound, target_delta, target_diameter));

    rs_arguments arguments; 

    arguments.q = q;
    arguments.g.m = t.m;
    arguments.g.n = t.n;
    arguments.g.r = t.r;
    arguments.listener = listener;
    arguments.samples  = ceil((double)eval_samples/(double)threads);

    arguments.only_check_diameter = false;
    arguments.expected_diameter = target_diameter; 

    random_search_thread::run(&arguments, threads);

    t.diameter_green_achievement = listener->on_green_diameter();
    t.delta_achievement = listener->on_delta();

    return true;
}

vector<tuple> generator::pareto_sort(const vector<tuple> &tuples) {

    vector<tuple>::const_iterator tuples_iterator;

    cout << "pareto_sort:" << endl;
    for (tuples_iterator = tuples.begin(); tuples_iterator != tuples.end(); tuples_iterator++) {
        cout << "\t" << tuples_iterator->tostring() << endl;
    }

    vector<tuple> pareto_ordered;
    for (tuples_iterator = tuples.begin(); tuples_iterator != tuples.end(); tuples_iterator++) {
        tuple t = *tuples_iterator;
        t.dominated_by = 0;
        for (vector<tuple>::const_iterator inner = tuples.begin(); inner != tuples.end(); inner++) {
            if (inner->dominates(t)) {
                t.dominated_by++;
            }
        }
        pareto_ordered.push_back(t);
    }

    sort(pareto_ordered.begin(), pareto_ordered.end());

    return pareto_ordered;
}

vector<uint64_t> generator::r_set(const uint64_t &m, const uint64_t &n, const uint64_t &r) {

    vector<uint64_t> s;

    for (uint64_t i = 1; i <= n; i++) {

        uint64_t v = power_modulus(r, i, m);
        if (find(s.begin(), s.end(), v) == s.end()) 
            s.push_back(v);
    }

    sort(s.begin(), s.end());

    return s;
}

void print_generated(const vector<uint64_t> &v) {

    for (vector<uint64_t>::const_iterator it = v.begin(); it != v.end(); it++) {
        cout << *it << " ";
    }
    cout << endl;
}

void print_generated(const vector<vector<uint64_t> > &v) {

    for (vector<vector<uint64_t> >::const_iterator it = v.begin(); it != v.end(); it++) {
        print_generated(*it);
    }
    cout << endl << endl;
}

bool generator::class_exists(const vector<vector<uint64_t> > &classes, const vector<uint64_t> &s) {
    vector<vector<uint64_t> >::const_iterator it;
    for (it = classes.begin(); it != classes.end(); it++) {
        if (std::equal(it->begin(), it->end(), s.begin())) {
            return true;
        }
    }
    return false;
}

vector<uint64_t> generator::maximal_r(const uint64_t &m, const uint64_t &n, uint64_t &max_rset) {
    vector<vector<uint64_t> > classes;
    vector<uint64_t> rs;
    max_rset = 0;

    for (uint64_t r = 2; r <= m; r++) {
        if (power_modulus(r, n, m) == 1) {

            vector<uint64_t> s = r_set(m, n, r);
            if (!s.empty()) {
                uint64_t s_size = (uint64_t)s.size();
                if (max_rset < s_size) {

                    classes.clear();
                    rs.clear();
                    max_rset = s_size;
                    classes.push_back(s);
                    rs.push_back(r);

                } else if (max_rset == s_size) {

                    if (!class_exists(classes, s)) {
                        classes.push_back(s);
                        rs.push_back(r);
                    }

                }
            }
        }
    }

    return rs;
}



