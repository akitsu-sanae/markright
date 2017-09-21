#include "utility.hpp"
#include "ast.hpp"

std::string Statement::to_xelatex() const {
    return content;
}

std::string Paragraph::to_xelatex() const {
    std::string result = R"(\par )";
    for (auto const& content : contents)
        result += content->to_xelatex() + "\n";
    return result;
}

std::string List::to_xelatex() const {
    std::string result = "\\begin{itemize}\n";
    for (auto const& content : contents)
        result += util::format("\\item {}\n", content->to_xelatex());
    result += "\\end{itemize}\n";
    return result;
}

std::string Section::to_xelatex() const {
    std::string result = "";
    result += util::format("\\section{{}}\n", title);
    for (auto const& content : contents)
        result += content->to_xelatex();
    return result;
}

std::string Article::to_xelatex() const {
    std::string result = "\\documentclass[a4paper]{article}\n";
    result += util::format("\\title{{}}\n", title);
    result += util::format("\\author{{}}\n", author);
    result += util::format("\\date{{}}\n", date);

    result += "\\usepackage{indentfirst}\n";
    result += "\\usepackage{xltxtra}\n";
    result += "\\usepackage{bussproofs}\n";

    result += "\\setmainfont{IPAMincho}\n";
    result += "\\setsansfont{IPAGothic}\n";
    result += "\\setmonofont{IPAGothic}\n";

    result += "\\XeTeXlinebreaklocale \"ja\"\n";

    result += "\\begin{document}\n";
    for (auto const& section : sections)
        result += section->to_xelatex();
    result += "\\end{document}\n";
    return result;
}

