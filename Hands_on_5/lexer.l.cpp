%{
#include <stdio.h>
#include <stdlib.h>
#include "parser.tab.h" /* Importante: Incluir tokens generados por Bison */
%}

%option yylineno
%option noyywrap

/* Definiciones */
DIGIT       [0-9]
LETTER      [a-zA-Z_]
ID          {LETTER}({LETTER}|{DIGIT})*
NUMBER      {DIGIT}+(\.{DIGIT}+)?

%x BLOCK_COMMENT

%%

"/*"            { BEGIN(BLOCK_COMMENT); }
<BLOCK_COMMENT>"*/" { BEGIN(INITIAL); }
<BLOCK_COMMENT>(.|\n) { /* Ignorar contenido comentario */ }
"//".* { /* Ignorar comentario linea */ }

"int"           { return INT; }
"float"         { return FLOAT; }
"double"        { return DOUBLE; }
"char"          { return CHAR; }
"void"          { return VOID; }
"return"        { return RETURN; }
"print"         { return PRINT; }
"#include"      { return INCLUDE; }
"#define"       { return DEFINE; }

{ID}            { return ID; }
{NUMBER}        { return NUMBER; }
\"[^\"]*\"      { return STRING_LITERAL; }

"+"             { return PLUS; }
"-"             { return MINUS; }
"*"             { return MULT; }
"/"             { return DIV; }
"="             { return ASSIGN; }
";"             { return SEMICOLON; }
","             { return COMMA; }
"("             { return LPAREN; }
")"             { return RPAREN; }
"{"             { return LBRACE; }
"}"             { return RBRACE; }
"<"             { return '<'; }
">"             { return '>'; }
"."             { return '.'; }

[ \t\r]+        { /* Ignorar espacios */ }
\n              { /* Ignorar saltos (yylineno los cuenta solo) */ }

.               { printf("Error léxico: Caracter desconocido '%s' en linea %d\n", yytext, yylineno); }

%%