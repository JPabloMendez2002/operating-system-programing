#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define CANTIDAD_HILOS 11

int sudoku[9][9]; //---------------->Matriz para almacenar el sudoku

//----------------->Estructura para pasar múltiples argumentos a los hilos
typedef struct
{
    int hilo_ID;
    int fila_ID;
    int columna_ID;
} ThreadArgs;

//---------------------------->Función que valida si los números en una fila están dentro del rango del 1 al 9
void *MetodoValidar_FILAS(void *args)
{
    ThreadArgs *t_args = (ThreadArgs *)args;
    int hilo_ID = t_args->hilo_ID;
    int fila_ID = t_args->fila_ID;
    int columna_ID = t_args->columna_ID;
    int errores = 0, indice = 0;
    int i, j;
    int filas_falladas[9];

    for (int i = 0; i < 9; i++)//------------------------------------>Recorro columnas
    {
        int used[9] = {0}; 

        for (int j = 0; j < 9; j++)//------------------------------------>Recorro filas
        {
            int num = sudoku[i][j];

            if (num < 1 || num > 9 || used[num - 1])
            {
                filas_falladas[indice++] = columna_ID + 1;
                pthread_exit(NULL);
            }
            else
            {
                used[num - 1] = 1;
            }
        }
    }



    pthread_exit(NULL);
}

//--------------------->Función que valida si los números en una columna están dentro del rango del 1 al 9
void *MetodoValidar_COLUMNAS(void *args)
{
    ThreadArgs *t_args = (ThreadArgs *)args;
    int hilo_ID = t_args->hilo_ID;
    int columna_ID = t_args->columna_ID;
    int i, j;
    int errores = 0, indice = 0;
    int columnas_falladas[9];

    for (int i = 0; i < 9; i++)//------------------------------------>Recorro columnas
    {
        int used[9] = {0}; 

        for (j = 0; j < 9; j++)//------------------------------------>Recorro filas
        {
            int num = sudoku[i][j];

            if (num < 1 || num > 9 || used[num - 1])
            {
                columnas_falladas[indice++] = columna_ID + 1;
                pthread_exit(NULL);
            }
            else
            {
                used[num - 1] = 1;
            }
        }
    }

    pthread_exit(NULL);
}

//---------------->Función que valida si los números en una cuadrícula están dentro del rango del 1 al 9
void* MetodoValidar_CUADRICULA(void* args) {
    
    ThreadArgs* t_args = (ThreadArgs*)args;
    int hilo_ID = t_args->hilo_ID;
    int fila_ID = t_args->fila_ID;
    int columna_ID = t_args->columna_ID;
    printf("Cuadrícula No. %d, resultado: ",fila_ID * 3 + columna_ID + 1);
    int used[9] = {0};
    int filaVacia = -1; // variable para almacenar la fila del espacio vacío
    int columnaVacia = -1; // variable para almacenar la columna del espacio vacío
    int i, j;
    for (i = fila_ID * 3; i < fila_ID * 3 + 3; i++) {
        for (j = columna_ID * 3; j < columna_ID * 3 + 3; j++) {
            int num = sudoku[i][j];
            if (num == 0) {
                filaVacia = i;
                columnaVacia = j;
                continue;
            }
            if (used[num - 1]) {
                printf("Mal\n");
                pthread_exit(NULL);
            } else {
                used[num - 1] = 1;
            }
        }
    }
    if (filaVacia != -1 && columnaVacia != -1) {
        printf("Mal, la columna (%d) y la fila (%d) puede solucionarlo con el número ", filaVacia+1, columnaVacia+1);
        
        for (int k = 1; k <= 9; k++) {
            if (!used[k - 1]) {
                printf("%d\n", k);
                pthread_exit(NULL);
            }
        }
    } else {
        printf("Ok\n");
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{

    FILE *ArchivoE;
    //--------------------------------->Verifico que se haya proporcionado el nombre del archivo como argumento
    if (argc < 2)
    {
        printf("Error: Debe proporcionar el nombre del archivo como argumento.\n");
        return 1;
    }

    //--------------------------------------->Intento abrir el archivo para lectura
    ArchivoE = fopen(argv[1], "r");
    if (ArchivoE == NULL)
    {
        printf("Error: No se pudo abrir el archivo %s.\n", argv[1]);
        return 1;
    }

    //-------------------------------------->Leo el sudoku del archivo
    int i, j;
    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            fscanf(ArchivoE, "%d", &sudoku[i][j]);
        }
    }

    //------------------------------------>Creo los hilos para validar el sudoku
    pthread_t threads[CANTIDAD_HILOS];
    ThreadArgs args[CANTIDAD_HILOS];
    int t;
    for (t = 0; t < CANTIDAD_HILOS; t++)
    {
        args[t].hilo_ID = t + 1;
        if (t == 0)
        {
            args[t].fila_ID = 0;
            args[t].columna_ID = 0;
            pthread_create(&threads[t], NULL, MetodoValidar_FILAS, &args[t]);
        }
        else if (t == 1)
        {
            args[t].fila_ID = 0;
            args[t].columna_ID = 0;
            pthread_create(&threads[t], NULL, MetodoValidar_COLUMNAS, &args[t]);
        }
        else
        {
            args[t].fila_ID = (t - 2) / 3;
            args[t].columna_ID = (t - 2) % 3;
            pthread_create(&threads[t], NULL, MetodoValidar_CUADRICULA, &args[t]);
        }
    }

    //---------------------->Espero a que los hilos terminen
    for (t = 0; t < CANTIDAD_HILOS; t++)
    {
        pthread_join(threads[t], NULL);
    }

    //---------------->Cierro el archivo de sudoku
    fclose(ArchivoE);
   
    return 0;
}