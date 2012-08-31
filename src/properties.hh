#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <vector>
#include <cstring>

using namespace std;

class properties {

private:
    map<string, string> data;
    static const int BUFFER_SIZE = 1024;

public:
    properties() {
    }

    properties(const properties &p) {
        data = p.data;
    }

    ~properties() {
    }

    properties &operator=(const properties &p) {
        if (this != &p) {
            this->data = p.data;
        }

        return *this;
    }

    static properties load(string filename) {
        ifstream stream (filename.c_str(), ifstream::in);

        char buffer[BUFFER_SIZE];

        properties p;

        while (stream.good()) {

            memset(buffer, 0, sizeof(char) * BUFFER_SIZE);
            stream.getline(buffer, BUFFER_SIZE);
            stringstream sstream;
            string key, value;

            if (buffer[0] != '#') {
                sstream.str(buffer);

                if ((sstream >> key) && (sstream >> value)) {
                    p.data[key] = value;
                }
            }
        }

        stream.close();

        return p;
    }

    string get(string key, string default_value = "") {

        if (exists(key)) return data[key];

        else return default_value;

    }

    bool get_bool(string key, bool default_value = true) {

        if (exists(key)) return data[key] == "true";

        else return default_value;

    }

    int get_int(string key, int default_value = 0) {

        if (exists(key)) {

            istringstream sstream;
            sstream.str(data[key]);
            int value;

            sstream >> value;

            return value;

        } else return default_value;
    }

    uint64_t get_unsigned_long(string key, uint64_t default_value = 0) {
        if (exists(key)) {

            istringstream sstream;
            sstream.str(data[key]);
            uint64_t value;

            sstream >> value;

            return value;

        } else return default_value;
    }

    uint32_t get_unsigned(string key, uint32_t default_value = 0) {
        if (exists(key)) {

            istringstream sstream;
            sstream.str(data[key]);
            uint32_t value;

            sstream >> value;

            return value;

        } else return default_value;
    }


    double get_double(string key, double default_value = 0) {

        if (exists(key)) {

            istringstream sstream;
            sstream.str(data[key]);
            double value;

            sstream >> value;

            return value;

        } else return default_value;
    }

    void put(string key, string value) {
        data[key] = value;
    }

    int length() {
        return (int)data.size();
    }

    vector<string> get_keys() {
        vector<string> keys;
        for (map<string, string>::iterator it = data.begin(); it != data.end(); it++)
            keys.push_back(it->first);

        return keys;
    }

    bool exists(string key) {
        return (data.find(key) != data.end());
    }
};

