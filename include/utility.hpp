#ifndef MARKRIGHT_UTILITY_HPP
#define MARKRIGHT_UTILITY_HPP

#include <string>
#include <memory>
#include <stdexcept>

namespace std {
inline static std::string to_string(std::string const& s) { return s; }
}

namespace util {

inline static std::string format(std::string const& text) {
    return text;
}

template<typename Head, typename ... Tail>
inline static std::string format(std::string const& text, Head const& head, Tail const& ... tail) {
    auto pos = text.find("{}");
    if (pos == std::string::npos)
        throw std::logic_error{"too few arguments"};
    std::string rest = text.substr(pos+2, text.length());
    return text.substr(0, pos) + std::to_string(head) + format(rest, tail ...);
}

template<typename T>
using ptr = std::unique_ptr<T>;

inline static std::string& trim_left(std::string& str) {
    auto start = std::begin(str);
    while (std::isspace(*start))
        ++start;
    str.erase(std::begin(str), start);
    return str;
}


}

#endif

