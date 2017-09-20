#include <iostream>
#include <fstream>

#include "ast.hpp"
#include "parser.hpp"
#include "codegen.hpp"

void print_usage(char const* name) {
    std::cout << "usage: " << name << " [filename]" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        print_usage(argv[0]);
        return 0;
    }

    Parser parser{std::ifstream{argv[1]}};
    std::cout << convert_to_xelatex(parser.parse()) << std::endl;
}

