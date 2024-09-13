#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>
#include <string.h>

#define MIN_PUNTOS 1500
#define MAX_PUNTOS 2000
#define CANT_APROX 5
#define MAX_HILOS 15

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int cant_hilos;
int puntos_totales = 0;
int puntos_dentro = 0;

int vector_ptos[CANT_APROX][MAX_HILOS];

void *calcular_pi(void *arg)
{
    int hilo = *((int *)arg);
    int cantidad_ptos;
    for (int i = 0; i < CANT_APROX; i++)
    {
        cantidad_ptos = vector_ptos[i][hilo];
        int puntos_circulo = 0;
        srand(time(NULL) + hilo + i);
        for (int j = 0; j < cantidad_ptos; j++)
        {
            double x = (double)rand() / (double)RAND_MAX;
            double y = (double)rand() / (double)RAND_MAX;
            double distancia = sqrt(x * x + y * y);
            if (distancia <= 1.0)
            {
                puntos_circulo++;
            }
        }
        pthread_mutex_lock(&mutex);
        puntos_totales += cantidad_ptos;
        puntos_dentro += puntos_circulo;
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}

void generar_vector_ptos()
{
    srand(time(NULL));
    for (int i = 0; i < CANT_APROX; i++)
    {
        for (int j = 0; j < cant_hilos; j++)
        {
            vector_ptos[i][j] = rand() % (MAX_PUNTOS - MIN_PUNTOS + 1) + MIN_PUNTOS;
        }
    }
}

void escribir_consola()
{
    //----------------------->IMPRIMO EN CONSOLA LOS RESULTADOS
    printf("\n");
    printf("Hilos generados: %d\n", cant_hilos);
    printf("Vector cantidad de puntos: Vector_ptos [");
    for (int i = 0; i < cant_hilos; i++)
    {
        printf("%d", vector_ptos[0][i]);
        if (i < cant_hilos - 1)
        {
            printf(", ");
        }
    }
    printf("]\n");

    //---------------------------------->CALCULO DE LA 5TA APROX DE PI
    int cantidad_ptos_5ta_aprox = 0;
    int puntos_circulo_5ta_aprox = 0;
    for (int i = 0; i < cant_hilos; i++)
    {
        cantidad_ptos_5ta_aprox += vector_ptos[4][i];
    }
    for (int i = 0; i < cant_hilos; i++)
    {
        puntos_circulo_5ta_aprox += vector_ptos[4][i] * (double)puntos_dentro / puntos_totales;
    }
    double pi_5ta_aprox = 4.0 * puntos_circulo_5ta_aprox / cantidad_ptos_5ta_aprox;
    printf("Puntos dentro del círculo (5ta aprox.) = %d\n", puntos_circulo_5ta_aprox);
    printf("Puntos totales generados (5ta aprox.) = %d\n", cantidad_ptos_5ta_aprox);
    // ---------------------------------->CALCULO DE APROXS DE PI
    double pi_aprox[CANT_APROX];
    for (int i = 0; i < CANT_APROX; i++)
    {
        int cantidad_ptos_aprox = 0;
        int puntos_circulo_aprox = 0;
        for (int j = 0; j < cant_hilos; j++)
        {
            cantidad_ptos_aprox += vector_ptos[i][j];
            puntos_circulo_aprox += vector_ptos[i][j] * (double)puntos_dentro / puntos_totales;
        }
        pi_aprox[i] = 4.0 * puntos_circulo_aprox / cantidad_ptos_aprox;
    }
    printf("Primera aproximación de PI : %.4f\n", pi_aprox[0]);
    printf("Segunda aproximación de PI : %.4f\n", pi_aprox[1]);
    printf("Tercera aproximación de PI : %.4f\n", pi_aprox[2]);
    printf("Cuarta aproximación de PI : %.4f\n", pi_aprox[3]);
    printf("Quinta aproximación de PI : %.4f\n", pi_aprox[4]);

    // ---------------------------------->CALCULO DEL PROMEDIO Y DESVIACION
    double promedio = 0;
    for (int i = 0; i < CANT_APROX; i++)
    {
        promedio += pi_aprox[i];
    }
    promedio /= CANT_APROX;
    double desv_est = 0;
    for (int i = 0; i < CANT_APROX; i++)
    {
        desv_est += pow(pi_aprox[i] - promedio, 2);
    }
    desv_est = sqrt(desv_est / CANT_APROX);

    printf("Promedio del cálculo : %.4f\n", promedio);
    printf("Desviación Estándar : %.4f\n", desv_est);

    //----------------------->FIN DE IMPRIMIR EN CONSOLA
}

void escribir_archivo(char *nombre_archivo, char *modo)
{
    FILE *archivo = fopen(nombre_archivo, modo);
    fprintf(archivo, "Hilos generados: %d\n", cant_hilos);
    fprintf(archivo, "Vector cantidad de puntos: Vector_ptos [");
    for (int i = 0; i < cant_hilos; i++)
    {
        fprintf(archivo, "%d", vector_ptos[0][i]);
        if (i < cant_hilos - 1)
        {
            fprintf(archivo, ", ");
        }
    }
    fprintf(archivo, "]\n");

    //------------------------------------>CALCULO DE LA 5TA APROX DE PI
    int cantidad_ptos_5ta_aprox = 0;
    int puntos_circulo_5ta_aprox = 0;
    for (int i = 0; i < cant_hilos; i++)
    {
        cantidad_ptos_5ta_aprox += vector_ptos[4][i];
    }
    for (int i = 0; i < cant_hilos; i++)
    {
        puntos_circulo_5ta_aprox += vector_ptos[4][i] * (double)puntos_dentro / puntos_totales;
    }
    double pi_5ta_aprox = 4.0 * puntos_circulo_5ta_aprox / cantidad_ptos_5ta_aprox;
    fprintf(archivo, "Puntos dentro del círculo (5ta aprox.) = %d\n", puntos_circulo_5ta_aprox);
    fprintf(archivo, "Puntos totales generados (5ta aprox.) = %d\n", cantidad_ptos_5ta_aprox);
    // ---------------------------------->CALCULO DE APROXS DE PI
    double pi_aprox[CANT_APROX];
    for (int i = 0; i < CANT_APROX; i++)
    {
        int cantidad_ptos_aprox = 0;
        int puntos_circulo_aprox = 0;
        for (int j = 0; j < cant_hilos; j++)
        {
            cantidad_ptos_aprox += vector_ptos[i][j];
            puntos_circulo_aprox += vector_ptos[i][j] * (double)puntos_dentro / puntos_totales;
        }
        pi_aprox[i] = 4.0 * puntos_circulo_aprox / cantidad_ptos_aprox;
    }
    fprintf(archivo, "Primera aproximación de PI : %.4f\n", pi_aprox[0]);
    fprintf(archivo, "Segunda aproximación de PI : %.4f\n", pi_aprox[1]);
    fprintf(archivo, "Tercera aproximación de PI : %.4f\n", pi_aprox[2]);
    fprintf(archivo, "Cuarta aproximación de PI : %.4f\n", pi_aprox[3]);
    fprintf(archivo, "Quinta aproximación de PI : %.4f\n", pi_aprox[4]);

    // ---------------------------------->CALCULO DEL PROMEDIO Y DESVIACION
    double promedio = 0;
    for (int i = 0; i < CANT_APROX; i++)
    {
        promedio += pi_aprox[i];
    }
    promedio /= CANT_APROX;
    double desv_est = 0;
    for (int i = 0; i < CANT_APROX; i++)
    {
        desv_est += pow(pi_aprox[i] - promedio, 2);
    }
    desv_est = sqrt(desv_est / CANT_APROX);

    fprintf(archivo, "Promedio del cálculo : %.4f\n", promedio);
    fprintf(archivo, "Desviación Estándar : %.4f\n", desv_est);
    fprintf(archivo, "--------------------------------------\n");
    fclose(archivo);
}

int main(int argc, char *argv[])
{
    FILE *archivoS;
    char *nombre_archivoS;
    char *modo_archivoS;


//--------------------------------------------------------------->VALIDACIONES DE LOS PARAMETOS
    if (argc < 2)
    {
        printf("Error: cantidad de hilos no especificada\n");
        exit(-1);
    }
    else if (argc < 3)
    {
        printf("Error: Debe ingresar el nombre del archivo de salida como parámetro.\n");
        return 1;
    }
    else if (argc < 4)
    {
        printf("Error: Indique si es Historico '-h' o No Historico '-u' .\n");
        return 1;
    }

    cant_hilos = atoi(argv[1]);
    if (cant_hilos < 10 || cant_hilos > 15)
    {
        printf("Error: cantidad de hilos fuera de rango\n");
        exit(-1);
    }

    generar_vector_ptos();//------------------------------------>METODO QUE GENERA LOS VECTORES

    pthread_t hilos[cant_hilos];
    int ids[cant_hilos];

    for (int i = 0; i < cant_hilos; i++)
    {
        ids[i] = i;
        pthread_create(&hilos[i], NULL, calcular_pi, (void *)&ids[i]);
    }

    for (int i = 0; i < cant_hilos; i++)
    {
        pthread_join(hilos[i], NULL);

    //------------------------------------------------>VALIDO LOS PARAMETROS DE SALIDA
    if (argc > 3 && strcmp(argv[3], "-h") == 0) {
        modo_archivoS = "a";
    } else if (argc > 3 && strcmp(argv[3], "-u") == 0) {
        modo_archivoS = "w";
    } else {
        printf("Error: El tercer parámetro UNICAMENTE debe ser '-h' o '-u'.\n");
        return 1;
    }
    }

    escribir_consola();//------------------------------------>METODO QUE ESCRIBE EN CONSOLA LOS RESULTADOS

    nombre_archivoS = argv[2];
    escribir_archivo(nombre_archivoS, modo_archivoS);


    return 0;
}
