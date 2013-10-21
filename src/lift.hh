#pragma once

#include <vector>
#include <tr1/memory>
#include <limits> 

#include "graph.hh"

using namespace std;
using namespace std::tr1;

class lift_vertex {
private: 
    vertex _v;
    shared_ptr<group_element> _g;

public:

    lift_vertex(vertex v, shared_ptr<group_element> g) {
        _v = v;
        _g = g;
    }

    lift_vertex(const lift_vertex &l) {
       _v = l._v;
       _g = l._g;
    }

    virtual ~lift_vertex() {
    }

    bool operator==(const lift_vertex &l) const {
        if (this == &l) return true;
        else return (_v == l._v && _g->equals(l._g));
    }

    lift_vertex &operator=(const lift_vertex &l) {
        if (this != &l) {
            _v = l._v;
            _g = l._g;
        }
        return *this;
    }

    bool operator<(const lift_vertex &l) const {
        if (this == &l) {
            return false;
        } else { 
            if (_v == l._v) {
                return *_g < *(l._g);
            } else {
                return _v < l._v;
            }
        }
    }

    bool operator>(const lift_vertex &l) const {
        if (this == &l) {
            return false;
        } else {
            if (_v == l._v) {
                return *_g > *(l._g);
            } else {
                return _v > l._v;
            }
        }
    }

    vertex v() const {
        return _v;
    }

    shared_ptr<group_element> g() const {
        return _g;
    }

};

class lift {
private:
    shared_ptr<graph> _gamma;
    shared_ptr<group> _group;

public:
    lift(shared_ptr<graph> gamma, shared_ptr<group> group) {
        _gamma = gamma;
        _group = group;
    }

    lift(const lift &l) {
        _gamma = l._gamma;
        _group = l._group;
    }
    
    lift &operator=(const lift &l) {
        if (this != &l) {
            _gamma = l._gamma;
            _group = l._group;
        }

        return *this;
    }

    void adjacents(const lift_vertex &u_g, vector<lift_vertex> &adjacents) const;

    vector<lift_vertex> adjacents(const lift_vertex &u_g) const;

    int delta() const;

    int diameter() const;

    bool check_diameter(const int &expected_diameter) const; 

    uint64_t size() const {
        return _group->size() * _gamma->size();
    }

    int max_distance(const lift_vertex &u) const;

    bool check_distance(const lift_vertex &u, const int &distance) const;
    
    bool check_distance(int &distance) const;
    
    uint64_t index_of(const lift_vertex &u) const {
        return u.v() * _group->size() + _group->index_of(u.g());
    }

    lift_vertex get_vertex(uint64_t index) const {
        uint64_t u = index / _group->size();
        uint64_t group_index = index % _group->size();
        shared_ptr<group_element> g = _group->get_element(group_index);
        
        lift_vertex u_g = lift_vertex(u,g);
        return u_g;
    }

private:

    int bfs(const lift_vertex &u, const int &distance) const;

};
