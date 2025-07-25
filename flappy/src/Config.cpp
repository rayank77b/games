#include "Config.h"
#include <fstream>
#include <sstream>
#include <algorithm>

static std::string trim(const std::string& s) {
    auto a = s.find_first_not_of(" \t\r\n");
    auto b = s.find_last_not_of (" \t\r\n");
    return (a == std::string::npos ? "" : s.substr(a, b-a+1));
}

bool Config::load(const std::string& fname) {
    std::ifstream in(fname);
    if (!in.is_open()) return false;
    std::string line;
    while (std::getline(in, line)) {
        if (std::size_t pos = line.find('='); pos != std::string::npos) {
            std::string k = trim(line.substr(0,pos));
            std::string v = trim(line.substr(pos+1));
            if (!k.empty() && !v.empty()) data_[k]=v;
        }
    }
    return true;
}

int Config::getInt(const std::string& k, int def) const {
    if (auto it = data_.find(k); it != data_.end())
        try { return std::stoi(it->second); } catch(...) {}
    return def;
}

float Config::getFloat(const std::string& k, float def) const {
    if (auto it = data_.find(k); it != data_.end())
        try { return std::stof(it->second); } catch(...) {}
    return def;
}
