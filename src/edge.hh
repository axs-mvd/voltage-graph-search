#pragma once

#include <tr1/memory>

#include <string>
#include <sstream>

#include "group.hh"

using namespace std;
using namespace std::tr1;

typedef int vertex;

class edge {
public:
    vertex _source;
    vertex _destination;
    shared_ptr<group_element> _alpha;
    
    edge(const vertex &source, 
         const vertex &destination, 
         shared_ptr<group_element> alpha):_alpha(alpha) {
        _source = source;
        _destination = destination;
    }

    edge(const edge &e) : _alpha(e._alpha){
        _source = e._source;
        _destination = e._destination;
    }

    virtual ~edge() {
    }
    
    edge &operator=(const edge &e) {

        if (this != &e) {
            _source = e._source;
            _destination = e._destination;
            _alpha = e._alpha;
        }
        return *this;
    }

    bool operator==(const edge &e) const {
        return (_source == e._source && _destination == e._destination && _alpha->equals(e._alpha));
    }
  
    vertex source() const {
        return _source;
    } 
     
    vertex destination() const {
        return _destination;
    } 

    shared_ptr<group_element> alpha() const {
        return _alpha;
    }

    string tostring() const {
        stringstream stream;
        stream << "[" << _source << ", " << _destination << "] " << _alpha->tostring();
        return stream.str();
    }

    static edge make_edge(const vertex &source, 
                          const vertex &destination, 
                          group_element  *alpha) {
        shared_ptr<group_element> t(alpha);
        edge e(source, destination, t);
        return e;
    }


    static edge make_edge(const vertex &source, 
                          const vertex &destination, 
                          shared_ptr<group_element>  alpha) {

        edge e(source, destination, alpha);
        return e;
    }

};


