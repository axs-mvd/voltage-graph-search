#pragma once

#include "lift.hh"
#include "graph.hh"
#include "zmxzn.hh"

#include <tr1/memory>

using namespace std;
using namespace std::tr1;

class quotient_factory {

public:
    static shared_ptr<graph> create_random_quotient(const bool &is_bouquet, 
                                             const uint32_t &k,
                                             const uint32_t &s,
                                             const uint32_t &l,
                                             const uint32_t &e,
                                             const shared_ptr<group> &g) {
        if (is_bouquet) {
            return create_random_bouquet(s, l, g); 
        } else {
            return create_random_complete(k, l, e, g);
        }
    }

private:

    static vector<uint64_t> random_distinct_elements(const uint32_t &how_many, 
            const uint64_t &max) {
        vector<uint64_t> randoms;
        for (uint32_t i = 0; i < how_many; i++) {
            uint64_t rand_value = rand() % max;
            while (find(randoms.begin(), randoms.end(), rand_value) != randoms.end()) {
                rand_value = rand() % max;
            }
            randoms.push_back(rand_value);
        }
        return randoms;
    }

    static vector<uint64_t> get_involutives(const shared_ptr<group> &g, 
            const uint32_t &s) {
        vector<uint64_t> involutives_idx;
        vector<shared_ptr<group_element> > involutives = g->get_involutives();
        if (involutives.size() < s) {
            throw involutives_exception();
        } else {
            for (uint32_t i = 0; i < s; i++) {
                bool done = false;
                while (!done) {
                    int idx = rand() % involutives.size();
                    shared_ptr<group_element> e = involutives[idx];
                    uint64_t element_idx = g->index_of(e);
                    if (find(involutives_idx.begin(), involutives_idx.end(), element_idx) == involutives_idx.end()) {
                        done = true;
                        involutives_idx.push_back(element_idx);
                    }
                }
            }
        }
        return involutives_idx;
    }

    static shared_ptr<group> create_group(const uint64_t &m, const uint64_t &n, const uint64_t &r)  {
        shared_ptr<group> g(new zmxzn(m, n, r));
        return g;
    }

    static void create_random_edges_between(shared_ptr<graph> &gamma, const shared_ptr<group> &g, 
            const vertex &u, const vertex &v, const uint32_t &e) {

        vector<uint64_t> random = random_distinct_elements(e, g->size());
        for (vector<uint64_t>::iterator it = random.begin(); 
                it != random.end(); it++) {
            gamma->add_edge(edge::make_edge(u, v, g->get_element(*it)));
        }
    }

    static shared_ptr<graph> create_random_bouquet(const uint32_t &s, const uint32_t &l, const shared_ptr<group> &g) {
        shared_ptr<graph> gamma(new graph(1, g));
        vertex u = 0;

        vector<uint64_t> involutives = get_involutives(g, s);

        for (vector<uint64_t>::iterator it = involutives.begin(); 
                it != involutives.end(); it++) {
            gamma->add_edge(edge::make_edge(u, graph::null_vertex, g->get_element(*it)));
        }

        create_random_edges_between(gamma, g, u, u, l);

        return gamma;
    }


    static shared_ptr<graph> create_random_complete(const uint32_t &k, const uint32_t &l, const uint32_t &e, const shared_ptr<group> &g) {

        shared_ptr<graph> gamma(new graph(k, g));

        //loops first
        for (uint32_t i = 0; i < k; i++) {
            create_random_edges_between(gamma, g, i, i, l);
        }

        //now the edges between them
        for (uint32_t i = 0; i < k; i++) {
            for (uint32_t j = i + 1; j < k; j++) {
                create_random_edges_between(gamma, g, i, j, e);
            }
        }

        return gamma;
    }

};
