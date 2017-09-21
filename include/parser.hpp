#ifndef MARKRIGHT_PARSER_HPP
#define MARKRIGHT_PARSER_HPP

#include <deque>
#include <fstream>

#include "utility.hpp"

struct Article;
struct Section;
struct BlockElement;
struct Paragraph;
struct List;
struct InlineElement;
struct Statement;

struct Parser {
    explicit Parser(std::ifstream ifs);
    std::deque<std::string> input;
    util::ptr<Article> parse();
private:
    util::ptr<Article> parse_article();
    util::ptr<Section> parse_section();

    util::ptr<BlockElement> parse_block_element();
    util::ptr<Paragraph> parse_paragraph();
    util::ptr<List> parse_list();

    util::ptr<InlineElement> parse_inline_element();
    util::ptr<Statement> parse_statement();

    bool is_match() const {
        return false;
    }
    template<typename ... Args>
    bool is_match(std::string const& head, Args const& ... args) {
        if (input.empty())
            return false;
        if (input.front().size() < head.size())
            return false;
        bool is_ok = true;
        for (size_t i=0; i<head.size(); ++i)
            is_ok &= head[i] == input.front()[i];
        if (is_ok)
            return true;

        return is_match(args ...);
    }
};

#endif

