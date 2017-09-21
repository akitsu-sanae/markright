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

util::ptr<Article> Parser::parse() {
    return parse_article();
}

util::ptr<Article> Parser::parse_article() {

    auto article = std::make_unique<Article>();

    // title, author, date
    while (!input.empty() && input.front()[0] == '%') {
        add_description(*article, input.front());
        input.pop_front();
    }

    while (!input.empty() && input.front()[0] == '#')
        article->sections.push_back(parse_section());

    if (!input.empty())
        throw std::runtime_error{"unparsed: " + input.front()};

   return article;
}

util::ptr<Section> Parser::parse_section() {
    auto section = std::make_unique<Section>();
    input.front().erase(0, 1); // first is '#'
    section->title = input.front();
    input.pop_front();

    while (!input.empty() && (input.front()[0] == ' ' || input.front()[0] == '1'))
        section->contents.push_back(parse_block_element());

    return section;
}

util::ptr<BlockElement> Parser::parse_block_element() {
    if (input.empty())
        return nullptr;
    switch (input.front()[0]) {
    case ' ':
        return parse_paragraph();
    case '1':
        return parse_list();
    default:
        throw std::logic_error{"invalid input"};
    }
}

util::ptr<Paragraph> Parser::parse_paragraph() {
    auto paragraph = std::make_unique<Paragraph>();
    input.front().erase(0, 1); // first is ' '
    while (true) {
        if (input.empty())
            break;
        if (input.front()[0] == ' ')
            break;
        if (input.front()[0] == '1' && input.front()[1] == '.')
            break;
        if (input.front()[0] == '#')
            break;
        paragraph->contents.push_back(parse_inline_element());
    }
    return paragraph;
}

util::ptr<List> Parser::parse_list() {
    auto list = std::make_unique<List>();
    int count = 1;
    while (!input.empty() && input.front()[0] == '0'+count) {
        input.front().erase(0, 2);
        list->contents.push_back(parse_inline_element());
        ++count;
    }
    return list;
}

util::ptr<InlineElement> Parser::parse_inline_element() {
    return parse_statement();
}

util::ptr<Statement> Parser::parse_statement() {
    auto statement = std::make_unique<Statement>(input.front());
    input.pop_front();
    return statement;
}

