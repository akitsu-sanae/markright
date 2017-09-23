#include <iostream>
#include <fstream>
#include <cstdlib>

#include "ast.hpp"
#include "parser.hpp"

struct Options {
    std::string input_filename;
    std::string output_filename = "output.tex";
};

Options parse_commandline_args(std::vector<std::string>&& arg) {
    Options result;
    switch (arg.size()) {
    case 1:
        result.input_filename = arg[0];
        break;
    case 3:
        if (arg[0] == "-o") {
            result.output_filename = arg[1];
            result.input_filename = arg[2];
            break;
        } else if (arg[1] == "-o") {
            result.input_filename = arg[0];
            result.output_filename = arg[2];
            break;
        }
    default:
        std::cout << "usage: markright [input filename] -o [output filename]" << std::endl;
        std::exit(-1);
    }
    return result;
}

int main(int argc, char* argv[]) {
    std::vector<std::string> args{argv + 1, argv+argc};
    auto options = parse_commandline_args(std::move(args));

    Parser parser{std::ifstream(options.input_filename)};
    std::ofstream output(options.output_filename);
    output << parser.parse()->to_xelatex() << std::endl;
}

