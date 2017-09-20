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

Result<Article, std::string> Parser::parse() {
    if (!input)
        return Result<Article>::error("can not open input file");

    std::string line;

    enum class Phase {
        ArticleDesc,
        Section,
    } phase = Phase::ArticleDesc;

    Article article;
    Section current_section;
    while (std::getline(input, line)) {
        if (line.empty())
            continue;
        if (line[0] == ';')
            continue;
        if (phase == Phase::ArticleDesc) {
            switch (line[0]) {
            case '%':
                add_description(article, line);
                break;
            case '#': // first section
                current_section.title = line.substr(1, line.size() - 1);
                phase = Phase::Section;
                break;
            default:
                return Result<Article>::error("invalid input");
            }
        } else if (phase == Phase::Section) {
            if (line[0] == '#') {
                article.sections.push_back(current_section);
                current_section.title = line.substr(1, line.size() - 1);
                current_section.content = "";
                continue;
            }
            current_section.content += line;
        } else {
            throw std::logic_error{"invalid parse phase"};
        }
    }
    article.sections.push_back(current_section);
    return article;
}


