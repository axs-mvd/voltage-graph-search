#include <iostream>
#include <fstream>
#include <stdint.h>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <cstring>
#include "zmxzn.hh"

struct tuple {
    uint64_t m;
    uint64_t n;
    uint64_t phi;
    uint64_t r_count;

    bool dominates(const tuple &t) {
        
        uint8_t score_a = 0;
        uint8_t score_b = 0;
        if (phi >= t.phi) score_a++;
        else if (phi <= t.phi) score_b++;

        if (r_count >= t.r_count) score_a++;
        else if (r_count <= t.r_count) score_b++;

        if (score_a == 2) return true;
        else if (score_a == 1 && score_b == 0) return true;
        else return false;
    }
    
    string tostring() {
        stringstream stream;

        stream << "m: " << m << " n: " << n << " phi: " << phi << " r_count: " << r_count;
        return stream.str();
    }
};


uint64_t gcd(uint64_t a, uint64_t b);
uint64_t phi(const uint64_t &m);
vector<uint64_t> generate_r_candidates(uint64_t m, uint64_t n);
uint64_t center_size(const shared_ptr<group> &g);
uint64_t moore_bound(uint64_t delta, uint64_t k);
void analyze_tuple(uint64_t m, uint64_t n, uint64_t r, uint64_t delta, uint64_t diameter, uint64_t vertex_count);

uint64_t power_modulus(uint64_t base, uint64_t exponent, uint64_t modulus) {
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

void test_tuple() {
//    uint64_t m = 5003;
    uint64_t m = 341057;
    //uint64_t n = phi(m);
    uint64_t n = 73;
    //////////////////////////////////////////////
    //    cout << "r^n mod m " << power_modulus(2103, 73, 341057) << endl;
    //////////////////////////////////////////////

    cout << "m: " << m << " n: " << n << endl;

    vector<uint64_t> rcandidates = generate_r_candidates(m, n);

    cout << "there are " << rcandidates.size() << " r candidates" << endl;

    vector<uint64_t> rs;
    for (vector<uint64_t>::iterator it = rcandidates.begin(); it != rcandidates.end(); it++) {
        uint64_t r = *it;

        cout << "r: " << r << endl;
        shared_ptr<group> g(new zmxzn(m, n, r));

        uint64_t cs = center_size(g);

        cout << "for m = " << m << " n = " << n << " r = " << r << " center has size: " << cs << endl;

        if (cs == 0) rs.push_back(r);

    }

    cout << "group candidates: " << endl;

    for (vector<uint64_t>::iterator it = rs.begin(); it != rs.end(); it++) {
        cout << "\t m: " << m << " n: " << n << " r: " << *it << endl;
    }
}

void test_analyze_tuple() {
    uint64_t m = 341057;
    uint64_t n = 73;
    uint64_t r = 298391;

    uint64_t delta    = 8;
    uint64_t diameter = 10;
    uint64_t vertex_count = 1;

    analyze_tuple(m, n, r, delta, diameter, vertex_count);
}

void analyze_tuples(string filename) {

    ifstream stream(filename.c_str(), ifstream::in);
   
    while (!stream.eof()) { 
        uint64_t m, n, r, delta, diameter, vertex_count;

        stream >> delta;
        stream >> diameter;
        stream >> m;
        stream >> n;
        stream >> r;
        stream >> vertex_count;

        if (!stream.eof()) {
            //cout << "m: " << m << " n: " << n << " delta: " << delta << " diameter: " << diameter << " vertex_count: " << vertex_count << endl;
            analyze_tuple(m, n, r, delta, diameter, vertex_count);
        }
    }
}

vector<tuple> generate_tuples(uint64_t m, uint64_t n, 
        double increase_factor) {

    uint64_t m_a = m + 1;
    uint64_t m_b = (uint64_t)(m * increase_factor);

    uint64_t n_a = n;
    uint64_t n_b = (uint64_t)(n * increase_factor)+2;

    //pareto front of tuples in phi an r_count
    vector<tuple> tuples;

    cout << "m_a: " << m_a << " m_b: " << m_b << endl;
    cout << "n_a: " << n_a << " n_b: " << n_b << endl;

    tuple source;
    source.m = m;
    source.n = n;
    source.phi = phi(m);
    source.r_count = (uint64_t)generate_r_candidates(source.m, source.n).size();

    cout << "starting from: " << source.tostring() << endl;
    tuples.push_back(source);

    for (uint64_t m_cand = m_a; m_cand <= m_b; m_cand++) {
        for (uint64_t n_cand = n_a; n_cand <= n_b; n_cand++) {
            tuple t;
            t.m = m_cand;
            t.n = n_cand;
            t.phi = phi(t.m);
            t.r_count = (uint64_t)generate_r_candidates(t.m, t.n).size();

            cout << "evaluating: " << t.tostring() << endl;
           
            bool not_dominated = true;
            vector<tuple>::iterator it;

            for (it = tuples.begin(); it != tuples.end(); it++) {
                if (it->dominates(t)) {
                    not_dominated = false;
                }
            }

            for (it = tuples.begin(); it != tuples.end();) {
                if (t.dominates(*it)) {
                    it = tuples.erase(it);
                } else {
                    it++;
                }
            }

            if (not_dominated) {
                cout << "not dominated! tuples = " << tuples.size() << endl;
                tuples.push_back(t);
                cout << "tuples: " << endl;

                for (it = tuples.begin(); it != tuples.end(); it++) {
                    cout << it->tostring() << endl;
                }
            }
        }
    }

    return tuples;
}

void test_generate_tuples() {
    //analyze_tuples("tuples");
//    uint64_t order = 24897161;
/*    uint64_t m = 341057;
    uint64_t n = 73;
    double increase_factor = 1.05;
*/
    uint64_t m = 923;
    uint64_t n = 21;
    double increase_factor = 1.02;

    vector<tuple> tuples = generate_tuples(m, n, increase_factor);

    cout << "tuples size: " << tuples.size() << endl;
    for (vector<tuple>::iterator tuple = tuples.begin(); tuple != tuples.end(); tuple++) {
       cout << tuple->tostring() << endl;
        vector<uint64_t> r_cand = generate_r_candidates(tuple->m, tuple->n);
        cout << "r:\t" << endl;
        for (vector<uint64_t>::iterator r_iter = r_cand.begin(); r_iter != r_cand.end(); r_iter++) {
            cout << *r_iter << " ";
        }
        cout << endl;
    }

}


int main() {

    test_generate_tuples();
/*
    vector<uint64_t> r_cand = generate_r_candidates(358051, 77);
    cout << endl;
    for (vector<uint64_t>::iterator r_iter = r_cand.begin(); r_iter != r_cand.end(); r_iter++) {
        cout << *r_iter << " ";
    }
    cout << endl;
    return 0;
*/
}

vector<uint64_t> generate_r_candidates(uint64_t m, uint64_t n) {
    vector<uint64_t> potencial_r; 

    for (uint64_t i = 1; i < n; i++) {
        if (power_modulus(i, n, m) == 1) {
            potencial_r.push_back(i);
        }
    }
    return potencial_r;
}

//Stein algorithm from: http://en.wikipedia.org/wiki/Binary_GCD_algorithm
uint64_t gcd(uint64_t u, uint64_t v) {
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
uint64_t phi(const uint64_t &m) {
    uint64_t count = 0;
    for (uint64_t i = 1; i < m; i++) {
        if (gcd(i, m) == 1) count++;
    }
    return count;
}

uint64_t center_size(const shared_ptr<group> &g) {
    uint64_t size = g->size();
    uint64_t center_count = 0;
    shared_ptr<group_element> e = g->neutral();
    for (uint64_t i = 0; i < size; i++) {
        shared_ptr<group_element> a = g->get_element(i);
        if (a != e) {
            bool all_ok = true;
            for (uint64_t j = 0; j < size && all_ok; j++) {
                shared_ptr<group_element> b = g->get_element(i);
                if (i != j && (b != e)) {
                    shared_ptr<group_element> x = g->operation(a, b);
                    shared_ptr<group_element> y = g->operation(b, a);

                    all_ok = (x == y);
                }
            }
            if (all_ok) center_count++;
        }
    }
    return center_count;
}

uint64_t moore_bound(uint64_t delta, uint64_t k) {

    uint64_t sum = 0;
    for (uint64_t i = 1; i < k; i++) {
        sum += (uint64_t)pow((long double)delta -1, (long double)i); 
    }
    return 1 + delta * sum;
}

void analyze_tuple(uint64_t m, uint64_t n, uint64_t r,  
        uint64_t delta, uint64_t diameter, uint64_t vertex_count) {

    vector<uint64_t> r_cand = generate_r_candidates(m, n);

    shared_ptr<group> g(new zmxzn(m, n, r));

    cout << m << " " << n << " " << delta << " " << diameter << " " << vertex_count << " " << m*n*vertex_count << " " 
        << phi(m) << " " << r_cand.size() << " " << (double) n / (double) m 
        << " " << (double) (m*n*vertex_count) / (double)moore_bound(delta, diameter) << " " << center_size(g) << endl;
}

