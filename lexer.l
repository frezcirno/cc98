%option noyywrap nounput batch reentrant
%option bison-bridge bison-locations
%option yylineno

%{
    #include <stdio.h>
    #include "ast.hpp"
    #include "parser.tab.hpp"

    #define YY_USER_ACTION \
        yylloc->first_line = yylineno; \
        yylloc->last_line = yylineno; \
        yylloc->first_column = yycolumn; \
        yylloc->last_column = yycolumn + yyleng - 1; \
        yycolumn += yyleng;

    extern int sym_type(const char *);

    #define LOAD_STR yylval->stringValue = strdup(yytext)
    #define LOAD_INT yylval->iVal = atoi(yytext)
    #define LOAD_FLOAT yylval->fVal = strtod(yytext, NULL)
%}

nonzero     [1-9]
digit       [0-9]
octdigit    [0-7]
hexdigit    [a-fA-F0-9]
alpha       [a-zA-Z_]
alnum       [a-zA-Z0-9_]
xX          (0[xX])
uUL         (u|U|L)
ul          (((u|U)(l|L|ll|LL)?)|((l|L|ll|LL)(u|U)?))
uUL8        (u8|u|U|L)
fl          (f|F|l|L)
white       [ \t\v\n\f]
escape      (\\(['"\?\\abfnrtv]|[0-7]{1,3}|x{hexdigit}+))
edigit      ([Ee][+-]?{digit}+)
pdigit      ([Pp][+-]?{digit}+)

decimal     (0|{nonzero}{digit}*){ul}?
octal       0{octdigit}+{ul}?
frac        ({digit}+"."{digit}*|{digit}*"."{digit}+)
float1      {digit}+{edigit}{fl}?
float2      {frac}{edigit}?{fl}?
hex         {xX}{hexdigit}+{ul}?
hexfrac     ({hexdigit}+"."{hexdigit}+|{hexdigit}*"."{hexdigit}+)
floatp1     {xX}{hexdigit}+{pdigit}{fl}?
floatp2     {xX}{hexfrac}{pdigit}{fl}?
char        {uUL}?"'"([^'\\\n]|{escape})+"'"
string      ({uUL8}?\"([^"\\\n]|{escape})*\"{white}*)+
identifier  {alpha}{alnum}*

binary      0[bB][01]+{ul}?

%%

"/*"                                    {
    int c;

    while ((c = yyinput(yyscanner)) != 0) {
        if (c == '*') {
            while ((c = yyinput(yyscanner)) == '*')
                continue;

            if (c == '/')
                goto done;

            if (c == 0)
                break;
        }
    }
    yyerror(yylloc, yyextra, "unterminated comment");
done:
    ;
}
"//".*                                  { /* eat up comment */ }
"auto"                                  { return AUTO; }
"break"                                 { return BREAK; }
"case"                                  { return CASE; }
"char"                                  { return CHAR; }
"const"                                 { return CONST; }
"continue"                              { return CONTINUE; }
"default"                               { return DEFAULT; }
"do"                                    { return DO; }
"double"                                { return DOUBLE; }
"else"                                  { return ELSE; }
"enum"                                  { return ENUM; }
"extern"                                { return EXTERN; }
"float"                                 { return FLOAT; }
"for"                                   { return FOR; }
"goto"                                  { return GOTO; }
"if"                                    { return IF; }
"inline"                                { return INLINE; }
"int"                                   { return INT; }
"long"                                  { return LONG; }
"register"                              { return REGISTER; }
"restrict"                              { return RESTRICT; }
"return"                                { return RETURN; }
"short"                                 { return SHORT; }
"signed"                                { return SIGNED; }
"sizeof"                                { return SIZEOF; }
"static"                                { return STATIC; }
"struct"                                { return STRUCT; }
"switch"                                { return SWITCH; }
"typedef"                               { return TYPEDEF; }
"union"                                 { return UNION; }
"unsigned"                              { return UNSIGNED; }
"void"                                  { return VOID; }
"volatile"                              { return VOLATILE; }
"while"                                 { return WHILE; }
"_Alignas"                              { return ALIGNAS; }
"_Alignof"                              { return ALIGNOF; }
"_Atomic"                               { return ATOMIC; }
"_Bool"                                 { return BOOL; }
"_Complex"                              { return COMPLEX; }
"_Generic"                              { return GENERIC; }
"_Imaginary"                            { return IMAGINARY; }
"_Noreturn"                             { return NORETURN; }
"_Static_assert"                        { return STATIC_ASSERT; }
"_Thread_local"                         { return THREAD_LOCAL; }
"__func__"                              { return FUNC_NAME; }

{identifier}                            {
    LOAD_STR;
    switch (sym_type(yytext)) {
    case TYPEDEF_NAME:                /* previously defined */
        return TYPEDEF_NAME;
    case ENUMERATION_CONSTANT:        /* previously defined */
        return ENUMERATION_CONSTANT;
    default:                          /* includes undefined */
        return IDENTIFIER;
    }
}

{hex}                                   { LOAD_INT; return I_CONSTANT; }
{decimal}                               { LOAD_INT; return I_CONSTANT; }
{octal}                                 { LOAD_INT; return I_CONSTANT; }
{char}                                  { LOAD_INT; return I_CONSTANT; }
{float1} |
{float2} |
{floatp1} |
{floatp2}                               { LOAD_FLOAT; return F_CONSTANT; }
{string}                                { LOAD_STR; return STRING_LITERAL; }

"..."                                   { return ELLIPSIS; }
">>="                                   { return RIGHT_ASSIGN; }
"<<="                                   { return LEFT_ASSIGN; }
"+="                                    { return ADD_ASSIGN; }
"-="                                    { return SUB_ASSIGN; }
"*="                                    { return MUL_ASSIGN; }
"/="                                    { return DIV_ASSIGN; }
"%="                                    { return MOD_ASSIGN; }
"&="                                    { return AND_ASSIGN; }
"^="                                    { return XOR_ASSIGN; }
"|="                                    { return OR_ASSIGN; }
">>"                                    { return RIGHT_OP; }
"<<"                                    { return LEFT_OP; }
"++"                                    { return INC_OP; }
"--"                                    { return DEC_OP; }
"->"                                    { return PTR_OP; }
"&&"                                    { return AND_OP; }
"||"                                    { return OR_OP; }
"<="                                    { return LE_OP; }
">="                                    { return GE_OP; }
"=="                                    { return EQ_OP; }
"!="                                    { return NE_OP; }
";"                                     { return ';'; }
("{"|"<%")                              { return '{'; }
("}"|"%>")                              { return '}'; }
","                                     { return ','; }
":"                                     { return ':'; }
"="                                     { return '='; }
"("                                     { return '('; }
")"                                     { return ')'; }
("["|"<:")                              { return '['; }
("]"|":>")                              { return ']'; }
"."                                     { return '.'; }
"&"                                     { return '&'; }
"!"                                     { return '!'; }
"~"                                     { return '~'; }
"-"                                     { return '-'; }
"+"                                     { return '+'; }
"*"                                     { return '*'; }
"/"                                     { return '/'; }
"%"                                     { return '%'; }
"<"                                     { return '<'; }
">"                                     { return '>'; }
"^"                                     { return '^'; }
"|"                                     { return '|'; }
"?"                                     { return '?'; }
{white}                                 { /* eat up whitespace */ }
.                                       {
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
