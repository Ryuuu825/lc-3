#include "stringhelper.hpp"

std::vector<std::string> split(const std::string& str, const std::string& delim)
{
    std::vector<std::string> tokens;
    size_t prev = 0, pos = 0;
    do
    {
        pos = str.find(delim, prev);
        if (pos == std::string::npos) pos = str.length();
        std::string token = str.substr(prev, pos - prev);
        if (!token.empty()) tokens.push_back(token);
        prev = pos + delim.length();
    } while (pos < str.length() && prev < str.length());
    return tokens;
}

std::string trim(const std::string& str)
{
    size_t first = str.find_first_not_of(' ');
    if (std::string::npos == first) return str;
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

std::string remove(const std::string& str, const std::vector<char>& delim)
{
    std::string result = str;
    for (const auto& c : delim)
    {
        result.erase(std::remove(result.begin(), result.end(), c), result.end());
    }
    return result;
}

bool starts_with(const std::string& str, const std::string& prefix)
{
    return str.find(prefix) == 0;
}

std::string remove_after(const std::string& str, const char delim)
{
    size_t pos = str.find(delim);
    if (pos == std::string::npos) return str;
    return str.substr(0, pos);
}
