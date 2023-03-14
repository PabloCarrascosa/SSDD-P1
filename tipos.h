#ifndef _TIPOS_H
#define _TIPOS_H


#define MAX_SIZE 256

enum OP {
	INIT = 0,
	SET_VALUE = 1,
	GET_VALUE = 2,
	MODIFY_VALUE = 3,
	DELETE_KEY = 4,
	EXIST = 5,
	COPY_KEY = 6
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
	tuple_t data;            // Tupla con los datos 
	int key2;
} request_t; 

typedef struct response_t{
	int status; 
	tuple_t data; 
} response_t; 



int init(void); 
int set_value(int key, char *value1, int value2, double value3); 
int get_value(int key, char *value1, int *value2, double *value3); 
int modify_value(int key, char *value1, int value2, double value3); 
int delete_key(int key); 
int exist(int key); 
int copy_key(int key1, int key2); 

#endif

