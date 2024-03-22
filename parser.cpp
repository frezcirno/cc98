#include <stdio.h>

#include "ast.hpp"
#include "parser.tab.hpp"
#include "printer.hpp"

int main(int argc, char** argv)
{
  int res;
  TranslationUnit* root;

  if (argc <= 1)
    return 1;

  res = parse(argv[1], &root);
  if (res == 0) {
    NodePrinter printer;
    printer.visit(root);
  }

  return res;
}
