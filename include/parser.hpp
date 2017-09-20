#ifndef MARKRIGHT_PARSER_HPP
#define MARKRIGHT_PARSER_HPP

#include <deque>
#include <fstream>
#include "keika.hpp"

struct Article;

struct Parser {
    explicit Parser(std::ifstream ifs);
    std::deque<std::string> input;
    keika::Result<Article, std::string> parse();
};

#endif

