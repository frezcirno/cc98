#include "parser.tab.hpp"
#include <stdio.h>

// stub function
void yyerror(YYLTYPE *loc, void *driver, const char *msg) {
  fprintf(stderr, "%s:%d error: %s\n", ((driver_t *)driver)->file, loc->first_line, msg);
}

// stub function
int sym_type(const char *) {
    return IDENTIFIER;
}

YYSTYPE yylval;
YYLTYPE yylloc;

int main(int argc, char **argv) {
  int token;

  if (argc <= 1) {
    return 1;
  }

  driver_t driver;
  driver.file = argv[1];
  driver.fd = fopen(argv[1], "r");
  if (driver.fd == NULL)
    return -ENOENT;
  lexer_begin(&driver);

  while (1) {
    token = yylex_orig(&yylval, &yylloc, &driver);
    if (token == 0) {
      break;
    }
  }

  lexer_end(&driver);
  fclose(driver.fd);

  return 0;
}
