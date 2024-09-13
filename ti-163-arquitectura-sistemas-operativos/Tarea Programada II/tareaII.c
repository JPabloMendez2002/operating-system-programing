
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

double Calcularcoseno(double coseno, int precision);

int main(int argc, char *argv[]) {
    double coseno;
    int precision;
    char *archivo_salida;
    FILE*archivo;
    

    if (argc != 4) {//--------------------------------------->Validacion para pedirle al usuario que ingrese los 3 parametros minimos
        printf("**Error: Debe ingresar al menos tres parámetros.\n");
        printf("Parámetro [1]: Coseno en un rango de 0 a 2pi.\n");
        printf("Parámetro [2]: Precisión de la fórmula en un rango de 3 a 6, número entero.\n");
        printf("Parámetro [3]: Nombre del archivo de salida en formato TXT.\n");
        return 1;
    }
    
    coseno = atof(argv[1]);
    precision = atoi(argv[2]);
    archivo_salida = argv[3];

 
    if (coseno < 0 || coseno > 6.28318530718) {//--------------------------------------->Validacion para el ingreso dentro del rango 0 a 2pi
        printf("**Error: El primer parámetro debe estar en el rango [0, 2pi].\n");
        return 1;
    }
    
    if (precision < 3 || precision > 6) {//--------------------------------------->Validacion para que la precision este entre 3 y 6 
        printf("**Error: El segundo parámetro debe ser un entero en el rango [3, 6].\n");
        return 1;
    }
    
 
    double resultado = Calcularcoseno(coseno, precision);//----------->LLamo al metodo que hace el calculo y le paso los 2 parametros que solicita
    
    
    printf(" \n El coseno de %f con una precisión de %d elementos es: %.5f.\n", coseno, precision, resultado);//---------->Print en consola del resultado

    
    
    archivo = fopen(archivo_salida, "a");//-------------->Abrimos el archivo o lo creamos para escibir los resultados
    if (archivo == NULL) {
        printf("**Error: no se pudo abrir el archivo %s.\n", archivo_salida);
        return 1;
    }
    fprintf(archivo, " \n El coseno de %f con una precisión de %d elementos es: %.5f.\n", coseno, precision, resultado);
    fclose(archivo);
    return 0;
}

double Calcularcoseno(double coseno, int precision) {
    double resultado = 1.0;
    double signo = -1.0;
    double potencia = 1.0;
    double factorial = 1.0; //------------------>Declaramos variables 
    int i, j;
    for (i = 1; i < precision; i++) { //---------->Con el primer for incremento de uno en uno si la precision es mayor a 1
        for (j = 1; j <= 2*i; j++) {//Con el segundo for segun el valor de I aplico la formula para el calculo
            potencia *= coseno;
            factorial *= j;
        }
        resultado += signo * potencia / factorial;//--------->Finalmente obtengo el resultado
        signo *= -1.0;
        potencia = 1.0;
        factorial = 1.0;
    }
    return resultado;
}


