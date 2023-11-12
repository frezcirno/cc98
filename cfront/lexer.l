%option noyywrap nounput noinput batch reentrant
%option bison-bridge bison-locations
%option yylineno

%{
    #include <stdio.h>
    #include "ast.h"
    #include "parser.tab.h"

    #define YY_USER_ACTION \
        yylloc->first_line = yylineno; \
        yylloc->last_line = yylineno; \
        yylloc->first_column = yycolumn; \
        yylloc->last_column = yycolumn + yyleng - 1; \
        yycolumn += yyleng;
%}

decimal ([-+])?(0|[1-9][0-9]*)
binary 0[bB][01]+
octal 0[0-7]+
hex 0[xX][0-9a-fA-F]+
FLOAT [0-9]+\.[0-9]+([eE][+-]?[0-9]+)?
ID [a-zA-Z_][a-zA-Z0-9_]*

%%

";" {return SEMICOLON; }
"," {return COMMA; }
"=" {return EQUAL; }
"==" {return DOUBLE_EQUAL; }
"!=" { return NE; }
"<" {return LT; }
">" { return GT; }
"<=" { return LE; }
">=" { return GE; }
"\+" { return PLUS; }
"-" { return MINUS; }
"\*" { return ASTERISK; }
"/" { return SLASH; }
"%" { return PERCENT; }
"\^" { return CARET; }
"!" { return EXCLAMATION; }
"&" { return AMPERSAND; }
"|" { return VERTICAL; }
"<<" { return DOUBLE_LESS; }
">>" { return DOUBLE_GREATER; }
"$" { return DOLLAR; }
"#" { return SHARP; }
":" { return COLON; }
"::" { return DOUBLE_COLON; }
"\?" { return QUESTION; }
"@" { return AT; }
"\." { return DOT; }
"(" { return LPAREN; }
")" { return RPAREN; }
"{" { return LBRACE; }
"}" { return RBRACE; }
"\[" { return LBRACKET; }
"\]" { return RBRACKET; }
struct { return STRUCT; }
if { return IF; }
else { return ELSE; }
while { return WHILE; }
return { return RETURN; }
void |
char |
short |
int |
long |
float |
double {
    yylval->stringValue = strdup(yytext);
    return TYPE;
}
true |
false {
    yylval->intValue = strcmp(yytext, "true") == 0 ? 1 : 0;
    return BOOL;
}
\'(\\.|[^\\'])\' {
    yylval->intValue = yytext[1];
    return CHAR;
}
{decimal} |
{octal} |
{hex} {
    yylval->intValue = atoi(yytext);
    return INT;
}
{FLOAT} {
    yylval->floatValue = atof(yytext);
    return FLOAT;
}
{ID} {
    yylval->stringValue = strdup(yytext);
    return ID;
}
\"(\\.|[^\\"])*\" {
    yylval->stringValue = strdup(yytext);
    return STRING;
}
"//.*" {
    /* eat up comment */
}
"/\*.*\*/" {
    /* eat up comment */
}
[ \t\r] {
    /* eat up whitespace */
}
\n {
    yylineno++;
    yycolumn = 1;
}
. {
    yyerror(yylloc, yyextra, "invalid input");
}

%%

int lexer_begin(driver_t *driver)
{
    int rv = yylex_init_extra(driver, &driver->scanner);
    if (rv != 0)
        return rv;

#if DEBUG
    yyset_debug(1, driver->scanner);
#endif

    yyrestart(driver->fd, driver->scanner);

    return 0;
}

int lexer_end(driver_t *driver)
{
    return yylex_destroy(driver->scanner);
}
