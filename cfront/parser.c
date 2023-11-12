#include <stdio.h>

#include "ast.h"
#include "parser.tab.h"

int main(int argc, char **argv) {
  int res;
  node_t *root;

  if (argc <= 1)
    return 1;

  res = parse(argv[1], &root);
  if (res == 0) {
    print_tree(root, 0);
  }

  return res;
}
