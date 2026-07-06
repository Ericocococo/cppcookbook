#include "logger.h"
#include <iostream>

Logger::Logger(std::string tag) : tag_(std::move(tag)) {}

void Logger::info(const std::string& msg) const {
    std::cout << "[" << tag_ << "] " << msg << "\n";
}
