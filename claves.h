#ifndef _CLAVES_H
#define _CLAVES_H

#include "tipos.h"

int init(void); 
int set_value(int key, char *value1, int value2, double value3); 
int get_value(int key, char *value1, int *value2, double *value3); 
int modify_value(int key, char *value1, int value2, double value3); 
int delete_key(int key); 
int exist(int key); 
int copy_key(int key1, int key2); 

#endif

