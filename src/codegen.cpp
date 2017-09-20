#include "utility.hpp"
#include "codegen.hpp"
#include "ast.hpp"

std::string convert_to_xelatex(Paragraph const& paragraph) {
    std::string result = R"(\par )";
    for (auto const& statement : paragraph.statements)
        result += statement;
    result += "\n";
    return result;
}

std::string convert_to_xelatex(Section const& section) {
    std::string result = "";
    result += util::format("\\section{{}}\n", section.title);
    for (auto const& content : section.contents)
        result += convert_to_xelatex(content);
    return result;
}

std::string convert_to_xelatex(Article const& article) {
    std::string result = "\\documentclass[a4paper]{article}\n";
    result += util::format("\\title{{}}\n", article.title);
    result += util::format("\\author{{}}\n", article.author);
    result += util::format("\\date{{}}\n", article.date);

    result += "\\usepackage{xltxtra}\n";
    result += "\\usepackage{bussproofs}\n";

    result += "\\setmainfont{IPAMincho}\n";
    result += "\\setsansfont{IPAGothic}\n";
    result += "\\setmonofont{IPAGothic}\n";

    result += "\\XeTeXlinebreaklocale \"ja\"\n";

    result += "\\begin{document}\n";
    for (auto const& section : article.sections)
        result += convert_to_xelatex(section);
    result += "\\end{document}\n";
    return result;
}

