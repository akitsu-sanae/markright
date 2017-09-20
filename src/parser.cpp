#include "parser.hpp"
#include "ast.hpp"

using namespace keika;

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

Result<Article, std::string> Parser::parse() {

    Article article;

    // title, author, date
    while (!input.empty() && input.front()[0] == '%') {
        add_description(article, input.front());
        input.pop_front();
    }

    while (!input.empty()) {
        if (input.front()[0] == '#') {
            auto title = input.front().substr(1, input.front().size()-1);
            article.sections.emplace_back();
            article.sections.back().title = title;
        } else {
            article.sections.back().content += input.front();
        }
        input.pop_front();
    }
    return article;
}


