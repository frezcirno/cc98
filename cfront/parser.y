/* Procude a GLR parser */
%glr-parser

/* Create a parser-header files for flex */
%defines

/* Verbose error messages */
%error-verbose

/* Enable location tracking */
%locations

/* Reentrant parser */
%define api.pure

/* extra parser yy*() function params */
%parse-param { driver_t *driver }

/* extra yylex() function params */
%lex-param { driver_t *driver }

%code requires {
    #include <stdio.h>
    #include <math.h>
    #include <errno.h>
    #include "ast.h"

    typedef void* yyscan_t;

    typedef struct driver {
        const char *file;
        FILE *fd;
        node_t *root;
        int err_num;
        yyscan_t scanner;
    } driver_t;

    int lexer_begin(driver_t *driver);

    int lexer_end(driver_t *driver);
}

%union {
    int intValue;
    float floatValue;
    double doubleValue;
    char *stringValue;
    char charValue;
    node_t *node;
}

/* internal function prototype */
%{
    static inline int yylex(YYSTYPE *yylval_param, YYLTYPE *yylloc_param, driver_t *driver) {
        return yylex_orig(yylval_param, yylloc_param, driver->scanner);
    }
%}

/* additional definitions and declarations that should be provided to other modules */
%code provides
{
    #define YY_DECL int yylex_orig(YYSTYPE* yylval_param, YYLTYPE* yylloc_param, yyscan_t yyscanner)
    extern YY_DECL;

    void yyerror(YYLTYPE *loc, driver_t *driver, const char *msg);

    int parse(const char *file, node_t **root);
}

/* the tokens */
%right EQUAL
%left DOUBLE_EQUAL NE LT GT LE GE
%left LPAREN RPAREN LBRACKET RBRACKET LBRACE RBRACE
%left PLUS MINUS ASTERISK SLASH PERCENT
%left AMPERSAND VERTICAL CARET
%left DOUBLE_AMPERSAND DOUBLE_VERTICAL EXCLAMATION
%left DOUBLE_GREATER DOUBLE_LESS
%left DOLLAR SHARP QUESTION AT
%left COMMA SEMICOLON COLON DOUBLE_COLON DOT
%nonassoc DO WHILE FOR IF ELSE RETURN STRUCT CLASS
%nonassoc INT BOOL CHAR FLOAT DOUBLE ID TYPE STRING

/* the types */
%type <node> Program ExtDefList ExtDef ExtDecList TypeSpec StructSpecifier OptTag Tag VarDecl FuncDecl ParamList ParamDec CompStmt StmtList Stmt DefList Def DecList Dec Exp Args
%type <node> RelOp
%type <intValue> INT BOOL
%type <charValue> CHAR
%type <floatValue> FLOAT
%type <doubleValue> DOUBLE
%type <stringValue> ID TYPE STRING

%start Program

%%

// High-level Definitions
Program : ExtDefList { $$ = make(N_PROGRAM); appendn($$, $1); driver->root = $$; }
        ;
ExtDefList : ExtDef ExtDefList { $$ = $2; prependn($$, $1); }
           | /* empty */ { $$ = make(N_EXTDEFLIST);}
           ;
ExtDef : TypeSpec ExtDecList SEMICOLON { $$ = make(N_EXTDEF); appendn($$, $1); appendn($$, $2); append($$, N_SEMICOLON); }
       | TypeSpec SEMICOLON { $$ = make(N_EXTDEF); appendn($$, $1); append($$, N_SEMICOLON); }
       | TypeSpec FuncDecl CompStmt { $$ = make(N_EXTDEF); appendn($$, $1); appendn($$, $2); appendn($$, $3); }
       ;
ExtDecList : VarDecl { $$ = make(N_EXTDECLIST); appendn($$, $1); }
           | VarDecl COMMA ExtDecList { $$ = $3; appendn($$, $1); append($$, N_COMMA); }
           ;

// Specifiers
TypeSpec : TYPE { $$ = make(N_TYPESPEC); appends($$, N_TYPE, $1); }
         | StructSpecifier { $$ = make(N_TYPESPEC); appendn($$, $1); }
         ;
StructSpecifier : STRUCT OptTag LBRACE DefList RBRACE { $$ = make(N_STRUCT_SPECIFIER); append($$, N_STRUCT); appendn($$, $2); append($$, N_LBRACE); appendn($$, $4); append($$, N_RBRACE); }
                | STRUCT Tag { $$ = make(N_STRUCT_SPECIFIER); append($$, N_STRUCT); appendn($$, $2); }
                ;
OptTag : ID { $$ = make(N_OPTTAG); appends($$, N_IDENTIFIER, $1); }
       | /* empty */ { $$ = make(N_OPTTAG); }
       ;
Tag : ID { $$ = make(N_TAG); appends($$, N_IDENTIFIER, $1); }
    ;

// Declarators
VarDecl : ID { $$ = make(N_VARDECL); appends($$, N_IDENTIFIER, $1); }
        | ASTERISK VarDecl { $$ = make(N_VARDECL); append($$, N_ASTERISK); appendn($$, $2); }
        | VarDecl LBRACKET INT RBRACKET { $$ = make(N_VARDECL); appendn($$, $1); append($$, N_LBRACKET); appendi($$, N_LITERAL_INT, $3); append($$, N_RBRACKET); }
        ;
FuncDecl : ID LPAREN ParamList RPAREN { $$ = make(N_FUNCDECL); appends($$, N_IDENTIFIER, $1); append($$, N_LPAREN); appendn($$, $3); append($$, N_RPAREN); }
         | ID LPAREN RPAREN { $$ = make(N_FUNCDECL); appends($$, N_IDENTIFIER, $1); append($$, N_LPAREN); append($$, N_RPAREN); }
         ;
ParamList : ParamDec COMMA ParamList { $$ = $3; prepend($$, N_COMMA); prependn($$, $1); }
          | ParamDec { $$ = make(N_PARAMLIST); appendn($$, $1); }
          ;
ParamDec : TypeSpec VarDecl { $$ = make(N_PARAMDEC); appendn($$, $1); appendn($$, $2); }
         ;

// Statements
CompStmt : LBRACE DefList StmtList RBRACE { $$ = make(N_COMPSTMT); append($$, N_LBRACE); appendn($$, $2); appendn($$, $3); append($$, N_RBRACE); }
         | error RBRACE { yyerrok; driver->err_num++; $$ = make(N_COMPSTMT); append($$, N_ERROR); append($$, N_RBRACE); }
         ;
StmtList : Stmt StmtList { $$ = $2; prependn($$, $1); }
         | /* empty */ { $$ = make(N_STMTLIST); }
         ;
Stmt : Exp SEMICOLON { $$ = make(N_STMT); appendn($$, $1); append($$, N_SEMICOLON); }
     | CompStmt { $$ = make(N_STMT); appendn($$, $1); }
     | RETURN Exp SEMICOLON { $$ = make(N_STMT); append($$, N_RETURN); appendn($$, $2); append($$, N_SEMICOLON); }
     | IF LPAREN Exp RPAREN Stmt { $$ = make(N_STMT); append($$, N_IF); append($$, N_LPAREN); appendn($$, $3); append($$, N_RPAREN); appendn($$, $5); }
     | IF LPAREN Exp RPAREN Stmt ELSE Stmt { $$ = make(N_STMT); append($$, N_IF); append($$, N_LPAREN); appendn($$, $3); append($$, N_RPAREN); appendn($$, $5); append($$, N_ELSE); appendn($$, $7); }
     | WHILE LPAREN Exp RPAREN Stmt { $$ = make(N_STMT); append($$, N_WHILE); append($$, N_LPAREN); appendn($$, $3); append($$, N_RPAREN); appendn($$, $5); }
     | DO Stmt WHILE LPAREN Exp RPAREN SEMICOLON { $$ = make(N_STMT); append($$, N_DO); appendn($$, $2); append($$, N_WHILE);
                                                   append($$, N_LPAREN); appendn($$, $5); append($$, N_RPAREN);
                                                   append($$, N_SEMICOLON); }
     | FOR LPAREN Exp SEMICOLON Exp SEMICOLON Exp RPAREN Stmt { $$ = make(N_STMT); append($$, N_FOR); append($$, N_LPAREN); appendn($$, $3);
                                                                append($$, N_SEMICOLON); appendn($$, $5); append($$, N_SEMICOLON); appendn($$, $7);
                                                                append($$, N_RPAREN); appendn($$, $9); }
     | error SEMICOLON { yyerrok; driver->err_num++; $$ = make(N_STMT); append($$, N_ERROR); append($$, N_SEMICOLON); }
     ;

// Local Definitions
DefList : Def DefList { $$ = $2; prependn($$, $1); }
        | /* empty */ { $$ = make(N_DEFLIST);}
        ;
Def : TypeSpec DecList SEMICOLON { $$ = make(N_DEF); appendn($$, $1); appendn($$, $2); append($$, N_SEMICOLON); }
    ;
DecList : Dec { $$ = make(N_DECLIST); appendn($$, $1); }
        | Dec COMMA DecList { $$ = $3; prepend($$, N_COMMA); prependn($$, $1); }
        ;
Dec : VarDecl { $$ = make(N_DEC); appendn($$, $1); }
    | VarDecl EQUAL Exp { $$ = make(N_DEC); appendn($$, $1); append($$, N_EQUAL); appendn($$, $3); }
    ;

// Expressions
Exp : Exp EQUAL Exp { $$ = make(N_EXP); appendn($$, $1); append($$, N_EQUAL); appendn($$, $3); }
    | Exp DOUBLE_AMPERSAND Exp { $$ = make(N_EXP); appendn($$, $1); append($$, N_DOUBLE_AMPERSAND); appendn($$, $3); }
    | Exp DOUBLE_VERTICAL Exp { $$ = make(N_EXP); appendn($$, $1); append($$, N_DOUBLE_VERTICAL); appendn($$, $3); }
    | Exp RelOp Exp %prec DOUBLE_EQUAL { $$ = make(N_EXP); appendn($$, $1); appendn($$, $2); appendn($$, $3); }
    | Exp PLUS Exp { $$ = make(N_EXP); appendn($$, $1); append($$, N_MINUS); appendn($$, $3); }
    | Exp MINUS Exp { $$ = make(N_EXP); appendn($$, $1); append($$, N_PLUS); appendn($$, $3); }
    | Exp ASTERISK Exp { $$ = make(N_EXP); appendn($$, $1); append($$, N_ASTERISK); appendn($$, $3); }
    | Exp SLASH Exp { $$ = make(N_EXP); appendn($$, $1); append($$, N_SLASH); appendn($$, $3); }
    | LPAREN Exp RPAREN { $$ = make(N_EXP); append($$, N_LPAREN); append($$, N_LPAREN); append($$, N_RPAREN); }
    | MINUS Exp %prec MINUS { $$ = make(N_EXP); append($$, N_MINUS); appendn($$, $2); }
    | PLUS Exp %prec PLUS { $$ = make(N_EXP); append($$, N_PLUS); appendn($$, $2); }
    | EXCLAMATION Exp { $$ = make(N_EXP); append($$, N_EXCLAMATION); appendn($$, $2); }
    | ID LPAREN Args RPAREN { $$ = make(N_EXP); appends($$, N_IDENTIFIER, $1); append($$, N_LPAREN); appendn($$, $3); append($$, N_RPAREN); }
    | ID LPAREN RPAREN { $$ = make(N_EXP); appends($$, N_IDENTIFIER, $1); append($$, N_LPAREN); append($$, N_RPAREN); }
    | Exp DOT ID { $$ = make(N_EXP); appendn($$, $1); append($$, N_DOT); appends($$, N_IDENTIFIER, $3); }
    | ID { $$ = make(N_EXP); appends($$, N_IDENTIFIER, $1); }
    | INT { $$ = make(N_EXP); appendi($$, N_LITERAL_INT, $1); }
    | FLOAT { $$ = make(N_EXP); appendf($$, N_LITERAL_FLOAT, $1); }
    | STRING { $$ = make(N_EXP); appends($$, N_LITERAL_STRING, $1); }
    | BOOL { $$ = make(N_EXP); appendi($$, N_LITERAL_BOOL, $1); }
    | CHAR { $$ = make(N_EXP); appendc($$, N_LITERAL_CHAR, $1); }
    | error RPAREN { yyerrok; driver->err_num++; $$ = make(N_EXP); append($$, N_ERROR); }
    ;

RelOp : DOUBLE_EQUAL { $$ = makes(N_RELOP, "DOUBLE_EQUAL"); }
      | NE { $$ = makes(N_RELOP, "NE"); }
      | LT { $$ = makes(N_RELOP, "LT"); }
      | GT { $$ = makes(N_RELOP, "GT"); }
      | LE { $$ = makes(N_RELOP, "LE"); }
      | GE { $$ = makes(N_RELOP, "GE"); }
      ;

Args : Exp COMMA Args { $$ = $3; prepend($$, N_COMMA); prependn($$, $1); }
     | Exp { $$ = make(N_ARGS); appendn($$, $1); }
     ;

%%

void yyerror(YYLTYPE *loc, driver_t *driver, const char *msg) {
    fprintf(stderr, "%s:%d:%d: error: %s\n", driver->file, loc->first_line, loc->first_column, msg);
}

int parse(const char *file, node_t **root) {
    int rv;
    driver_t driver;

#ifdef DEBUG
    yydebug = 1;
#endif

    driver.file = file;
    driver.fd   = fopen(file, "r");
    if (driver.fd == NULL)
        return -ENOENT;

    rv = lexer_begin(&driver);
    if (rv != 0)
        return rv;

    rv = yyparse(&driver);
    if (rv != 0)
        return rv;
    *root = driver.root;

    rv = lexer_end(&driver);
    if (rv != 0)
        return rv;
    fclose(driver.fd);

    return driver.err_num;
}
