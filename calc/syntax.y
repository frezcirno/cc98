%{
    #include <stdio.h>
    #include <math.h>
%}

/* declared types */
%union {
    int int_value;
    double double_value;
    char *str_value;
}

%locations

/* declared tokens */
%nonassoc Int2Real
%nonassoc <int_value> INT
%nonassoc <double_value> REAL
%nonassoc <str_value> ID
%nonassoc DO WHILE FOR IF ELSE RETURN
%right EQUAL
%left DOUBLE_EQUAL NE LT GT LE GE
%left LPAREN RPAREN LBRACKET RBRACKET LBRACE RBRACE
%left PLUS MINUS ASTERISK SLASH PERCENT
%left AMPERSAND VERTICAL EXCLAMATION CARET
%left DOUBLE_GREATER DOUBLE_LESS
%left DOLLAR SHARP QUESTION AT
%left COMMA SEMICOLON COLON DOUBLE_COLON DOT
%left TYPE STRUCT

%type <double_value> Calc Stmt Expr
%type <int_value> IntExpr

%%

Calc : /* empty */
    | Stmt                        { printf("= %g\n", $1); }
    | Calc Stmt                   { printf("= %g\n", $2); }
    ;
Stmt : Expr SEMICOLON             { $$ = $1; }
    ;
Expr : REAL
    | IntExpr    %prec Int2Real             { $$ = $1; }
    | Expr       PLUS        Expr           { $$ = $1 + $3; }
    | IntExpr    PLUS        Expr           { $$ = $1 + $3; }
    | Expr       MINUS       Expr           { $$ = $1 - $3; }
    | IntExpr    MINUS       Expr           { $$ = $1 - $3; }
    | Expr       ASTERISK    Expr           { $$ = $1 * $3; }
    | IntExpr    ASTERISK    Expr           { $$ = $1 * $3; }
    | Expr       SLASH       Expr           { $$ = $1 / $3; }
    | IntExpr    SLASH       Expr           { $$ = $1 / $3; }
    | IntExpr    SLASH       IntExpr        { $$ = $1 / $3; }
    | Expr       PERCENT     Expr           { $$ = fmod($1, $3); }
    | LPAREN     Expr        RPAREN         { $$ = $2; }
    | PLUS Expr                             { $$ = $2; }
    | MINUS Expr                            { $$ = -$2; }
    ;
IntExpr : INT
    | IntExpr   PLUS            IntExpr     { $$ = $1 + $3; }
    | IntExpr   MINUS           IntExpr     { $$ = $1 - $3; }
    | IntExpr   ASTERISK        IntExpr     { $$ = $1 * $3; }
    | IntExpr   PERCENT         IntExpr     { $$ = $1 % $3; }
    | IntExpr   AMPERSAND       IntExpr     { $$ = $1 & $3; }
    | IntExpr   VERTICAL        IntExpr     { $$ = $1 | $3; }
    | IntExpr   CARET           IntExpr     { $$ = $1 ^ $3; }
    | IntExpr   DOUBLE_LESS     IntExpr     { $$ = $1 << $3; }
    | IntExpr   DOUBLE_GREATER  IntExpr     { $$ = $1 >> $3; }
    | Expr      DOUBLE_EQUAL    Expr        { $$ = $1 == $3; }
    | Expr      NE              Expr        { $$ = $1 != $3; }
    | Expr      LT              Expr        { $$ = $1 < $3; }
    | Expr      GT              Expr        { $$ = $1 > $3; }
    | Expr      LE              Expr        { $$ = $1 <= $3; }
    | Expr      GE              Expr        { $$ = $1 >= $3; }
    | LPAREN    IntExpr         RPAREN      { $$ = $2; }
    | PLUS IntExpr                          { $$ = $2; }
    | MINUS IntExpr                         { $$ = -$2; }
    ;
%%
#include "lex.yy.c"

yyerror(char* msg) {
    fprintf(stderr, "error: %s at line %d column %d\n", msg, yylineno, yycolumn);
}
