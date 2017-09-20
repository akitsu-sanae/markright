#ifndef MARKRIGHT_PARSER_HPP
#define MARKRIGHT_PARSER_HPP

#include <deque>
#include <fstream>

struct Article;
struct Section;
struct Paragraph;

struct Parser {
    explicit Parser(std::ifstream ifs);
    std::deque<std::string> input;
    Article parse();
private:
    Article parse_article();
    Section parse_section();
    Paragraph parse_paragraph();
};

#endif

