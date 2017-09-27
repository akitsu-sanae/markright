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
    else if (desc_type == "\%slide-mode")
        article.is_slide = true;
    else
        throw std::logic_error{"invalid desc: " + desc_type};
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
    if (is_match("##"))
        return parse_subsection();
    if (is_match(" "))
        return parse_paragraph();
    if (is_match("1."))
        return parse_indexed_list();
    if (is_match("*"))
        return parse_list();
    if (is_match(">"))
        return parse_quote();
    if (is_match("[code]"))
        return parse_codeblock();
    if (is_match("[proof]"))
        return parse_prooftree();
    if (is_match("#")) // section
        return nullptr;
    throw std::logic_error{"invalid input"};
}

util::ptr<SubSection> Parser::parse_subsection() {
    if (!is_match("##"))
        return nullptr;
    input.front().erase(0, 2); // remove '##'
    util::trim_left(input.front());
    auto subsection = std::make_unique<SubSection>();
    subsection->title = input.front();
    input.pop_front();

    while (!is_match("##")) {
        auto block = parse_block_element();
        if (block)
            subsection->contents.push_back(std::move(block));
        else
            break;
    }
    return subsection;
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

util::ptr<IndexedList> Parser::parse_indexed_list() {
    auto list = std::make_unique<IndexedList>();
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

util::ptr<List> Parser::parse_list() {
    auto list = std::make_unique<List>();
    while (is_match("*")) {
        input.front().erase(0, 1);
        util::trim_left(input.front());
        list->contents.push_back(parse_inline_element());
    }
    if (list->contents.empty())
        return nullptr;
    else
        return list;
}

util::ptr<Quote> Parser::parse_quote() {
    auto list = std::make_unique<Quote>();
    while (is_match(">")) {
        input.front().erase(0, 1); // remove '>'
        util::trim_left(input.front());
        list->contents.push_back(parse_inline_element());
    }
    if (list->contents.empty())
        return nullptr;
    else
        return list;
}

auto calc_block_body(std::deque<std::string> const& input) {
    auto start = input.front().begin();
    while (*start != '{')
        ++start;
    int  brace_count = 0;
    int count = 0;
    for (auto const& line : input) {
        for (auto it=line.begin(); it!=line.end(); ++it) {
            if (*it == '{')
                ++brace_count;
            if (*it == '}')
                --brace_count;
            if (brace_count == 0 && it > start)
                return std::make_pair(start, it);
        }
        count++;
    }
    throw std::logic_error{"code block is not closed"};
}

util::ptr<CodeBlock> Parser::parse_codeblock() {
    input.front().erase(0, 6); // remove '[code]'

    auto code_body = calc_block_body(input);
    std::string content{code_body.first + 1, input.front().cend()};
    input.pop_front();

    while (true) {
        if (input.front().begin() <= code_body.second && code_body.second < input.front().end())
            break;
        content += input.front() + "\n";
        input.pop_front();
    }
    auto lastline_length = code_body.second - input.front().begin();
    content += input.front().substr(0, lastline_length);
    input.front().erase(0, lastline_length + 1);
    if (input.front().empty())
        input.pop_front();

    auto code = std::make_unique<CodeBlock>();
    code->source_code = content;
    return code;
}

#include <iostream>
void debug(std::deque<std::string> const& input) {
    std::cout << "---------------------------" << std::endl;
    for (auto const& line : input)
        std::cout << line << std::endl;
}

util::ptr<ProofTree> Parser::parse_prooftree() {
    input.front().erase(0, 7); // remove '[proof]'
    while (is_match(" "))
        input.front().erase(0, 1);
    if (!is_match("{"))
        throw std::logic_error{"expected is '{', but " + input.front() + " comes..."};
    input.pop_front();

    auto proof = std::make_unique<ProofTree>();
    util::trim_left(input.front());
    while (is_match("[premise]")) {
        input.front().erase(0, 9); // erase [premise]
        util::trim_left(input.front());
        proof->premises.push_back(input.front());
        input.pop_front();
        util::trim_left(input.front());
    }
    if (is_match("[label]")) {
        input.front().erase(0, 7); // erase [label]
        proof->rule_name = input.front();
        input.pop_front();
        util::trim_left(input.front());
    }
    if (!is_match("[conclusion]"))
        throw std::logic_error{"expected is [conclusion], but " + input.front() + " comes..."};
    input.front().erase(0, 12); // erase [conclusion]
    util::trim_left(input.front());
    proof->conclusion = input.front();
    input.pop_front();

    if (!is_match("}"))
        throw std::logic_error{"expected is '}', but " + input.front() + " comes ..."};
    input.pop_front();
    return proof;
}

util::ptr<InlineElement> Parser::parse_inline_element() {
    return parse_statement();
}

util::ptr<Statement> Parser::parse_statement() {
    if (is_match("#", " ", "1.", "*", ">", "[code]", "[proof]", "##"))
        return nullptr;
    if (input.empty())
        return nullptr;
    auto statement = std::make_unique<Statement>(input.front());
    input.pop_front();
    return statement;
}

