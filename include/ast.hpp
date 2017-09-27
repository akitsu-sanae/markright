#ifndef MARKRIGHT_AST_HPP
#define MARKRIGHT_AST_HPP

#include <string>
#include <vector>

#include "utility.hpp"

struct CodeGenInfo {
    bool is_slide;
};

struct Ast {
    virtual std::string to_xelatex(CodeGenInfo const&) const = 0;
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
    std::string to_xelatex(CodeGenInfo const&) const override;
    explicit Statement(std::string const& str) :
        content{str}
    {}
    std::string content;
};

struct BlockElement : public Ast {
    enum Type {
        SubSection,
        Paragraph,
        IndexedList,
        List,
        Quote,
        CodeBlock,
        ProofTree,
    };
    virtual Type type() const = 0;
};

struct SubSection : public BlockElement {
    std::string title;
    std::vector<util::ptr<BlockElement>> contents;
    Type type() const override {
        return Type::SubSection;
    }
    std::string to_xelatex(CodeGenInfo const&) const override;
};

struct Paragraph : public BlockElement {
    std::vector<util::ptr<InlineElement>> contents;
    Type type() const override {
        return Type::Paragraph;
    }
    std::string to_xelatex(CodeGenInfo const&) const override;
};

struct List : public BlockElement {
    std::vector<util::ptr<InlineElement>> contents;
    Type type() const override {
        return Type::List;
    }
    std::string to_xelatex(CodeGenInfo const&) const override;
};

struct IndexedList : public BlockElement {
    std::vector<util::ptr<InlineElement>> contents;
    Type type() const override {
        return Type::IndexedList;
    }
    std::string to_xelatex(CodeGenInfo const&) const override;
};

struct Quote : public BlockElement {
    std::vector<util::ptr<InlineElement>> contents;
    Type type() const override {
        return Type::Quote;
    }
    std::string to_xelatex(CodeGenInfo const&) const override;
};

struct CodeBlock : public BlockElement {
    std::string source_code;
    Type type() const override {
        return Type::CodeBlock;
    }
    std::string to_xelatex(CodeGenInfo const&) const override;
};

struct ProofTree : public BlockElement {
    std::string rule_name;
    std::vector<std::string> premises;
    std::string conclusion;
    Type type() const override {
        return Type::ProofTree;
    }
    std::string to_xelatex(CodeGenInfo const&) const override;
};

struct Section : public Ast {
    std::string title;
    std::vector<util::ptr<BlockElement>> contents;
    std::string to_xelatex(CodeGenInfo const&) const override;
};

struct Article : public Ast {
    std::string title;
    std::string author;
    std::string date;
    bool is_slide = false;
    std::vector<util::ptr<Section>> sections;
    std::string to_xelatex(CodeGenInfo const&) const override;
    std::string to_xelatex() const {
        return to_xelatex(CodeGenInfo{is_slide});
    }
};

#endif
