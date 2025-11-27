/* lexer.l */
%{
#include <stdio.h>
#include <stdlib.h>
%}

/* Definiciones regulares para simplificar las reglas */
DIGIT       [0-9]
LETTER      [a-zA-Z_]
ID          {LETTER}({LETTER}|{DIGIT})*
NUMBER      {DIGIT}+(\.{DIGIT}+)?

/* Estado exclusivo para manejar comentarios de bloque multilínea */
%x BLOCK_COMMENT

%%

    /* --- REGLAS DE COMENTARIOS --- */
"/*"            { BEGIN(BLOCK_COMMENT); } /* Inicia estado comentario */
<BLOCK_COMMENT>"*/" { BEGIN(INITIAL); }   /* Termina estado comentario */
<BLOCK_COMMENT>(.|\n) { /* Ignorar todo dentro del comentario */ }

"//".* { /* Ignorar comentarios de una sola línea */ }

    /* --- PALABRAS RESERVADAS --- */
"int"           { printf("PALABRA_RESERVADA: %s\n", yytext); }
"float"         { printf("PALABRA_RESERVADA: %s\n", yytext); }
"double"        { printf("PALABRA_RESERVADA: %s\n", yytext); }
"char"          { printf("PALABRA_RESERVADA: %s\n", yytext); }
"void"          { printf("PALABRA_RESERVADA: %s\n", yytext); }
"short"         { printf("PALABRA_RESERVADA: %s\n", yytext); }
"return"        { printf("PALABRA_RESERVADA: %s\n", yytext); }
"include"       { printf("PALABRA_RESERVADA: %s\n", yytext); }
"define"        { printf("PALABRA_RESERVADA: %s\n", yytext); }

    /* --- IDENTIFICADORES --- */
{ID}            { printf("IDENTIFICADOR: %s\n", yytext); }

    /* --- LITERALES NUMÉRICOS --- */
{NUMBER}        { printf("NUMERO: %s\n", yytext); }

    /* --- OPERADORES --- */
"++"            { printf("OPERADOR: %s\n", yytext); } /* ++ antes de + */
"+"             { printf("OPERADOR: %s\n", yytext); }
"-"             { printf("OPERADOR: %s\n", yytext); }
"*"             { printf("OPERADOR: %s\n", yytext); }
"/"             { printf("OPERADOR: %s\n", yytext); }
"="             { printf("OPERADOR: %s\n", yytext); }

    /* --- DELIMITADORES --- */
"("             { printf("DELIMITADOR: %s\n", yytext); }
")"             { printf("DELIMITADOR: %s\n", yytext); }
"{"             { printf("DELIMITADOR: %s\n", yytext); }
"}"             { printf("DELIMITADOR: %s\n", yytext); }
";"             { printf("DELIMITADOR: %s\n", yytext); }
","             { printf("DELIMITADOR: %s\n", yytext); }

    /* --- CARACTERES ESPECIALES (Para que compile el ejemplo) --- */
"#"             { printf("CARACTER_ESP: %s\n", yytext); }
"<"             { printf("DELIMITADOR: %s\n", yytext); }
">"             { printf("DELIMITADOR: %s\n", yytext); }
"."             { printf("DELIMITADOR: %s\n", yytext); }

    /* --- IGNORAR ESPACIOS EN BLANCO Y SALTOS DE LÍNEA --- */
[ \t\n\r]+      { /* Ignorar */ }

    /* --- CUALQUIER OTRA COSA (ERRORES) --- */
.               { printf("ERROR_LEXICO: %s\n", yytext); }

%%

/* Código de usuario (Función Main) */
int main(int argc, char **argv) {
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            perror("Error al abrir el archivo");
            return 1;
        }
    } else {
        printf("Uso: ./lexer <archivo_entrada>\n");
        return 1;
    }
    
    yylex();
    return 0;
}

int yywrap() {
    return 1;
}