#include <stdlib.h>
#define SCALE_FACTOR 2

int globalA;
int globalB;

int addValues(int first, int second) {
    int resultLocal;
    resultLocal = first + second;
    return resultLocal;
}

int processValue(int value) {
    int temporaryVal;
    temporaryVal = value * SCALE_FACTOR;
    { /* nested block */
        int innerResult;
        innerResult = temporaryVal + 5;
        print(innerResult);
    }
    return temporaryVal;
}

int main() {
    int resultMain;
    int auxValue;
    globalA = 3;
    globalB = 4;
    
    resultMain = addValues(globalA, globalB);
    print(resultMain);
    auxValue = processValue(resultMain);
    print(auxValue);
    {
        int finalOutput;
        finalOutput = auxValue + resultMain;
        print(finalOutput);
    }
    
    /* Pruebas de errores semánticos */
    int duplicateVar;    // OK
    int duplicateVar;    // ERROR: Redeclaración
    
    undeclaredVar = 10;  // ERROR: Variable no declarada
    
    return 0;
}