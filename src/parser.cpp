#include "parser.hpp"
#include "ast.hpp"

static void add_description(Article& article, std::string const& line) {
    auto pos = line.find_first_of(' ');
    auto desc_type = line.substr(0, pos);
    auto desc = line.substr();
    if (desc_type == "\%title")
        article.title = line.substr(pos+1, line.size()-1);
    else if (desc_type == "\%author")
        article.author = line.substr(pos+1, line.size()-1);
    else if (desc_type == "\%date")
        article.date = line.substr(pos+1, line.size()-1);
}

Parser::Parser(std::ifstream ifs) {
    if (!ifs)
        return;
    std::string line;
    while (std::getline(ifs, line)) {
        if (!line.empty() && line[0] != ';')
            input.push_back(line);
    }
}

Article Parser::parse() {
    return parse_article();
}

Article Parser::parse_article() {

    Article article;

    // title, author, date
    while (!input.empty() && input.front()[0] == '%') {
        add_description(article, input.front());
        input.pop_front();
    }

    while (!input.empty() && input.front()[0] == '#')
        article.sections.push_back(parse_section());

    if (!input.empty())
        throw std::runtime_error{"unparsed: " + input.front()};

   return article;
}

Section Parser::parse_section() {
    Section section;
    input.front().erase(0, 1); // first is '#'
    section.title = input.front();
    input.pop_front();

    while (!input.empty() && input.front()[0] == ' ')
        section.contents.push_back(parse_paragraph());

    return section;
}

Paragraph Parser::parse_paragraph() {
    Paragraph paragraph;
    input.front().erase(0, 1); // first is ' '
    while (!input.empty() && input.front()[0] != ' ' && input.front()[0] != '#') {
        paragraph.statements.push_back(input.front());
        input.pop_front();
    }
    return paragraph;
}

