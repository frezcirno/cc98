#include <getopt.h>
#include <stdio.h>
#include <unistd.h>

#include "ast/node.hpp"
#include "parser.tab.hpp"
#include "translate.h"

extern char *optarg;
extern int optind, opterr, optopt;

int main(int argc, char **argv) {
  int res;
  FuncUnit *root;

  char *input, *output;

  struct option longopts[] = {{"input", required_argument, NULL, 'i'},
                              {"output", required_argument, NULL, 'o'},
                              {"help", no_argument, NULL, 'h'},
                              {0, 0, 0, 0}};

  while (getopt_long(argc, argv, "i:o:h", longopts, NULL) != -1) {
    switch (optopt) {
    case 'i':
      // input file
      input = optarg;
      break;
    case 'o':
      output = optarg;
      break;
    case 'h':
      printf("Usage: %s [options] <input>\n", argv[0]);
      printf("Options:\n");
      printf("  -i, --input <file>   input file\n");
      printf("  -o, --output <file>  output file\n");
      printf("  -h, --help           display this help and exit\n");
      return 0;
    default:
      printf("%s: invalid option -- '%c'\n", argv[0], optopt);
      printf("Try '%s --help' for more information.\n", argv[0]);
      return 1;
    }
  }

  res = parse(input, &root);
  if (res != 0)
    return res;

  compile(root, 0);

  return res;
}
