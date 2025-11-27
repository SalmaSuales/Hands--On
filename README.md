# Hands--On 4

Integrantes del Equipo:

Salma Yanet Mercado Suales
  correo: Salma.mercado5517@alumnos.udg.mx
  codigo: 218551772
  
Garcia Herrera Juan Pablo
	correo: juan.garcia3484@alumnos.udg.mx
	código: 214634843


Descripción
Este proyecto implementa un analizador léxico (lexer) capaz de reconocer los tokens fundamentales de un subconjunto del lenguaje C. El lexer procesa un flujo de caracteres de entrada y identifica los diferentes elementos léxicos según las especificaciones del lenguaje.

Capacidades del Analizador Léxico
Tokens Reconocidos
  Palabras Reservadas
Tipos de datos: int, float, double, char, void, short

Control: return

Preprocesador: include, define

  Identificadores
Nombres de variables, funciones, constantes y macros

Ejemplos: globalA, addValues, SCALE_FACTOR

  Literales Numéricos
Números enteros y decimales

Ejemplos: 123, 45.67

  Operadores
Aritméticos: +, -, *, /

Incremento: ++

Asignación: =

  Delimitadores
Paréntesis: (, )

Llaves: {, }

Punto y coma: ;

Coma: ,

Otros: <, >, ., #

  Comentarios
Comentarios de una línea: //

Comentarios de bloque: /* ... */

  Manejo de Errores
Detecta y reporta caracteres no reconocidos

Instrucciones de Compilación y Ejecución
Prerrequisitos
Flex (Fast Lexical Analyzer)

Compilador de C (gcc)

Compilación

Pasos para ejecutar en terminal de kali linux:
1. Crear el archivo del analizador
Escribe este comando y dale Enter:

Bash

nano lexer.l
(Se abrirá una pantalla vacía).

Copia el primer código, lexer.l

Pégalo ahí dentro.

Guarda presionando: Ctrl + O, luego Enter, y sales con Ctrl + X.

2. Crear el archivo de prueba
Escribe este comando y dale Enter:

Bash

nano input.c
(Se abrirá otra pantalla vacía).

Copia el segundo código  input.c

Pégalo ahí dentro.

Guarda igual: Ctrl + O, Enter, Ctrl + X.

3. Ejecuta
Ahora que los archivos realmente existen en esa carpeta, copia y pega esto para ejecutar todo:

Bash

flex lexer.l && gcc lex.yy.c -o analizador && ./analizador input.c
