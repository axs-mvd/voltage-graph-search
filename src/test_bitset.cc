#include <iostream>
#include "big_bitset.hh"
using namespace std;
int main() {

    big_bitset bs(7041746081);

    bs.set(18);
    bs.set(40);
    bs.set(11);
    bs.set(2050103983);

    cout << endl << endl;
    for (uint64_t i = 0; i < bs.size(); i++) {
        if (bs.get(i)) {
            cout << i << endl;
        } 
    }
    cout << endl; 
}

