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

int main(int argc, char** argv)
{
  int res;
  TranslationUnit* root;

  if (argc <= 1)
    return 1;

  res = parse(argv[1], &root);
  if (res == 0) {
    X86Helper x86;
    Translator trans(&x86);
    root->accept(&trans);
    x86.writeProgram(std::cout);
  }

  return res;
}
