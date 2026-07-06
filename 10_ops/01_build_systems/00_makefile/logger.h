#pragma once
#include <string>

class Logger {
public:
    explicit Logger(std::string tag);
    void info(const std::string& msg) const;
private:
    std::string tag_;
};
