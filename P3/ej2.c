#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int SumPar=0;                                                                                   //variable de tipo entero que guardará la suma de los números pares
int SumImpar=0;                                                                                 //variable de tipo entero que guardará la suma de los números impares
pthread_mutex_t x = PTHREAD_MUTEX_INITIALIZER;                                                  //Declaramos la variable x que nos servirá de semáforo para las hebras

void * HiloCalculador(void * i){
    int *var= (int*) i;
    int *sum=malloc(sizeof(int *));                                                             //Declaramos la variable sum a la cual le reservamos memoria y que guardará la suma de los números generados aleatoriamente
    for(int i=0; i<5; i++){
        int numAl=(rand()%11);                                                                  //Declaramos la variable numAl que nos servirá para guardar los números aleatorios que generemos en el bucle
        *sum+=numAl;                                                                            //Guardamos en la variable sum el valor de la suma de los números generados aleatoriamente
    }


    if (pthread_mutex_lock(&x)){                                                                //Bloqueamos la hebra para que las demas se esperen hasta que termine la primera y asi sucesivamente
        printf("Error en bloqueo de mutex");                                                    //Imprimimos un mensaje por pantalla
        exit(EXIT_FAILURE);                                                                     //Salimos del programa si ocurre algún error
    }



    if(*var%2==0){                                                                              //Si var es par entramos dentro de esta parte
        printf("Variable %d de tipo par :%d\n",*var,*sum);                                      //Imprimimos el valor de la variable *sum
        SumPar+=*sum;                                                                           //Vamos realizando la suma en la variable global SumPar
        printf("Hebra de orden de creación %d devolviendo la suma %d\n\n",*var,SumPar);         //Imprimimos por pantalla el valor actual de la variable SumPar
    }
    else{                                                                                       //Si var es impar, entraremos dentro de esta parte
        printf("Variable %d de tipo impar :%d\n",*var,*sum);                                    //Imprimimos el valor de la variable *sum
        SumImpar+=*sum;                                                                         //Vamos realizando la suma en la variable global SumImpar
        printf("Hebra de orden de creación %d devolviendo la suma %d\n",*var,SumImpar);         //Imprimimos por pantalla el valor actual de la variable SumImpar
    }



    if (pthread_mutex_unlock(&x)){                                                              //Desbloqueamos la hebra
        printf("Error en bloqueo de mutex");                                                    //Imprimimos un mensaje por pantalla
        exit(EXIT_FAILURE);                                                                     //Salimos del programa si ocurre algún error
    }


    pthread_exit((void*)sum);                                                                   //Terminamos la hebra devolviendo el valor sum
}



int main(int argc, char* const argv[]){
    int n=atoi(argv[1]);                                                                        //La variable n, va a guardar el número de hilos que se van a crear
    int error=0;                                                                                //Variable para la comprobación de errores durante la creación de hebras
    int *recogida;                                                                              //variable que servirá para la recogida de datos de las hebras
    int iden[n];                                                                                //Variable que usamos para identificar las hebras en orden
    pthread_t thread[n];                                                                        //Variable de tipo thread_t para la creación de hebras

    srand(time(NULL));                                                                          //Función que sirve para la creación de números aleatorios

    for(int i=1; i<=n; i++){                                                                    //Bucle que nos servirá para la creación de hilos y asignación de números a los mismos
        iden[i]=i ;                                                                             //Asignamos al vector iden[i] el valor de i pues ahora mismo solo posee en su interior basura
        error=pthread_create(&thread[i], NULL, HiloCalculador, (void*) &iden[i]);               //Guardamos en la variable error la función de creación de hebras a la cual le pasamos la hebra, a la función HiloCalculador y el parámetro i          
        if(error!=0){                                                                           //Si la variable error es distinta de 0, entramos dentro
            printf("Error en la creación de hebras pares\n");                                   //Imprimimos un mensaje de error por pantalla
            printf("Cerrando el programa");                                                     //Imprimimos un mensaje de error por pantalla
            exit(EXIT_FAILURE);                                                                 //Salimos del programa de forma fallida
        }
    }

    for(int i=1; i<=n; i++){
        pthread_join(thread[i],(void**)&recogida);                                              //Llamamos a la función de recogida de valores creados en las hebras y guardamos el valor en recogida
    }
    printf("\nValores de la suma de variables pares:%d\n",SumPar);
    printf("valores de la suma de variables impares:%d\n",SumImpar);

    exit(EXIT_SUCCESS);                                                                         //Terminamos el programa de forma exitosa

}