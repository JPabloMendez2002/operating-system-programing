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
                printf("Hilo No. %d revisa fila No. %d, resultado: ", hilo_ID, j + 1);//------------------------------------>Imprimo la fila mala
                printf("Mal\n");
                filas_falladas[indice++] = columna_ID + 1;
                pthread_exit(NULL);
            }
            else
            {
                used[num - 1] = 1;
            }
        }
    }

    if (errores == 0)
    {
        printf("Hilo No. %d reviso TODAS las filas resultado: Ok\n", hilo_ID);
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
                printf("Hilo No. %d revisa columna No. %d, resultado: ", hilo_ID, i + 1);//------------------------------------>Imprimo la columna malaa
                printf("Mal\n");
                columnas_falladas[indice++] = columna_ID + 1;
                pthread_exit(NULL);
            }
            else
            {
                used[num - 1] = 1;
            }
        }
    }

    if (errores == 0)
    {
        printf("Hilo No. %d reviso TODAS las columna resultado: Ok\n", hilo_ID);
    }

    pthread_exit(NULL);
}

//---------------->Función que valida si los números en una cuadrícula están dentro del rango del 1 al 9
void *MetodoValidar_CUADRICULA(void *args)
{
    ThreadArgs *t_args = (ThreadArgs *)args;
    int hilo_ID = t_args->hilo_ID;
    int fila_ID = t_args->fila_ID;
    int columna_ID = t_args->columna_ID;
    printf("Hilo No. %d revisa cuadrícula No. %d, resultado: ", hilo_ID, fila_ID * 3 + columna_ID + 1);
    int used[9] = {0}; //-------------->Array para marcar los números utilizados
    int i, j;
    for (i = fila_ID * 3; i < fila_ID * 3 + 3; i++)
    {
        for (j = columna_ID * 3; j < columna_ID * 3 + 3; j++)
        {
            int num = sudoku[i][j];
            if (num < 1 || num > 9 || used[num - 1])
            {
                printf("Mal\n"); //------------>En caso de que salga mal la revision
                pthread_exit(NULL);
            }
            else
            {
                used[num - 1] = 1;
            }
        }
    }
    printf("Ok\n"); //------------>En caso de que salga bien la revision
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{

    FILE *ArchivoE;
    FILE *ArchivoE2;

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

    //----------------------------------------------->Verifico si el sudoku es válido o no
    int valid = 1;
    for (i = 0; i < 9; i++)
    {
        int row_used[9] = {0};
        int col_used[9] = {0};
        int subgrid_used[9] = {0};
        for (j = 0; j < 9; j++)
        {
            //------------------------------->Verifico fila
            int num = sudoku[i][j];
            if (num < 1 || num > 9 || row_used[num - 1])
            {
                valid = 0;
            }
            else
            {
                row_used[num - 1] = 1;
            }
            //-------------------------------->Verifico columna
            num = sudoku[j][i];
            if (num < 1 || num > 9 || col_used[num - 1])
            {
                valid = 0;
            }
            else
            {
                col_used[num - 1] = 1;
            }
            //----------------------------->Verifico cuadrícula
            num = sudoku[(i / 3) * 3 + j / 3][(i % 3) * 3 + j % 3];
            if (num < 1 || num > 9 || subgrid_used[num - 1])
            {
                valid = 0;
            }
            else
            {
                subgrid_used[num - 1] = 1;
            }
        }
    }

    //------------------------------->Escribo el resultado en la consola
    if (valid)
    {
        printf("\n********Solución Sudoku válida");
    }
    else
    {
        printf("\n********Solución Sudoku NO válida");
    }

    //----------------------->Escribo el resultado en el mismo archivo
    ArchivoE2 = fopen(argv[1], "w");

    if (valid)
    {
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                fprintf(ArchivoE2, "%d ", sudoku[i][j]);
            }
            fprintf(ArchivoE2, "\n");
        }
        fprintf(ArchivoE2, "\n/********************************************/\n");
        fprintf(ArchivoE2, "Solución Sudoku válida\n");
        fprintf(ArchivoE2, "/********************************************/\n");
    }
    else
    {
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                fprintf(ArchivoE2, "%d ", sudoku[i][j]);
            }
            fprintf(ArchivoE2, "\n");
        }
        fprintf(ArchivoE2, "\n/********************************************/\n");
        fprintf(ArchivoE2, "Solución Sudoku NO válida\n");
        fprintf(ArchivoE2, "/********************************************/\n");
    }

    //---------------->Cierro el archivo de sudoku
    fclose(ArchivoE);
    fclose(ArchivoE2);

    return 0;
}