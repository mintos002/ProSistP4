#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include "buffer_circ.h"


//pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // Iniciar mutex -----------------------

//sem_t buffer_lock, items, huecos; // Definir semaforos


struct Buffer_Circ {  // Definir estructura Buffer_Circ
 int buffer[BUFSIZE];
 int bufIN, bufOUT;
 int contador;
};

// Iniciar bufer
void initbuffer( struct Buffer_Circ *buff) {
 int i;
 
 //sem_init (&buffer_lock, 0, 1); //----------------------------------
 //sem_init(&items, 0, 0);
 //sem_init(&huecos, 0, BUFSIZE-1);
                                                                                             
 for(i=0; i<BUFSIZE; i++){
  (*buff).buffer[i] = -1;
  }
  (*buff).bufIN = 0;
  (*buff).bufOUT = 0;
  (*buff).contador = 0;
}

// Get item
int get_item(int* x, struct Buffer_Circ *buff) {
 int nxtOUT = (*buff).bufOUT % BUFSIZE;

 /*CON SEMAFOROS*/ //---------------------------------
 //sem_wait(&items);
 //sem_wait(&buffer_lock);

 if( (*buff).contador > 0){           // Si el buffer no esta vacio
                                  
   /*CON MUTEX:*/   //-------------------------------
   pthread_mutex_lock(&buffer_lock); // Bloquear mutex
     
   *x = (*buff).buffer[nxtOUT];       // Asignar resultado a x
   (*buff).bufOUT = (nxtOUT + 1) % BUFSIZE; // Actualizar bufOUT
   (*buff).contador = (*buff).contador - 1; // Actualizar contador
   
   /*CON MUTEX:*/      //------------------------------------
   pthread_mutex_unlock(&buffer_lock); // Desbloquear mutex

   /*CON SEMAFOROS*/ //----------------------------------
   //sem_post(&buffer_lock);
   //sem_post(&huecos);
                                       
   return 0;                         // Devolver 0 -> OK
  }
 else {                              // Si buffer esta lleno
  /*CON MUTEX:*/                         
  pthread_mutex_unlock(&buffer_lock); // Desbloquear mutex

  return -1;                         // Devolver -1 -> NOT OK
 }
}

// Put item
int put_item(int x, struct Buffer_Circ *buff) {
 int nxtIN = (*buff).bufIN % BUFSIZE;

 /*CON SEMAFOROS*/ //----------------------------
 //sem_wait(&huecos);
 //sem_wait(&buffer_lock);

 if( (*buff).contador < BUFSIZE ){   // Si el buffer esta vacio
  
  /*CON MUTEX:*/ //------------------------------
  pthread_mutex_lock(&buffer_lock); // Desbloquear mutex

  (*buff).buffer[nxtIN] = x;        // Insertar x
  (*buff).bufIN = (nxtIN + 1) % BUFSIZE;// Actualizar bufIN
  (*buff).contador = (*buff).contador + 1; // Actualizar contador

  /*CON MUTEX:*/  //-----------------------------
  pthread_mutex_unlock(&buffer_lock); // Desbloquear mutex

  /*CON SEMAFOROS*/ //------------------------
  //sem_post(&buffer_lock);
  //sem_post(&items);

  return 0;                         // Devolver 0 -> OK
 }
 else {                              // Si buffer esta lleno

  /*CON MUTEX:*/  //----------------------------                        
  pthread_mutex_unlock(&buffer_lock); // Desbloquear mutex

  return -1;                         // Devolver -1 -> NOT OK
 }

}

// Consultar si una variable Buffer_Circ está vacía
char *bc_vacio(struct Buffer_Circ *buff){
 if( (*buff).contador == 0 ) {
  return "True";
 } else {
  return "False";
 }
}


// Consultar si una variable Buffer_Circ está lleno
char* bc_lleno(struct Buffer_Circ *buff){
 if( (*buff).contador == BUFSIZE ) {
  return "True";
 } else {
  return "False";
 }
}

//PRINT
void print (struct Buffer_Circ *buff){
// printf("OK? = %d\n", ok );
 printf("bufIN = %d\n", (*buff).bufIN );
 printf("bufOUT = %d\n", (*buff).bufOUT );
 printf("contador = %d\n", (*buff).contador );
 int i;
 for(i=0; i<BUFSIZE; i++){
  printf("Posicion %d valor: %d\n", i, (*buff).buffer[i] );
 }
 printf("------------------------------------------------------------\n");
}

// Devolver número de elementos
int num_elementos (struct Buffer_Circ *buff){
 return (*buff).contador;
}
