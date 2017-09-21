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

    while (auto section = parse_section()) {
        if (section)
            article->sections.push_back(std::move(section));
        else
            break;
    }
    return article;
}

util::ptr<Section> Parser::parse_section() {
    if (!is_match("#"))
        return nullptr;

    auto section = std::make_unique<Section>();
    input.front().erase(0, 1); // first is '#'
    util::trim_left(input.front());
    section->title = input.front();
    input.pop_front();

    while (auto block = parse_block_element()) {
        if (block)
            section->contents.push_back(std::move(block));
        else
            break;
    }
    return section;
}

util::ptr<BlockElement> Parser::parse_block_element() {
    if (input.empty())
        return nullptr;
    if (is_match(" "))
        return parse_paragraph();
    if (is_match("1."))
        return parse_list();
    throw std::logic_error{"invalid input"};
}

util::ptr<Paragraph> Parser::parse_paragraph() {
    if (!is_match(" "))
        return nullptr;
    auto paragraph = std::make_unique<Paragraph>();
    input.front().erase(0, 1); // first is ' '
    util::trim_left(input.front());
    while (auto inline_ = parse_inline_element()) {
        if (inline_)
            paragraph->contents.push_back(std::move(inline_));
        else
            break;
    }
    return paragraph;
}

util::ptr<List> Parser::parse_list() {
    auto list = std::make_unique<List>();
    int count = 1;
    while (is_match(std::to_string(count) + ".")) {
        input.front().erase(0, 2);
        util::trim_left(input.front());
        list->contents.push_back(parse_inline_element());
        ++count;
    }
    if (list->contents.empty())
        return nullptr;
    else
        return list;
}

util::ptr<InlineElement> Parser::parse_inline_element() {
    return parse_statement();
}

util::ptr<Statement> Parser::parse_statement() {
    if (is_match("#", " ", "1."))
        return nullptr;
    auto statement = std::make_unique<Statement>(input.front());
    input.pop_front();
    return statement;
}

