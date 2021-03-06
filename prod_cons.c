/**
* Para compilar teclea: gcc x.c -lpthread -o x
*/
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "buffer_circ.c"

#define INTPROD 20
#define INTCONS 25
#define NTHREADS 50

sem_t mutex, items, huecos;           // Definir semaforos

int ww;
void *Productor( void *arg )          // Funcion productor
{
  struct Buffer_Circ *buffer;         // Crear buffer
  buffer  = (struct Buffer_Circ*)arg; // Preparar el buffer
  int w, ins, err;

  for(w=0; w < INTPROD; w++){         // Bucle con interacciones de productor
    ins = ww+100;
    ww++;

    sem_wait(&huecos);                // Esperar a huecos
    sem_wait(&mutex);                 // Esperar mutex

    err = put_item(ins,buffer);       // Llamar a put_item

    if(err == -1){                    // Si put item devuelve error
      printf("OP: %d. Error al insertar %d\n", w, ins); // Avisar del error
      printf("CONTADOR: %d\n", num_elementos(buffer));  // Mostrar el contador
      printf("------------------------------------------------------------\n");
      sem_post(&mutex);               // Incrementa mutex
      sem_post(&items);               // Incrementa mutex
    }

    else {                            // Si no hay error
      printf("OP: %d. Se ha INSERTADO el numero: %d\n", w, ins);
      //   print(buffer);
      printf("CONTADOR: %d\n", num_elementos(buffer));
//      printf("Buffer lleno %s", bc_lleno(buffer) ? "true" : "false");
      printf(" --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- \n");
      sem_post(&mutex); 
      sem_post(&items); 
    }
    usleep(1000000);                  // Retraso 1seg
  }
}

void *Consumidor( void *arg )         // Funcion consumidor
{
  struct Buffer_Circ *buffer;
  buffer  = (struct Buffer_Circ*)arg;

  int w, err;
  int val;
  for(w=0; w < INTCONS; w++){

  sem_wait(&items);
  sem_wait(&mutex);

  err = get_item(&val,buffer);

  if(err == -1){
    printf("OP: %d. Error al obtener %d\n", w, val);
    printf("CONTADOR: %d\n", num_elementos(buffer));
    printf("------------------------------------------------------------\n");
    sem_post(&mutex);
    sem_post(&huecos);
  }
  else {
    printf("OP: %d. Se ha EXTRAIDO el numero: %d\n", w, val);
    //print(buffer);
    printf("CONTADOR: %d\n", num_elementos(buffer));
//    printf("Buffer vacio? %s\n", bc_vacio(buffer) ? "true" : "false");
    printf(" --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- \n");
    sem_post(&mutex);
    sem_post(&huecos);
  }
  usleep(2000000);                    // Retraso 2seg
 }
}

int main()
{
  int x, i, j;
  struct Buffer_Circ bc;              // Crear puntero buff y buff
  struct Buffer_Circ *pbc;

  // Iniciar el semaforo
  sem_init(&mutex, 0, 1);             //----------------------------------
  sem_init(&items, 0, 0);
  sem_init(&huecos, 0, BUFSIZE);

  pbc = &bc;                          // Apuntar a buff
  initbuffer(pbc);                    // Iniciar buffer
 
  pthread_attr_t atrib;               //Crear atributo
 
  pthread_t t_productor[NTHREADS], t_consumidor[NTHREADS];        // Crear array de threads

  pthread_attr_init( &atrib );

  for(i=0;i<NTHREADS;i++){
    pthread_create( &t_productor[i], &atrib, Productor, pbc);
    pthread_create( &t_consumidor[i], &atrib, Consumidor, pbc);
  }

  pthread_join( t_productor[NTHREADS-1], NULL);                   // Acabar con los threads
  pthread_join( t_consumidor[NTHREADS-1], NULL);

  printf("FIN DE LA APLICACIÓN\n");
  return 0;
}
