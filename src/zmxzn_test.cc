#include "zmxzn.hh"
#include "zn.hh"

#include <iostream>
#include <cstdlib>
#include <ctime>


using namespace std;

bool test_inverse(shared_ptr<group> g) {
    shared_ptr<group_element> x = g->random_element();

    cout << "random: " << x->tostring() << endl;
    
    shared_ptr<group_element> x_inverse = g->inverse(x);

    cout << " inverse: " << x_inverse->tostring() << endl;

    shared_ptr<group_element> y = g->operation(x, x_inverse);

    cout << " sum: " << y->tostring() << endl;

    return y->equals(g->neutral());
}

void test_inverse_loop() {
    uint32_t m = 33;
    uint32_t n = 20;
    uint32_t r = 2;

    cout << "m: " << m << " n: " << n << " r: " << r << endl;

    shared_ptr<group> g(new zmxzn(m, n, r));
   
    for (int i = 0; i < 1000000; i++) { 
        if (!test_inverse(g)) {
            cout << "epic fail" << endl;
        }
    }
    cout << "epic win" << endl;
}

void test_inverse_195_34() {
    uint32_t m = 333;
    uint32_t n = 36;
    uint32_t r = 2;

    cout << "m: " << m << " n: " << n << " r: " << r << endl;

    shared_ptr<group> g(new zmxzn(m, n, r));

    shared_ptr<group_element> x(new zmxzn_member(195,34));

    cout << "x: " << x->tostring() << endl;

    shared_ptr<group_element> y = g->inverse(x);

    cout << "y: " << y->tostring() << endl;

    shared_ptr<group_element> z = g->operation(x, y);
    
    cout << "z: " << z->tostring() << endl;
}

void test_pow_modulo() {
    uint32_t a = 2;
    uint32_t b = 34;
    uint32_t n = 333; 

    uint64_t temp = (uint64_t)pow((long double)a, (long double)b);
    cout << "temp: " << temp % n << endl;
}


void test_involutives(shared_ptr<group> g) {
    vector<shared_ptr<group_element> > involutives = g->get_involutives();

    vector<shared_ptr<group_element> >::iterator it; 
    for (it = involutives.begin(); it != involutives.end(); it++) {
        if (!g->is_involutive(*it)) {
            cout << (*it)->tostring() << " is not involutive!" << endl;
        }
    }
    cout << "generated: " << involutives.size() << endl;
}

void test_involutives_zn(uint64_t n) {
    shared_ptr<group> g(new zn(n));
    test_involutives(g);
}

void test_involutives_zmxzn(uint64_t m, uint64_t n, uint64_t r) {
    shared_ptr<group> g(new zmxzn(m, n, r));
    test_involutives(g);
}

void generate_m_n_r(uint64_t &m, uint64_t &n, uint64_t &r) {
    bool done = false;
    while (!done) {
        m = n = 0;
        while (m == 0) m = rand() % 500;
        while (n == 0) n = rand() % 500;
        for (uint64_t i = n-1; (!done) && i >= 1; i--) {
            if (zmxzn::power_modulus(i, n, m) == 1) {
                r = i;
                done = true;
            }
        }
    }
}

void loop_random_test_zmxzn() {
    while (true) {
        uint64_t m, n, r;
        m = n = r = 0;
        generate_m_n_r(m, n, r);
        cout << "m: " << m << " n: " << n << " r: " << r << endl;
        test_involutives_zmxzn(m, n, r);
        sleep(1);
    }
}

void loop_random_test_zn() {
    while (true) {
        uint64_t n = 0;
        while (n == 0) n = rand() % 500;

        test_involutives_zn(n);
        sleep(1);
    }
}

int main() {
    srand(time(0));
//    loop_random_test_zn();
    loop_random_test_zmxzn();
//    test_pow_modulo();
//    test_inverse_195_34();
//    test_inverse_loop();
}

