%{
    #include <stdio.h>
    #include "syntax.tab.h"

    #define YY_USER_ACTION \
        yylloc.first_line = yylloc.last_line = yylineno; \
        yylloc.first_column = yycolumn; \
        yylloc.last_column = yycolumn + yyleng - 1; \
        yycolumn += yyleng;

    int yycolumn = 1;
%}

DIGITS      [0-9]+
REAL       [0-9]+\.[0-9]+
ID          [a-zA-Z][a-zA-Z0-9]*

%%
{DIGITS} {
    yylval.int_value = atoi(yytext);
    return INT;
}
{REAL} {
    yylval.double_value = atof(yytext);
    return REAL;
}
{ID} {
    yylval.str_value = strdup(yytext);
    return ID;
}
";" { return SEMICOLON; }
"," { return COMMA; }
"=" { return EQUAL; }
"==" { return DOUBLE_EQUAL; }
"!=" { return NE; }
"<" { return LT; }
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
"struct" { return STRUCT; }
"if" { return IF; }
"else" { return ELSE; }
"while" { return WHILE; }
"return" { return RETURN; }
"int|float|void" { return TYPE; }

"//".* {
    /* eat up comment */
}
"/*" {
    /* eat up comment */
    while (1) {
        // allow nested comment
        while (yylex() != '*');
        if (yylex() == '/') break;
    }
}
"\n" {
    yycolumn = 1;
}
[ \t\r] {
}
. {
    printf("Error type at Line %d: Mysterious characters \'%s\'\n", yylineno, yytext);
}

%%
