#ifndef _CLAVES_H
#define _CLAVES_H


#define MAX_SIZE 256

enum OP {
	INIT = 0,
	SET_VALUE,
	GET_VALUE,
	MODIFY_VALUE,
	DELETE_KEY,
	EXIST
}; 


typedef struct Tuple{
	int key;               // Clave de la tupla
	char value1[MAX_SIZE]; // Valor 1 
	int value2; 		   // Valor 2
	double value3;         // Valor 3  
} Tuple; 

typedef struct Request{
	char name[MAX_SIZE];   // Nombre de la cola 
	int op;                // Codigo de operacion 
	Tuple data;            // Tupla con los datos 
} Request; 

typedef struct Response{
	int status; 
	Tuple data; 

} Response; 


int init(void); 
int set_value(int key, char *value1, int value2, double value3); 
int get_value(int key, char *value1, int *value2, double *value3); 
int modify_value(int key, char *value1, int value2, double value3); 
int delete_key(int key); 
int exist(int key); 
int copy_key(int key1, int key2); 

#endif

