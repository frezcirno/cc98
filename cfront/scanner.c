#include "parser.tab.h"
#include <stdio.h>

void yyerror(YYLTYPE *loc, driver_t *driver, const char *msg) {
  fprintf(stderr, "%s:%d error: %s\n", driver->file, loc->first_line, msg);
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
