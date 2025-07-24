// Config.cpp
#include "Config.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

static std::string trim(const std::string& s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    size_t b = s.find_last_not_of (" \t\r\n");
    return (a==std::string::npos ? "" : s.substr(a, b-a+1));
}

bool Config::load(const std::string& fname) {
    std::ifstream in(fname);
    if (!in.is_open()) return false;
    std::string line;
    while (std::getline(in,line)) {
        auto pos = line.find('=');
        if (pos==std::string::npos) continue;
        std::string key = trim(line.substr(0,pos));
        std::string val = trim(line.substr(pos+1));
        if (key.empty()||val.empty()) continue;
        data_[key] = val;
    }
    return true;
}

int Config::getInt(const std::string& k, int def) const {
    auto it = data_.find(k);
    if (it==data_.end()) return def;
    try { return std::stoi(it->second); }
    catch(...) { return def; }
}

float Config::getFloat(const std::string& k, float def) const {
    auto it = data_.find(k);
    if (it==data_.end()) return def;
    try { return std::stof(it->second); }
    catch(...) { return def; }
}

std::string Config::getString(const std::string& k,
                              const std::string& def) const {
    auto it = data_.find(k);
    return (it==data_.end() ? def : it->second);
}
