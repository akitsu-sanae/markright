#ifndef MARKRIGHT_AST_HPP
#define MARKRIGHT_AST_HPP

#include <string>
#include <vector>

struct Paragraph {
    std::vector<std::string> statements;
};

struct Section {
    std::string title;
    std::vector<Paragraph> contents;
};

struct Article {
    std::string title;
    std::string author;
    std::string date;
    std::vector<Section> sections;
};

#endif
