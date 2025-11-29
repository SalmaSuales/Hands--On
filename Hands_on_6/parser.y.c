%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Declaramos funciones externas y variables
extern int yylex();
extern int yylineno;
extern char *yytext;
void yyerror(const char *s);

/* --- ESTRUCTURAS PARA ANÁLISIS SEMÁNTICO --- */

typedef struct Symbol {
    char name[100];
    char type[50];
    int is_function;
    int param_count;
    int declared;
    int used;
    struct Symbol *next;
} Symbol;

typedef struct SymbolTable {
    Symbol *symbols;
    struct SymbolTable *parent;
    struct SymbolTable *next;
} SymbolTable;

// Variables globales para gestión de scopes
SymbolTable *global_table = NULL;
SymbolTable *current_scope = NULL;
SymbolTable *scope_stack[100];
int scope_stack_top = -1;

// Funciones para gestión de símbolos y scopes
void push_scope();
void pop_scope();
Symbol* declare_symbol(const char *name, const char *type, int is_function, int param_count);
Symbol* find_symbol(const char *name);
Symbol* find_symbol_current_scope(const char *name);
void check_symbol_usage();
void semantic_error(const char *message, const char *symbol_name);

%}

/* Definición de Tokens (Terminales) que vienen del Lexer */
%token INT FLOAT DOUBLE CHAR VOID RETURN PRINT
%token INCLUDE DEFINE
%token ID NUMBER STRING_LITERAL
%token PLUS MINUS MULT DIV ASSIGN
%token LPAREN RPAREN LBRACE RBRACE SEMICOLON COMMA
%token EQ NEQ GT LT

/* Precedencia de operadores */
%left PLUS MINUS
%left MULT DIV

/* Símbolo inicial de la gramática */
%start program

%%

/* --- REGLAS GRAMATICALES CON ACCIONES SEMÁNTICAS --- */

program:
    /* Vacío */ {
        // Inicializar tabla global al comenzar el programa
        global_table = (SymbolTable*)malloc(sizeof(SymbolTable));
        global_table->symbols = NULL;
        global_table->parent = NULL;
        global_table->next = NULL;
        current_scope = global_table;
        scope_stack_top = -1;
        push_scope(); // Scope global
    }
    | program element
    ;

element:
    var_declaration
    | fun_declaration
    | preprocessor_directive
    ;

preprocessor_directive:
    INCLUDE STRING_LITERAL      { printf("-> Directiva Include detectada\n"); }
    | INCLUDE '<' ID '.' ID '>' { printf("-> Directiva Include std detectada\n"); }
    | DEFINE ID NUMBER          { 
        printf("-> Directiva Define detectada: %s\n", $2);
        declare_symbol($2, "MACRO", 0, 0);
    }
    ;

var_declaration:
    type_specifier ID SEMICOLON { 
        printf("-> Declaración de variable: %s\n", $2);
        Symbol *existing = find_symbol_current_scope($2);
        if (existing != NULL) {
            semantic_error("Redeclaración de variable", $2);
        } else {
            declare_symbol($2, $1, 0, 0);
        }
    }
    ;

fun_declaration:
    type_specifier ID LPAREN param_list RPAREN block {
        printf("-> Definición de función completada: %s\n", $2);
        // Marcar la función como usada (se llamó a sí misma durante la definición)
        Symbol *func = find_symbol($2);
        if (func != NULL) {
            func->used = 1;
        }
        pop_scope(); // Salir del scope de la función
    }
    ;

param_list:
    /* vacío */ { $$ = 0; } // 0 parámetros
    | param_nonempty { $$ = $1; }
    ;

param_nonempty:
    type_specifier ID { 
        $$ = 1;
        declare_symbol($2, $1, 0, 0);
    }
    | param_nonempty COMMA type_specifier ID { 
        $$ = $1 + 1;
        declare_symbol($4, $3, 0, 0);
    }
    ;

type_specifier:
    INT { strcpy($$, "int"); }
    | FLOAT { strcpy($$, "float"); }
    | DOUBLE { strcpy($$, "double"); }
    | CHAR { strcpy($$, "char"); }
    | VOID { strcpy($$, "void"); }
    ;

block:
    LBRACE { push_scope(); } statement_list RBRACE { pop_scope(); }
    ;

statement_list:
    /* vacío */
    | statement_list statement
    ;

statement:
    var_declaration
    | assignment
    | fun_call_stmt
    | return_stmt
    | print_stmt
    | block  /* Bloques anidados */
    ;

assignment:
    ID ASSIGN expression SEMICOLON {
        Symbol *sym = find_symbol($1);
        if (sym == NULL) {
            semantic_error("Variable no declarada", $1);
        } else {
            sym->used = 1; // Marcar como usada
        }
    }
    ;

fun_call_stmt:
    ID LPAREN arg_list RPAREN SEMICOLON {
        Symbol *func = find_symbol($1);
        if (func == NULL) {
            semantic_error("Función no declarada", $1);
        } else if (!func->is_function) {
            semantic_error("No es una función", $1);
        } else {
            func->used = 1;
            // Aquí se podría validar número de parámetros si se guardó esa info
        }
    }
    ;

fun_call_expr:
    ID LPAREN arg_list RPAREN {
        Symbol *func = find_symbol($1);
        if (func == NULL) {
            semantic_error("Función no declarada", $1);
        } else if (!func->is_function) {
            semantic_error("No es una función", $1);
        } else {
            func->used = 1;
        }
    }
    ;

print_stmt:
    PRINT LPAREN expression RPAREN SEMICOLON
    ;

return_stmt:
    RETURN expression SEMICOLON
    ;

arg_list:
    /* vacío */ { $$ = 0; }
    | arg_nonempty { $$ = $1; }
    ;

arg_nonempty:
    expression { $$ = 1; }
    | arg_nonempty COMMA expression { $$ = $1 + 1; }
    ;

expression:
    NUMBER
    | ID {
        Symbol *sym = find_symbol($1);
        if (sym == NULL) {
            semantic_error("Variable no declarada", $1);
        } else {
            sym->used = 1;
        }
    }
    | fun_call_expr
    | expression PLUS expression
    | expression MINUS expression
    | expression MULT expression
    | expression DIV expression
    | LPAREN expression RPAREN
    ;

%%

/* --- IMPLEMENTACIÓN FUNCIONES SEMÁNTICAS --- */

void push_scope() {
    SymbolTable *new_scope = (SymbolTable*)malloc(sizeof(SymbolTable));
    new_scope->symbols = NULL;
    new_scope->parent = current_scope;
    new_scope->next = NULL;
    
    scope_stack[++scope_stack_top] = current_scope;
    current_scope = new_scope;
}

void pop_scope() {
    if (scope_stack_top >= 0) {
        // Verificar uso de variables en el scope que se está cerrando
        Symbol *sym = current_scope->symbols;
        while (sym != NULL) {
            if (!sym->used && !sym->is_function) {
                printf("Advertencia: Variable '%s' declarada pero no usada\n", sym->name);
            }
            sym = sym->next;
        }
        
        current_scope = scope_stack[scope_stack_top--];
    }
}

Symbol* declare_symbol(const char *name, const char *type, int is_function, int param_count) {
    Symbol *new_symbol = (Symbol*)malloc(sizeof(Symbol));
    strcpy(new_symbol->name, name);
    strcpy(new_symbol->type, type);
    new_symbol->is_function = is_function;
    new_symbol->param_count = param_count;
    new_symbol->declared = 1;
    new_symbol->used = 0;
    new_symbol->next = current_scope->symbols;
    current_scope->symbols = new_symbol;
    return new_symbol;
}

Symbol* find_symbol(const char *name) {
    SymbolTable *scope = current_scope;
    while (scope != NULL) {
        Symbol *sym = scope->symbols;
        while (sym != NULL) {
            if (strcmp(sym->name, name) == 0) {
                return sym;
            }
            sym = sym->next;
        }
        scope = scope->parent;
    }
    return NULL;
}

Symbol* find_symbol_current_scope(const char *name) {
    Symbol *sym = current_scope->symbols;
    while (sym != NULL) {
        if (strcmp(sym->name, name) == 0) {
            return sym;
        }
        sym = sym->next;
    }
    return NULL;
}

void semantic_error(const char *message, const char *symbol_name) {
    fprintf(stderr, "\033[1;31mError semántico en línea %d: %s - '%s'\033[0m\n", 
            yylineno, message, symbol_name);
}

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
    
    printf("Iniciando análisis sintáctico y semántico...\n");
    if (yyparse() == 0) {
        printf("\n\033[1;32m>> El análisis finalizó exitosamente.\033[0m\n");
        
        // Verificar uso de variables globales
        Symbol *sym = global_table->symbols;
        while (sym != NULL) {
            if (!sym->used && !sym->is_function) {
                printf("Advertencia: Variable global '%s' declarada pero no usada\n", sym->name);
            }
            sym = sym->next;
        }
    } else {
        printf("\n\033[1;31m>> Falló el análisis.\033[0m\n");
    }
    return 0;
}