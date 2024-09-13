#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/stat.h>

//-------------------------------------------> METODO HISTORICO

void MetodoH(char ArchivoEntrada[], char ArchivoSalida[], char Tamano[], int byteInicia, int bytesLeer)
{
      int a = 0;
      int e = 0;
      int letraI = 0;
      int o = 0;
      int u = 0;
      int contadorLinea = 0;
      char c;
      int Arch_Open = open(ArchivoEntrada, O_RDONLY); // Abro el archivo de entrada solo para LECTURA
      int Arch_Close = open(ArchivoSalida, O_WRONLY | O_CREAT | O_APPEND,0600); // Abro el archivo de salida y con APPEDN le concateno lo anterior
      lseek(Arch_Open, byteInicia, SEEK_CUR); // Con ISEEK Accedo a los bytes del texto
      read(Arch_Open, Tamano, bytesLeer); // Con READ empiezo a leer el archivo con el byte que le indique


      //-------------------------------------->Con FOR cuento la cantidad de vocales que hay en el archivo
      for (int i=0;i< strlen(Tamano); ++i){
          c = toupper(Tamano[i]);
          if (c=='A')
          a++;
          if (c=='E')
          e++;
          if (c=='I')
          letraI++;
          if (c=='O')
          o++;
          if (c=='U')
          u++;
          if (c =='\n')
          contadorLinea++;
          }
   
      printf("%s \n", Tamano);
      char cadena[] = "\nTotal lineas : ";
      char array[12];
      sprintf(array,"%d",contadorLinea+1); 
      strcat(cadena,array);
      
      write(Arch_Close, cadena, strlen(cadena));//Escribo la cantidad de lineas en el TXT
      

      char cadenaletraA[] = "\n A = : ";
      char arrayletraA[12];
      sprintf(arrayletraA,"%d",a); 
      strcat(cadenaletraA,arrayletraA);
      write(Arch_Close, cadenaletraA, strlen(cadenaletraA));//Escribo la letra A en el TXT


      char cadenaletraE[] = "\n E = : ";
      char arrayletraE[12];
      sprintf(arrayletraE,"%d",e); 
      strcat(cadenaletraE,arrayletraE);
      write(Arch_Close, cadenaletraE, strlen(cadenaletraE));//Escribo la letra E en el TXT


      char cadenaletraI[] = "\n I = : ";
      char arrayletraI[12];
      sprintf(arrayletraI,"%d",letraI); 
      strcat(cadenaletraI,arrayletraI);
      write(Arch_Close, cadenaletraI, strlen(cadenaletraI));//Escribo la letra I en el TXT


      char cadenaletraO[] = "\n O = : ";
      char arrayletraO[12];
      sprintf(arrayletraO,"%d",o); 
      strcat(cadenaletraO,arrayletraO);
      write(Arch_Close, cadenaletraO, strlen(cadenaletraO));//Escribo la letra O en el TXT

      char cadenaletraU[] = "\n U = : ";
      char arrayletraU[12];
      sprintf(arrayletraU,"%d",u); 
      strcat(cadenaletraU,arrayletraU);
      write(Arch_Close, cadenaletraU, strlen(cadenaletraU));//Escribo la letra U en el TXT

      

      printf("A = %d\n",a);
      printf("E = %d\n",e);
      printf("I = %d\n",letraI);
      printf("O = %d\n",o);
      printf("U = %d\n",u);
      printf("Total lineas: %d\n",contadorLinea + 1);
      close(Arch_Open);
      close(Arch_Close); //Cierre de los archivos que se utilizaronn
}



//-------------------------------------------> METODO NO HISTORICO


void MetodoNh(char ArchivoEntrada[], char ArchivoSalida[], char Tamano[], int byteInicia, int bytesLeer)
{   
    //Variables para las vocales
      int a = 0,e = 0;
      int letraI = 0;
      int o = 0;
      int u = 0;
      int contadorLinea = 0;
      char c;
      int Arch_Open = open(ArchivoEntrada, O_RDONLY);  // Abro el archivo de entrada solo para LECTURA
      int Arch_Close = open(ArchivoSalida, O_WRONLY | O_CREAT | O_TRUNC, 0600); // Abro el archivo de salida para CREARLO Y ESCRIBIRLO
      lseek(Arch_Open, byteInicia, SEEK_CUR);  // Con ISEEK Accedo a los bytes del texto
      read(Arch_Open, Tamano, bytesLeer); //Con READ empiezo a leer el archivo con el byte que le indique

      //-------------------------------------->Con FOR cuento la cantidad de vocales que hay en el archivo
      for (int i=0;i< strlen(Tamano); ++i){
          c = toupper(Tamano[i]);
          if (c=='A')
          a++;
          if (c=='E')
          e++;
          if (c=='I')
          letraI++;
          if (c=='O')
          o++;
          if (c=='U')
          u++;
          if (c =='\n')
          contadorLinea++;
          }
      
      printf("%s \n", Tamano);
      char cadena[] = "\nTotal lineas : ";
      char array[12];
      sprintf(array,"%d",contadorLinea+1); 
      strcat(cadena,array);
      write(Arch_Close, cadena, strlen(cadena));//Esccribo la cantidad de lineas en el TXT


       char cadenaletraA[] = "\n A = : ";
      char arrayletraA[12];
      sprintf(arrayletraA,"%d",a); 
      strcat(cadenaletraA,arrayletraA);
      write(Arch_Close, cadenaletraA, strlen(cadenaletraA));//Escribo la letra A en el TXT


      char cadenaletraE[] = "\n E = : ";
      char arrayletraE[12];
      sprintf(arrayletraE,"%d",e); 
      strcat(cadenaletraE,arrayletraE);
      write(Arch_Close, cadenaletraE, strlen(cadenaletraE));//Escribo la letra E en el TXT


      char cadenaletraI[] = "\n I = : ";
      char arrayletraI[12];
      sprintf(arrayletraI,"%d",letraI); 
      strcat(cadenaletraI,arrayletraI);
      write(Arch_Close, cadenaletraI, strlen(cadenaletraI));//Escribo la letra I en el TXT


      char cadenaletraO[] = "\n O = : ";
      char arrayletraO[12];
      sprintf(arrayletraO,"%d",o); 
      strcat(cadenaletraO,arrayletraO);
      write(Arch_Close, cadenaletraO, strlen(cadenaletraO));//Escribo la letra O en el TXT

      char cadenaletraU[] = "\n U = : ";
      char arrayletraU[12];
      sprintf(arrayletraU,"%d",u); 
      strcat(cadenaletraU,arrayletraU);
      write(Arch_Close, cadenaletraU, strlen(cadenaletraU));//Escribo la letra U en el TXT

      printf("A = %d\n",a);
      printf("E = %d\n",e);
      printf("I = %d\n",letraI);
      printf("O = %d\n",o);
      printf("U = %d\n",u);
      printf("Total lineas: %d\n",contadorLinea + 1);
      close(Arch_Open);
      close(Arch_Close);  //Cerramos los archivos utilizados en el metodo
}


int main(int argc, char *argv[])
{
      char Tamano[1000];
      int byteInicia; // Le indico el byte donde inicia
      int bytesLeer; // Almacena los bytes por leer
      char c;
      char ArchivoEntrada[100];
      char ArchivoSalida[100];
      int Arch_Open;
      int Arch_Close;
      char *hnh; // Guardo el tipo historico o no
      char Total[] = "T";
      char *Total_Par = argv[2];
      off_t size;
       //Valido si es lectura total o no
      if(strcmp(Total_Par, Total) == 0) 
      {
     
          strcpy(ArchivoEntrada, argv[1]);     //---> Acomodo los parametros en caso de ser lectura total
          struct stat st;
          stat(ArchivoEntrada, &st);
          size = st.st_size; //--> Obtengo el tamaño de bytes del archivo
          byteInicia = 0; 
          bytesLeer = size;
          strcpy(ArchivoSalida, argv[3]);
          hnh = argv[4];
      }else
      {
           strcpy(ArchivoEntrada, argv[1]);
           struct stat st;
           stat(ArchivoEntrada, &st);
           size = st.st_size;
           byteInicia = atoi(argv[2]);
           bytesLeer = atoi(argv[3]);
           strcpy(ArchivoSalida, argv[4]);
           hnh = argv[5];
      }
      char Historico[] = "h";
      char NoHistorico[] = "Nh";
      Arch_Open = open(ArchivoEntrada, O_RDONLY);

      if (Arch_Open == -1)//-----------------> Validacion de ruta
      {
            printf("Error!, Archivo no encontrado \n.");
            exit(1);
      }
      close(Arch_Open);
      if (byteInicia < 0 || bytesLeer < 0)//-----------------> Validacion de numeros positivos
      {
            printf("Error! Solo se permiten números mayores a 0.");
            printf("\n");
            exit(1);
      }
      if(byteInicia > size)//-----------------> Validacion de bytes superados
      {
            printf("Error! El numero de bytes ingresados supera el máximo de los bytes del archivo.");
            printf("\n");
            exit(1);  
      }
      // Datos que se recaudaron con el llamado de la funcion
      if (strcmp(hnh, Historico) == 0)
      {
            MetodoH(ArchivoEntrada, ArchivoSalida, Tamano, byteInicia, bytesLeer);
      }
      else if (strcmp(hnh, NoHistorico) == 0)
      {
            MetodoNh(ArchivoEntrada, ArchivoSalida, Tamano, byteInicia, bytesLeer);
      }
      return 0;
}
