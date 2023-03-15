#ifndef _TIPOS_H
#define _TIPOS_H


#define MAX_SIZE 256

enum OP {
	INIT = 0,
	SET_VALUE,
	GET_VALUE,
	MODIFY_VALUE,
	DELETE_KEY,
	EXIST,
	COPY_KEY
}; 

typedef struct tuple_t{    
	int key;               // Clave de la tupla
	char value1[MAX_SIZE]; // Valor 1 
	int value2; 		   // Valor 2
	double value3;         // Valor 3  
} tuple_t; 

typedef struct request_t{
	char name[MAX_SIZE];   // Nombre de la cola 
	int op;                // Codigo de operacion 
	tuple_t data;          // Tupla con los datos 
	int key2;			   // Segunda key opcional, usada solo para el copy 
} request_t; 

typedef struct response_t{
	int status; 
	tuple_t data; 
} response_t; 

#endif

