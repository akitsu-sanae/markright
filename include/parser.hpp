#ifndef MARKRIGHT_PARSER_HPP
#define MARKRIGHT_PARSER_HPP

#include <fstream>
#include "keika.hpp"

struct Article;

struct Parser {
    std::ifstream input;
    keika::Result<Article, std::string> parse();
};

#endif

