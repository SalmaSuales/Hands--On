# Analizador Sintáctico para Subconjunto de C

Este proyecto implementa un analizador léxico y sintáctico utilizando **Flex** y **Bison**.
Valida la estructura gramatical de programas escritos en un subconjunto del lenguaje C.

## Gramática Definida (Producciones Principales)
El analizador valida las siguientes estructuras:

1.  **Declaraciones Globales y Locales:** `tipo identificador;`
2.  **Funciones:** `tipo nombre(params) { cuerpo }`
3.  **Bloques:** `{ sentencias }` (soporta anidamiento).
4.  **Sentencias Soportadas:**
    * Asignaciones (`a = b + 5;`)
    * Retornos (`return x;`)
    * Llamadas a función (`fun(a,b);`)
    * Instrucción especial `print(x);`
5.  **Directivas Preprocesador:** Soporte básico para `#include` y `#define`.

## Compilación y Ejecución
Requisitos: `flex`, `bison`, `gcc`.

```bash
# 1. Generar el parser (crea parser.tab.c y parser.tab.h)
bison -d parser.y

# 2. Generar el lexer (crea lex.yy.c)
flex lexer.l

# 3. Compilar todo junto
gcc parser.tab.c lex.yy.c -o compilador

# 4. Ejecutar
./compilador input.c
