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

D      [0-9]
E      [Ee][+-]?{D}+
FS     (f|F|l|L)
IS     (u|U|l|L)*

/* my extension */
binary      0[bB][01]+{IS}?
octal       0[0-7]+{IS}?
decimal     (0|[1-9]{D}*){IS}?
hex         0[xX][a-fA-F0-9]+{IS}?
float1      {D}+"."{D}*({E})?{FS}?
float2      {D}*"."{D}+({E})?{FS}?
float3      {D}+{E}{FS}?
identifier  [a-zA-Z_][a-zA-Z0-9_]*
string      L?\"(\\.|[^\\"])*\"
char        L?'(\\.|[^\\'])+'

%%

"//".*      { /* eat up comment */ }
"/*".*?"*/" { /* eat up comment */ }
";"         { return SEMICOLON; }
","         { return COMMA; }
"="         { return EQUAL; }
"=="        { return DOUBLE_EQUAL; }
"!="        { return NE; }
"<"         { return LT; }
">"         { return GT; }
"<="        { return LE; }
">="        { return GE; }
"\+"        { return PLUS; }
"-"         { return MINUS; }
"\*"        { return ASTERISK; }
"/"         { return SLASH; }
"%"         { return PERCENT; }
"\^"        { return CARET; }
"!"         { return EXCLAMATION; }
"&"         { return AMPERSAND; }
"|"         { return VERTICAL; }
"<<"        { return DOUBLE_LESS; }
">>"        { return DOUBLE_GREATER; }
"$"         { return DOLLAR; }
"#"         { return SHARP; }
":"         { return COLON; }
"::"        { return DOUBLE_COLON; }
"\?"        { return QUESTION; }
"@"         { return AT; }
"\."        { return DOT; }
"("         { return LPAREN; }
")"         { return RPAREN; }
"{"         { return LBRACE; }
"}"         { return RBRACE; }
"\["        { return LBRACKET; }
"\]"        { return RBRACKET; }
"struct"    { return STRUCT; }
"if"        { return IF; }
"else"      { return ELSE; }
"while"     { return WHILE; }
"return"    { return RETURN; }
"void" |
"char" |
"short" |
"int" |
"long" |
"float" |
"double" {
    yylval->stringValue = strdup(yytext);
    return TYPE;
}
"true" |
"false" {
    // yylval->intValue = strcmp(yytext, "true") == 0 ? 1 : 0;
    yylval->stringValue = strdup(yytext);
    return BOOL;
}
{decimal} |
{octal} |
{hex} {
    yylval->stringValue = strdup(yytext);
    return INT;
}
{float1} |
{float2} |
{float3} {
    yylval->stringValue = strdup(yytext);
    return FLOAT;
}
{identifier} {
    yylval->stringValue = strdup(yytext);
    return ID;
}
{char} {
    // yylval->intValue = yytext[1];
    yylval->stringValue = strdup(yytext);
    return CHAR;
}
{string} {
    yylval->stringValue = strdup(yytext);
    return STRING;
}
[ \t\n\f\r] { /* eat up whitespace */ }
. {
    char msg[256];
    snprintf(msg, 256, "unexpected input: %s", yytext);
    yyerror(yylloc, yyextra, msg);
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
