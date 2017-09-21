#ifndef MARKRIGHT_AST_HPP
#define MARKRIGHT_AST_HPP

#include <string>
#include <vector>

#include "utility.hpp"

struct Ast {
    virtual std::string to_xelatex() const = 0;
};

struct InlineElement : public Ast {
    enum Type {
        Statement
    };
    virtual Type type() const = 0;
};

struct Statement : public InlineElement {
    Type type() const override {
        return Type::Statement;
    }
    std::string to_xelatex() const override;
    explicit Statement(std::string const& str) :
        content{str}
    {}
    std::string content;
};

struct BlockElement : public Ast {
    enum Type {
        Paragraph,
        IndexedList,
        List,
        Quote,
    };
    virtual Type type() const = 0;
};

struct Paragraph : public BlockElement {
    std::vector<util::ptr<InlineElement>> contents;
    Type type() const override {
        return Type::Paragraph;
    }
    std::string to_xelatex() const override;
};

struct List : public BlockElement {
    std::vector<util::ptr<InlineElement>> contents;
    Type type() const override {
        return Type::List;
    }
    std::string to_xelatex() const override;
};

struct IndexedList : public BlockElement {
    std::vector<util::ptr<InlineElement>> contents;
    Type type() const override {
        return Type::IndexedList;
    }
    std::string to_xelatex() const override;
};

struct Quote : public BlockElement {
    std::vector<util::ptr<InlineElement>> contents;
    Type type() const override {
        return Type::Quote;
    }
    std::string to_xelatex() const override;
};

struct Section : public Ast {
    std::string title;
    std::vector<util::ptr<BlockElement>> contents;
    std::string to_xelatex() const override;
};

struct Article : public Ast {
    std::string title;
    std::string author;
    std::string date;
    std::vector<util::ptr<Section>> sections;
    std::string to_xelatex() const override;
};

#endif
