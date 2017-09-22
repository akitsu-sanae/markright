#include "utility.hpp"
#include "ast.hpp"

std::string Statement::to_xelatex() const {
    return content;
}

std::string SubSection::to_xelatex() const {
    std::string result = util::format("\\subsection{{}}\n", title);
    for (auto const& content : contents)
        result += content->to_xelatex() + "\n";
    return result;
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

std::string IndexedList::to_xelatex() const {
    std::string result = "\\begin{enumerate}\n";
    for (auto const& content : contents)
        result += util::format("\\item {}\n", content->to_xelatex());
    result += "\\end{enumerate}\n";
    return result;
}

std::string Quote::to_xelatex() const {
    std::string result = "\\begin{quotation}\n";
    for (auto const& content : contents)
        result += content->to_xelatex() + "\n";
    result += "\\end{quotation}\n";
    return result;
}

std::string CodeBlock::to_xelatex() const {
    std::string result = "\\begin{lstlisting}\n";
    result += source_code;
    result += "\\end{lstlisting}\n";
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

    result += "\\usepackage{listings}";
    result += "\\lstset{basicstyle={\\ttfamily}}";

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

