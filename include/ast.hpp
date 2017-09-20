#ifndef MARKRIGHT_AST_HPP
#define MARKRIGHT_AST_HPP

#include <string>
#include <vector>

struct Section {
    std::string to_string() const;
    std::string title;
    std::string content;
};

struct Article {
    std::string to_string() const;
    std::string title;
    std::string author;
    std::string date;
    std::vector<Section> sections;
};

#endif
