#pragma once
#include <string>
#include <tr1/memory>
#include <vector>
#include <stdint.h>

using namespace std;
using namespace std::tr1;

class group_element {
public:
    group_element(){};
    virtual ~group_element(){};
    virtual bool operator==(const group_element& a) const = 0;
    virtual bool  operator>(const group_element& a) const = 0;
    virtual bool  operator<(const group_element& a) const = 0; 
    virtual string tostring() const = 0;

    bool equals(const group_element &a) const {
        return *this == a;
    }

    bool equals(shared_ptr<group_element> a) const {
        return *this == *a;
    }

    virtual bool operator!=(const group_element& a) const {
        return !((*this) == a);
    }
};

class group {
    
public:   
    group(){};
    virtual ~group(){};
    
    virtual shared_ptr<group_element> neutral() const = 0;

    virtual shared_ptr<group_element> operation(shared_ptr<group_element> a, 
                           shared_ptr<group_element> b) const = 0;

    virtual shared_ptr<group_element> inverse(shared_ptr<group_element> a) const = 0;

    virtual shared_ptr<group_element> random_element() const = 0;

    virtual uint64_t size() const = 0;

    virtual uint64_t index_of(shared_ptr<group_element> a) const = 0;

    virtual shared_ptr<group_element> get_element(uint64_t index) const = 0;

    bool is_involutive(shared_ptr<group_element> a) const {
        shared_ptr<group_element> b = operation(a,a);
        return b->equals(neutral()); 
    }

    virtual vector<shared_ptr<group_element> > get_involutives() const = 0;
};


