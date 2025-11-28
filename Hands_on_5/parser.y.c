%{
#include <stdio.h>
#include <stdlib.h>

// Declaramos funciones externas y variables
extern int yylex();
extern int yylineno;
extern char *yytext;
void yyerror(const char *s);

%}

/* Definición de Tokens (Terminales) que vienen del Lexer */
%token INT FLOAT DOUBLE CHAR VOID RETURN PRINT
%token INCLUDE DEFINE
%token ID NUMBER STRING_LITERAL
%token PLUS MINUS MULT DIV ASSIGN
%token LPAREN RPAREN LBRACE RBRACE SEMICOLON COMMA
%token EQ NEQ GT LT

/* Precedencia de operadores (para evitar ambigüedad en restas/divisiones) */
%left PLUS MINUS
%left MULT DIV

/* Símbolo inicial de la gramática */
%start program

%%

/* --- REGLAS GRAMATICALES (Producciones) --- */

program:
    /* Vacío */
    | program element
    ;

element:
    var_declaration
    | fun_declaration
    | preprocessor_directive
    ;

/* Directivas de preprocesador (#include, #define) */
preprocessor_directive:
    INCLUDE STRING_LITERAL      { printf("-> Directiva Include detectada\n"); }
    | INCLUDE '<' ID '.' ID '>' { printf("-> Directiva Include std detectada\n"); }
    | DEFINE ID NUMBER          { printf("-> Directiva Define detectada\n"); }
    ;

/* Declaración de variables: int x; */
var_declaration:
    type_specifier ID SEMICOLON { 
        printf("-> Declaración de variable global/local: %s\n", $2); 
    }
    ;

/* Declaración de funciones: int main() { ... } */
fun_declaration:
    type_specifier ID LPAREN param_list RPAREN block {
        printf("-> Definición de función completada: %s\n", $2);
    }
    ;

/* Lista de parámetros: (int a, int b) o vacío */
param_list:
    /* vacío */
    | param_nonempty
    ;

param_nonempty:
    type_specifier ID
    | param_nonempty COMMA type_specifier ID
    ;

/* Tipos de datos soportados */
type_specifier:
    INT | FLOAT | DOUBLE | CHAR | VOID
    ;

/* Bloque de código: { sentencias } */
block:
    LBRACE statement_list RBRACE
    ;

statement_list:
    /* vacío */
    | statement_list statement
    ;

/* Tipos de sentencias dentro de un bloque */
statement:
    var_declaration
    | assignment
    | fun_call_stmt
    | return_stmt
    | print_stmt
    | block  /* Bloques anidados */
    ;

/* Asignación: x = 5 + y; */
assignment:
    ID ASSIGN expression SEMICOLON
    ;

/* Llamada a función como sentencia: funcion(x); */
fun_call_stmt:
    ID LPAREN arg_list RPAREN SEMICOLON
    ;

/* Llamada a función dentro de una expresión (sin punto y coma) */
fun_call_expr:
    ID LPAREN arg_list RPAREN
    ;

/* Print especial del ejemplo: print(x); */
print_stmt:
    PRINT LPAREN expression RPAREN SEMICOLON
    ;

/* Return: return 0; */
return_stmt:
    RETURN expression SEMICOLON
    ;

/* Argumentos de llamada: (a, b + 5) */
arg_list:
    /* vacío */
    | arg_nonempty
    ;

arg_nonempty:
    expression
    | arg_nonempty COMMA expression
    ;

/* Expresiones Aritméticas */
expression:
    NUMBER
    | ID
    | fun_call_expr
    | expression PLUS expression
    | expression MINUS expression
    | expression MULT expression
    | expression DIV expression
    | LPAREN expression RPAREN
    ;

%%

/* --- CÓDIGO C DE SOPORTE --- */

void yyerror(const char *s) {
    fprintf(stderr, "\033[1;31mError de sintaxis en la línea %d: %s cerca de '%s'\033[0m\n", yylineno, s, yytext);
}

int main(int argc, char **argv) {
    if (argc > 1) {
        extern FILE *yyin;
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            perror("Error al abrir archivo");
            return 1;
        }
    }
    
    printf("Iniciando análisis sintáctico...\n");
    if (yyparse() == 0) {
        printf("\n\033[1;32m>> El análisis sintáctico finalizó exitosamente. El código es válido.\033[0m\n");
    } else {
        printf("\n\033[1;31m>> Falló el análisis.\033[0m\n");
    }
    return 0;
}