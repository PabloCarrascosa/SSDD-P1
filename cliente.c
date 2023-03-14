#include <mqueue.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include "claves.h"

int main(void) {
	init(); 

	char value1[256];
	int value2 = 2;
	double value3 = 2.34;
	sprintf(value1, "pepito grillo");

	char value1_get[256]; 
	int value2_get; 
	double value3_get; 
	


    int init_res = init();
    printf("%d\n", init_res); 

    set_value(0, value1, value2, value3);
    set_value(1, value1, value2, value3);
    set_value(100, value1, value2, value3);
    set_value(1540, "Hola holita", 23, 23.25);


    int get_res = get_value(100, value1_get, &value2_get, &value3_get);
	int set_res = set_value(2, value1, value2, value3);
	printf("Get: Codigo error: %d; Value1: %s; Value2: %d; Value3: %lf;\n", get_res, value1_get, value2_get, value3_get);
	printf("Set: %d\n", set_res);


	// Prueba no exist
	set_value(3, value1, value2, value3);
	int exist_res = exist(3); 
	printf("Exist: %d\n", exist_res);


	//Prueba delete key
	int delete = delete_key(3);
	printf("Delete: %d\n", delete);


	// Prueba exist
	exist_res = exist(3); 
	printf("Exist: %d\n", exist_res);

}

