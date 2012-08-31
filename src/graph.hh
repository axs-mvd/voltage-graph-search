#pragma once

#include <string>
#include <sstream>

#include <vector>
#include <tr1/memory>
#include "edge.hh"
#include "group.hh"


using namespace std;
using namespace std::tr1;

class graph {
private:
   
    typedef vector<shared_ptr<group_element> > alpha_set;

    alpha_set **_adjacency_matrix; 
    shared_ptr<group> _g;
    uint32_t _size;
 
public:

    static const vertex null_vertex;

    graph(uint32_t size, shared_ptr<group> g);

    virtual ~graph();

    void add_edge(const edge &e);

    uint32_t size() const;

    vector<vertex>        vertices() const;
    vector<shared_ptr<group_element> > alphas(const vertex &u, const vertex &v) const;
    vector<vertex>        adjacents(const vertex &u) const;

    void vertices(vector<vertex> &v) const;

    void alphas(const vertex &u, const vertex &v, 
                vector<shared_ptr<group_element> > &a) const;

    void adjacents(const vertex &u, vector<vertex> &v) const;

    string tostring() const;

private:

   alpha_set *get(vertex i, vertex j) const {
        return _adjacency_matrix[i*_size + j];
    }

    void set(vertex i, vertex j, alpha_set *alpha) {
        _adjacency_matrix[i*_size + j] = alpha;
    }

    alpha_set filter_inverses(const alpha_set &in) const {
        alpha_set filtered;
        for (alpha_set::const_iterator alpha = in.begin(); alpha != in.end(); alpha++) {
            shared_ptr<group_element> element = *alpha;
            shared_ptr<group_element> inverse = _g->inverse(element);

            bool found = false;
            for (alpha_set::iterator it = filtered.begin(); it != filtered.end(); it++) {
                if (inverse->equals(*it)) found = true;
            }

            if (!found) filtered.push_back(element);
        }
        return filtered;
    }
};

