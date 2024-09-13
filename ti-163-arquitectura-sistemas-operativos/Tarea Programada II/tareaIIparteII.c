#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>       
#include <fcntl.h> 


#define SMOBJ_NAME "/MemoriaCompartida" 
#define SMOBJ_SIZE 200

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
    

pid_t childpid;
int status=0;
int result;
    if ((childpid = fork()) == -1)
    {
        perror("Error en llamada a fork\n");
        exit(1);
    }
    else if (childpid == 0)
    {
        double resultado = Calcularcoseno(coseno, precision);//----------->LLamo al metodo que hace el calculo y le paso los 2 parametros que solicita
            //--------------------->Escribo en la memoria compartida
            int fd;
            char *ptrr;
            fd = shm_open(SMOBJ_NAME, O_RDWR, 0);
            

            if(fd == -1){
                printf("Eror: EL objeto de memoria compartida no pudo crearse \n");
                exit(1);
            }
        
          ptrr = mmap(0, sizeof(resultado), PROT_WRITE, MAP_SHARED, fd, 0);

                if(ptrr == MAP_FAILED){
                    printf("Error en el mapeo de memoria \n");
                    exit(1);
                }
                   
            memcpy(ptrr, resultado, strlen(resultado));
            close(fd);
            return 0;

   //--------------------->Fin de escribrir en la memoria compartida

        fprintf(stdout, "Soy el proceso hijo (%ld) y voy a devolver a mi padre (%ld)\n",(long)getpid(),(long)getppid());    
        printf("El coseno de %f con una precisión de %d elementos es: %.5f.\n", coseno, precision, resultado);//---------->Print en consola del resultado


         
        sleep(4);
        exit (result);
    }
    else{
        while( childpid != wait(&status));

            //--------------->Crear Memoria Compartida
            
            int fd;
            fd = shm_open(SMOBJ_NAME, O_CREAT | O_RDWR, 00600);
            if(fd == -1){
                printf("Eror: EL objeto de memoria compartida no pudo crearse \n");
                exit(1);
            }
            if(-1 == ftruncate(fd, SMOBJ_SIZE)){
                printf("Eror: EL objeto de memoria compartida no pudo asignarse \n");
                exit(1);
            }
            close(fd);
            return 0;
            //--------------->Fin de crear Memoria Compartida

        fprintf(stdout, "Soy el proceso padre (%ld) y mi hijo (%ld) me ha devuelto %d\n",(long)getpid(),(long)childpid,status);
            
        archivo = fopen(archivo_salida, "a");//-------------->Abrimos el archivo o lo creamos para escibir los resultados
        if (archivo == NULL) {
            printf("**Error: no se pudo abrir el archivo %s.\n", archivo_salida);
            return 1;
        }
        fprintf(archivo, " \n El coseno de %f con una precisión de %d elementos es: %.5f.\n", coseno, precision, result);
        fclose(archivo);
        return 0;
        }
        return (0);
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
