#include "ast.hpp"
#include "c2.hpp"
#include "translator.hpp"
#include "backend.hpp"
#include "x86.hpp"
#include "ir.hpp"
#include "parser.tab.hpp"
#include "type.hpp"
#include <cassert>
#include <iostream>
#include <stack>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>
#include <getopt.h>

int main(int argc, char** argv)
{
  int res;
  int opt;
  TranslationUnit* root;
  const char* input_file;

  // Parse command line options
  // -o <file> : redirect output to file
  // -h : print help message
  // input file is the last argument
  while ((opt = getopt(argc, argv, "o:h")) != -1) {
    switch (opt) {
      case 'o':
        // redirect output to file
        if (freopen(optarg, "w", stdout) == NULL) {
          std::cerr << "Cannot open file " << optarg << " for writing\n";
          return 1;
        }
        break;
      case 'h':
      default:
        std::cout << "Usage: " << argv[0] << " [-o <file>] <input file>\n";
        return 1;
    }
  }

  if (optind >= argc) {
    std::cerr << "No input file\n";
    return 1;
  }
  input_file = argv[optind];

  res = parse(input_file, &root);
  if (res == 0) {
    X86Helper x86;
    Translator trans(&x86);
    root->accept(&trans);
    x86.writeProgram(std::cout);
  } else {
    std::cerr << "Parse error\n";
  }

  return res;
}
