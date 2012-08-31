#pragma once
#include "graph.hh"
#include <cstdlib>
#include <cmath>
#include <sstream>
#include <iostream>

using namespace std;

class zmxzn;

class zmxzn_member: public group_element {
private:
    uint64_t _value_m;
    uint64_t _value_n;
public:
    zmxzn_member(uint64_t value_m, uint64_t value_n) {
        _value_m = value_m;
        _value_n = value_n;
    }

    zmxzn_member(const zmxzn_member &m) :group_element() {
        _value_m = m._value_m;
        _value_n = m._value_n;
    }

    virtual ~zmxzn_member() {
    }

    group_element &operator=(const group_element &m) {
        if (this != &m) {
            _value_m = ((zmxzn_member *)&m)->_value_m;
            _value_n = ((zmxzn_member *)&m)->_value_n; } return *this;
    }   
    
    bool operator==(const group_element &m) const {
        if (this == &m) return true;
        return _value_m == ((zmxzn_member *)&m)->_value_m &&
               _value_n == ((zmxzn_member *)&m)->_value_n;
    }

    bool operator<(const group_element &m) const {
        if (this == &m) return false;
        uint64_t m_value_m = ((zmxzn_member *)&m)->_value_m;
        uint64_t m_value_n = ((zmxzn_member *)&m)->_value_n;
        
        if (_value_m != m_value_m) return _value_m < m_value_m; 
        else return _value_n < m_value_n;
    }

    bool operator>(const group_element &m) const {
        if (this == &m) return false;

        uint64_t m_value_m = ((zmxzn_member *)&m)->_value_m;
        uint64_t m_value_n = ((zmxzn_member *)&m)->_value_n;
 
        if (_value_m != m_value_m) return _value_m > m_value_m; 
        else return _value_n > m_value_n;
    }

    string tostring() const {
        stringstream stream;
        stream << "(" <<  _value_m << ", " << _value_n << ")";
        return stream.str();
    }


    friend class zmxzn;
};

class zmxzn : public group {
private:
    uint64_t _m;
    uint64_t _n;
    uint64_t _r;
    uint64_t _inverted_r;

    uint64_t *power_modulus_cache;

public:
    zmxzn(uint64_t m, uint64_t n, uint64_t r) {
        _m = m;
        _n = n;
        _r = r;
        _inverted_r = _m + 1;
        for (uint64_t i = 0; i < _m; i++) {
            if ((_r * i) % _m == 1) {
                _inverted_r = i;
            }
        }

        power_modulus_cache = new uint64_t[_n];
        for (uint64_t i = 0; i < _n; i++) {
            power_modulus_cache[i] = _m;
        }

        if (_inverted_r == _m + 1) {
            cout << "[zmxzn] could not find inverse for r: " << _r << endl;
        }
    }

    virtual ~zmxzn() {
        delete [] power_modulus_cache;
    }

    virtual shared_ptr<group_element> random_element() const {
        uint64_t random_m;
        uint64_t random_n;

        random_m = rand() % _m;
        random_n = rand() % _n;

        shared_ptr<group_element> x(new zmxzn_member(random_m, random_n));
        return x;
    }

    virtual uint64_t size() const {
        return _m * _n;
    }

    virtual uint64_t index_of(shared_ptr<group_element> a) const {
        zmxzn_member *ga = (zmxzn_member *)a.get();
        return ga->_value_m * _n + ga->_value_n;
    }

    virtual shared_ptr<group_element> get_element(uint64_t index) const {
        uint64_t a = index / _n;
        uint64_t b = index % _n;

        shared_ptr<group_element> x(new zmxzn_member(a, b));
        return x;
    }

    virtual shared_ptr<group_element> neutral() const {
        shared_ptr<group_element> e(new zmxzn_member(0,0));
        return e;
    }

    //(a,b)(c,d) = (a + (r^b)*c, b + d)
    virtual shared_ptr<group_element> operation(shared_ptr<group_element> one, 
                           shared_ptr<group_element> other) const {

        uint64_t a = ((zmxzn_member *)one.get())->_value_m;
        uint64_t b = ((zmxzn_member *)one.get())->_value_n;

        uint64_t c = ((zmxzn_member *)other.get())->_value_m;
        uint64_t d = ((zmxzn_member *)other.get())->_value_n;

        if (power_modulus_cache[b] == _m) {
            power_modulus_cache[b] = power_modulus(_r, b, _m);
        }

        uint64_t r_b = power_modulus_cache[b];
//        uint64_t r_b = power_modulus(_r, b, _m);

        uint64_t r_b_c = r_b*c;

        uint64_t i = (a + r_b_c) % _m;
        uint64_t j = (b + d) % _n;
        
        shared_ptr<group_element> x(new zmxzn_member(i,j));
        return x;
    }

    virtual shared_ptr<group_element> inverse(shared_ptr<group_element> one) const {

        uint64_t a = ((zmxzn_member *)one.get())->_value_m;
        uint64_t b = ((zmxzn_member *)one.get())->_value_n;

        uint64_t inverted_r_b = power_modulus(_inverted_r, b, _m);

        uint64_t c = ((_m - a) * inverted_r_b) % _m;
   
        uint64_t d = (_n - b) % _n;

        shared_ptr<group_element> inverse(new zmxzn_member(c, d));
        return inverse;
    }

    vector<shared_ptr<group_element> > get_involutives() const {
        vector<shared_ptr<group_element> > involutives;
        if (_n % 2 == 0) {
            uint64_t b = _n / 2;            
            uint64_t r_b = power_modulus(_r, b, _m);
            for (uint64_t i = 0; i < _m; i++) {
                if (i*(1 + r_b) % _m == 0) {
                    shared_ptr<group_element> x(new zmxzn_member(i, b));
                    involutives.push_back(x);
                }
            }
        } 
        
        if (_m % 2 == 0) {
            shared_ptr<group_element> x1(new zmxzn_member(_m / 2, 0));
            involutives.push_back(x1);
        }

        return involutives;
    }

public:
    
    static uint64_t power_modulus(uint64_t base, uint64_t exponent, uint64_t modulus) {
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
};


