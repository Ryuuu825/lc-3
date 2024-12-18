#pragma once

#include <string>
#include <vector>

std::vector<std::string> split(const std::string& str, const std::string& delim);
std::string trim(const std::string& str);
std::string remove(const std::string& str, const std::vector<char>& delim);
bool starts_with(const std::string& str, const std::string& prefix);
std::string remove_after(const std::string& str, const char delim);
