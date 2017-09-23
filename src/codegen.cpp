#include "utility.hpp"
#include "ast.hpp"

std::string Statement::to_xelatex(CodeGenInfo const&) const {
    return content;
}

std::string SubSection::to_xelatex(CodeGenInfo const& info) const {
    std::string result;
    if (info.is_slide) {
        result = util::format("\\begin{{}}{{}}\n", "block", title);
        for (auto const& content : contents)
            result += content->to_xelatex(info);
        result += util::format("\\end{{}}\n", "block");
    } else {
        result = util::format("\\subsection{{}}\n", title);
        for (auto const& content : contents)
            result += content->to_xelatex(info);
    }
    return result;
}

std::string Paragraph::to_xelatex(CodeGenInfo const& info) const {
    std::string result = R"(\par )";
    for (auto const& content : contents)
        result += content->to_xelatex(info) + "\n";
    return result;
}

std::string List::to_xelatex(CodeGenInfo const& info) const {
    std::string result = "\\begin{itemize}\n";
    for (auto const& content : contents)
        result += util::format("\\item {}\n", content->to_xelatex(info));
    result += "\\end{itemize}\n";
    return result;
}

std::string IndexedList::to_xelatex(CodeGenInfo const& info) const {
    std::string result = "\\begin{enumerate}\n";
    for (auto const& content : contents)
        result += util::format("\\item {}\n", content->to_xelatex(info));
    result += "\\end{enumerate}\n";
    return result;
}

std::string Quote::to_xelatex(CodeGenInfo const& info) const {
    std::string result = "\\begin{quotation}\n";
    for (auto const& content : contents)
        result += content->to_xelatex(info) + "\n";
    result += "\\end{quotation}\n";
    return result;
}

std::string CodeBlock::to_xelatex(CodeGenInfo const&) const {
    std::string result = "\\begin{lstlisting}\n";
    result += source_code;
    result += "\\end{lstlisting}\n";
    return result;
}

std::string Section::to_xelatex(CodeGenInfo const& info) const {
    std::string result;
    if (info.is_slide) {
        result = util::format("\\begin{{}}[fragile]{{}}\n", "frame", title);
        for (auto const& content: contents)
            result += content->to_xelatex(info);
        result += util::format("\\end{{}}\n", "frame");
    } else {
        result = util::format("\\section{{}}\n", title);
        for (auto const& content: contents)
            result += content->to_xelatex(info);
    }
    return result;
}

std::string Article::to_xelatex(CodeGenInfo const& info) const {
    std::string result;
    if (is_slide) {
        result = "\\documentclass[12pt, unicode]{beamer}\n";
        result += "\\usetheme{CambridgeUS}\n";
    } else {
        result = "\\documentclass[a4paper]{article}\n";
    }

    result += util::format("\\title{{}}\n", title);
    result += util::format("\\author{{}}\n", author);
    result += util::format("\\date{{}}\n", date);

    result += "\\usepackage{indentfirst}\n";
    result += "\\usepackage{xltxtra}\n";
    result += "\\usepackage{bussproofs}\n";

    result += "\\usepackage{listings}";
    result += "\\lstset{basicstyle={\\ttfamily}, frame={tblr}}";

    result += "\\setmainfont{IPAMincho}\n";
    result += "\\setsansfont{IPAGothic}\n";
    result += "\\setmonofont{IPAGothic}\n";

    result += "\\XeTeXlinebreaklocale \"ja\"\n";

    result += "\\begin{document}\n";
    for (auto const& section : sections)
        result += section->to_xelatex(info);
    result += "\\end{document}\n";
    return result;
}

