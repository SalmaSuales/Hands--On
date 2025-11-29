# Analizador Sintáctico y Semántico para Subconjunto de C

## Integrantes del Equipo:
- Salma Yanet Mercado Suales - 218551772
- García Herrera Juan Pablo - 214634843

## Alcances del Análisis Semántico Implementado

### Validaciones Implementadas:

1. **Gestión de Scopes Anidados**
   - Scope global para variables y funciones globales
   - Scopes locales por función
   - Scopes anidados por bloques `{}`
   - Pila de scopes para manejar entrada/salida de bloques

2. **Detección de Variables No Declaradas**
   - Validación en asignaciones y expresiones
   - Búsqueda en scopes anidados (desde local hasta global)

3. **Prevención de Redeclaraciones**
   - No se permite redeclarar variables en el mismo scope
   - Se permiten variables con mismo nombre en scopes diferentes

4. **Control de Uso de Variables**
   - Advertencia para variables declaradas pero no usadas
   - Seguimiento de declaración vs uso

5. **Validación Básica de Funciones**
   - Detección de funciones no declaradas
   - Distinción entre variables y funciones

### Estructuras de Datos Implementadas:

```c
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