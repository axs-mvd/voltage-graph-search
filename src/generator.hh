#pragma once

#include "graph.hh"
#include "lift.hh"
#include <cmath>
#include <algorithm>

#include "random_search_thread.hh"

using namespace std;

//**add more shit*/
struct tuple {
    uint64_t m;
    uint64_t n;
    uint64_t r;
    uint64_t r_set;

    //evaluation//
    uint64_t m_phi;        //min

    uint64_t diameter_green_achievement; //max
    uint64_t delta_achievement;          //max

    uint32_t dominated_by;

    tuple() {
        m = 0;
        n = 0;
        r = 0;
        m_phi = 0;
        diameter_green_achievement = 0;
        delta_achievement = 0;
        dominated_by = 0;
    }

    bool operator<(const tuple &t) const {
        return dominated_by < t.dominated_by;
    }

    bool dominates(const tuple &t) const {
        uint8_t more = 0;
        
        //if (!pareto_minimize(m_phi, t.m_phi, more)) return false;
        if (!pareto_maximize(diameter_green_achievement, t.diameter_green_achievement, more)) return false;
        if (!pareto_maximize(delta_achievement, t.delta_achievement, more)) return false;

        if (more > 0) return true;

        else return false;
    }

    bool pareto_maximize(const uint64_t &a, const uint64_t &b, uint8_t &more) const{
        if (a >= b) {
            if (a > b) more++;

            return true;
        } 

        return false;
    }
 
   bool pareto_minimize(const uint64_t &a, const uint64_t &b, uint8_t &more) const {
        if (a <= b) {
            if (a < b) more++;

            return true;
        } 

        return false;
    }
    
    string tostring() const {
        stringstream stream;

        stream << "m: " << m << " n: " << n << " r: " << r << " [m - phi(m)]: " << m_phi << " dga: " << diameter_green_achievement << " da: " << delta_achievement;
        return stream.str();
    }
};

class evaluation_rs_listener: public default_rs_listener {
private:
    uint64_t _on_green_diameter;
    uint64_t _on_delta;

public:
    evaluation_rs_listener(const uint64_t &m, const uint64_t &n, const uint64_t &r, 
                        const uint64_t &expected_order, const uint32_t &expected_delta, 
                        const uint32_t &expected_diameter) : 
                        default_rs_listener(m, n, r, expected_order, 
                                            expected_delta, expected_diameter) {

        _on_green_diameter = 0;
        _on_delta = 0;
    }
    
    virtual ~evaluation_rs_listener() {
    }

    uint64_t on_green_diameter() const {
        return _on_green_diameter;
    }
    
    uint64_t on_delta() const {
        return _on_delta;
    }

    virtual void on_begin() {};
    virtual void on_end() {};

    virtual void on_evaluation(const shared_ptr<graph> &gamma, const uint64_t &order, 
                               const uint32_t &delta, const uint32_t &diameter, 
                               const uint32_t &sample_number) {

       pthread_mutex_lock(&_exclude_mutex);

        _total_samples++;
        if (diameter == (_expected_diameter +1)) {
            _on_green_diameter++;
        }
    
        if (delta == _expected_delta) {
            _on_delta++;
        }

        if (delta == _expected_delta && 
                order == _expected_order && 
                diameter == _expected_diameter) { 

           cout << "{" << get_date() << "} " << 
            "[" << _total_samples << "|" << sample_number << 
            "] order: " << order << " " << 
            " degree: " << delta << " " << 
            " diameter: " << diameter <<  endl;
 
            cout << "quotient:" << endl;

            cout << gamma->tostring();
        
            cout << "group:" << endl;
        
            cout << "\tm: " << _m << endl;
            cout << "\tn: " << _n << endl;
            cout << "\tr: " << _r << endl;
        }  
    
        pthread_mutex_unlock(&_exclude_mutex);
    }
};

class generator {

public:

    static vector<tuple> start(const uint64_t &m_a, const uint64_t &m_b, const uint64_t &n_a, const uint64_t &n_b,
                      const uint64_t &order_lower_bound, const uint32_t &target_delta, 
                      const uint32_t &target_diameter, const quotient_parameters &q, 
                      const uint32_t &eval_samples, const uint32_t &threads);
private:

    static uint64_t power_modulus(uint64_t base, uint64_t exponent, uint64_t modulus);
    static uint64_t gcd(uint64_t a, uint64_t b);
    static uint64_t phi(const uint64_t &m);

    static bool evaluate_tuple(tuple &t, const quotient_parameters &q, 
                               const uint64_t &order_lower_bound, const uint32_t &target_delta, 
                               const uint32_t &target_diameter, const uint32_t &eval_samples, 
                               const uint32_t &threads);

    static vector<tuple> pareto_sort(const vector<tuple> &tuples);
    
    static vector<uint64_t> maximal_r(const uint64_t &m, const uint64_t &n, uint64_t &max_rset);
    static vector<uint64_t> r_set(const uint64_t &m, const uint64_t &n, const uint64_t &r);

    static bool class_exists(const vector<vector<uint64_t> > &classes, const vector<uint64_t> &s);
};

