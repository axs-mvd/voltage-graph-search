#pragma once

#include <cstdlib>
#include <exception>
#include <limits>
#include <ctime>
#include <tr1/memory>
#include <pthread.h>
#include <vector>
#include <algorithm>

#include "lift.hh"
#include "graph.hh"
#include "zmxzn.hh"

using namespace std;
using namespace std::tr1;

class involutives_exception: public exception {};

string get_date();

struct group_parameters {
    uint64_t m;
    uint64_t n;
    uint64_t r;
};

struct quotient_parameters {
    bool is_bouquet;
    uint32_t s; //semi-edges
    uint32_t k; //complete graph size
    uint32_t l; //loops
    uint32_t e; //edges between any pair of vertices
};

//forward declaration
class rs_listener;

struct rs_arguments{
    group_parameters g;
    quotient_parameters q;
    uint32_t samples;
    bool only_check_diameter;
    uint32_t expected_diameter;

    shared_ptr<rs_listener> listener;
};

class rs_listener {
public:

    rs_listener(){};
    virtual ~rs_listener(){};

    virtual void on_begin() = 0;

    virtual void on_evaluation(const shared_ptr<graph> &gamma, 
                 const uint64_t &order, const uint32_t &delta, 
                 const uint32_t &diameter, const uint32_t &sample_number) = 0;

    virtual void on_check(const shared_ptr<graph> &gamma, 
                 const uint64_t &order, const uint32_t &delta, 
                 const bool &is_valid, const uint32_t &diameter,
                 const uint32_t &sample_number) = 0;


    virtual void on_end() = 0;
};

class default_rs_listener : public rs_listener{
protected:
    pthread_mutex_t _exclude_mutex;

    uint64_t _total_samples;

    uint64_t _m;
    uint64_t _n;
    uint64_t _r;

    uint64_t _expected_order;
    uint32_t _expected_delta;
    uint32_t _expected_diameter;

public:

    default_rs_listener(const uint64_t &m, const uint64_t &n, const uint64_t &r, 
                        const uint64_t &expected_order, const uint32_t &expected_delta, 
                        const uint32_t &expected_diameter): rs_listener() {

        pthread_mutex_init(&_exclude_mutex, 0);
        
        _m = m;
        _n = n;
        _r = r;

        _expected_order = expected_order;
        _expected_delta = expected_delta;
        _expected_diameter = expected_diameter;
        _total_samples = 0;
    }

    virtual ~default_rs_listener() {
        pthread_mutex_destroy(&_exclude_mutex);
    }

    virtual void on_begin() {}
    virtual void on_end() {}

    virtual void on_evaluation(const shared_ptr<graph> &gamma, const uint64_t &order, 
                               const uint32_t &delta, const uint32_t &diameter, 
                               const uint32_t &sample_number) {

        string message_ok = "[ok]";
        string message_fail = "[fail]";

        string expected_delta_message = message_fail;
        if (_expected_delta== delta) expected_delta_message = message_ok;

        string expected_order_message = message_fail;
        if (_expected_order == order) expected_order_message = message_ok;

        string expected_diameter_message = message_fail;
        if (_expected_diameter == diameter) expected_diameter_message = message_ok;

        pthread_mutex_lock(&_exclude_mutex);

        _total_samples++;

        if (delta == _expected_delta && 
                order == _expected_order && 
                diameter == _expected_diameter) { 

            cout << "{" << get_date() << "} " << 
                "[" << _total_samples << "|" << sample_number << 
                "] order: " << order << " " << expected_order_message <<
                " degree: " << delta << " " << expected_delta_message <<
                " diameter: " << diameter << " " << expected_diameter_message << endl;

            cout << "quotient:" << endl;

            cout << gamma->tostring();

            cout << "group:" << endl;
        
            cout << "\tm: " << _m << endl;
            cout << "\tn: " << _n << endl;
            cout << "\tr: " << _r << endl;
        }  
    
        pthread_mutex_unlock(&_exclude_mutex);
    }

    virtual void on_check(const shared_ptr<graph> &gamma, const uint64_t &order, 
                               const uint32_t &delta, const bool &is_valid, 
                               const uint32_t &diameter, const uint32_t &sample_number) {

        string message_ok = "[ok]";
        string message_fail = "[fail]";

        string expected_delta_message = message_fail;
        if (_expected_delta== delta) expected_delta_message = message_ok;

        string expected_order_message = message_fail;
        if (_expected_order == order) expected_order_message = message_ok;

        string expected_diameter_message = message_fail;
        if (is_valid) expected_diameter_message = message_ok;

        pthread_mutex_lock(&_exclude_mutex);

        _total_samples++;

        if (delta == _expected_delta && 
                order == _expected_order && 
                is_valid) { 
            cout << "{" << get_date() << "} " << 
                "[" << _total_samples << "|" << sample_number << 
                "] order: " << order << " " << expected_order_message <<
                " degree: " << delta << " " << expected_delta_message <<
                " diameter: " << diameter << " " << expected_diameter_message << endl;

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

class random_search_thread {
public:
    static pthread_t start(rs_arguments *args);
    static void run(rs_arguments *args, const uint32_t &thread_count);

private:
    static void *routine(void *args);

};

