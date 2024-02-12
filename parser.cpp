#include <stdio.h>

#include "parser.tab.hpp"
#include "printer.hpp"

int main(int argc, char **argv) {
  int res;
  Node *root;

  if (argc <= 1)
    return 1;

  res = parse(argv[1], &root);
  if (res == 0) {
    NodePrinter printer;
    root->accept(&printer);
  }

  return res;
}
