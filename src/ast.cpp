#include "utility.hpp"
#include "ast.hpp"

std::string Section::to_string() const {
    std::string result = "[section] {\n";
    result += util::format("title: {}\n", title);
    result += util::format("content: {}\n", content);
    result += "}\n";
    return result;
}

std::string Article::to_string() const {
    std::string result = "[article] {\n";
    result += util::format("title:{}\n", title);
    result += util::format("author:{}\n", author);
    result += util::format("date:{}\n", date);
    for (auto const& section : sections)
        result += section.to_string();
    result += "}\n";
    return result;
}



