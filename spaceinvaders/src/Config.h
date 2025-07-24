// Config.h
#pragma once
#include <string>
#include <unordered_map>

class Config {
public:
    bool load(const std::string& filename);
    int    getInt   (const std::string& key, int def)    const;
    float  getFloat (const std::string& key, float def)  const;
    std::string getString(const std::string& key,
                          const std::string& def) const;
private:
    std::unordered_map<std::string,std::string> data_;
};
