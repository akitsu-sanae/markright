#ifndef MARKRIGHT_PARSER_HPP
#define MARKRIGHT_PARSER_HPP

#include <deque>
#include <fstream>

#include "utility.hpp"

struct Article;
struct Section;
struct Paragraph;

struct Parser {
    explicit Parser(std::ifstream ifs);
    std::deque<std::string> input;
    util::ptr<Article> parse();
private:
    util::ptr<Article> parse_article();
    util::ptr<Section> parse_section();
    util::ptr<Paragraph> parse_paragraph();
};

#endif

