#include "graph.hh"
#include <cstdlib>
#include <sstream>

class zn;

class zn_member: public group_element {
private:
    uint32_t _value;
public:
    zn_member(uint32_t value) {
        _value = value;
    }

    zn_member(const zn_member &m): group_element() {
        _value = m._value;
    }

    virtual ~zn_member() {
    }

    group_element &operator=(const group_element &m) {
        if (this != &m) {
            _value = ((zn_member *)&m)->_value;
        }
        return *this;
    }   
    
    bool operator==(const group_element &m) const {
        if (this == &m) return true;
        return _value == ((zn_member *)&m)->_value;
    }

    bool operator<(const group_element &m) const {
        if (this == &m) return false;
        return _value < ((zn_member *)&m)->_value;
    }

    bool operator>(const group_element &m) const {
        if (this == &m) return false;
        return _value > ((zn_member *)&m)->_value;
    }

    string tostring() const {
        stringstream stream;
        stream << _value;
        return stream.str();
    }

    friend class zn;
};

class zn : public group { 
private:
    uint32_t _n;
public:
    zn(uint32_t n) {
        _n = n;
    }

    virtual ~zn() {
    } 
   
    virtual shared_ptr<group_element> random_element() const {
        shared_ptr<group_element> x(new zn_member(rand() % _n));
        return x;
    }

    virtual uint64_t size() const {
        return _n;
    }

    virtual shared_ptr<group_element> neutral() const {
        shared_ptr<zn_member> e(new zn_member(0));
        return e;
    }

    virtual shared_ptr<group_element> operation(shared_ptr<group_element> a, 
                           shared_ptr<group_element> b) const {

        zn_member *ga = (zn_member *)a.get();
        zn_member *gb = (zn_member *)b.get();
        
        shared_ptr<group_element> x(new zn_member((ga->_value + gb->_value) % _n));
        return x;
    }
    
    virtual uint64_t index_of(shared_ptr<group_element> a) const {
        zn_member *ga = (zn_member *)a.get();
        return ga->_value;
    }

    virtual shared_ptr<group_element> get_element(uint64_t index) const {
        shared_ptr<zn_member> x(new zn_member(index % _n));
        return x;
    }

    virtual shared_ptr<group_element> inverse(shared_ptr<group_element> a) const {
        zn_member *ga = (zn_member *)a.get();
        uint32_t inverse_value = (_n - ga->_value) %_n;
        shared_ptr<zn_member> inverse(new zn_member(inverse_value));
        return inverse;
    }

    virtual vector<shared_ptr<group_element> > get_involutives() const {
        vector<shared_ptr<group_element> > involutives;
        if (_n % 2 == 0) {
            shared_ptr<zn_member> x(new zn_member(_n / 2));
            involutives.push_back(x);
        }

        return involutives;
    }
};


