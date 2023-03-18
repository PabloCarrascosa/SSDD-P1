#include <mqueue.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include "claves.h"


int main(void) {


	char value1[256];
	int value2 = 2;
	double value3 = 2.34;
	sprintf(value1, "pepito grillo");

	char value1_get[256]; 
	int value2_get; 
	double value3_get; 
	

	/***************INIT*************/
	printf("/***************INIT*************/\n");
    int init_res = init();
    printf("Resultado init (resultado esperado 0): %d;\n",  init_res);



    /***************SET*************/
    printf("\n/***************SET*************/\n");
    int set_res; 
    set_res = set_value(0, value1, value2, value3);
  	printf("Resultado set 0 (resultado esperado 0): %d;\n",  set_res);
    set_res = set_value(1, value1, value2, value3);
	printf("Resultado set 1 (resultado esperado 0): %d;\n", set_res);
    set_res = set_value(100, value1, value2, value3);
    printf("Resultado set 100 (resultado esperado 0): %d;\n", set_res);
    set_res = set_value(1540, "Hola holita", 23, 23.25);
    printf("Resultado set 1540 (resultado esperado 0): %d;\n", set_res);
	set_res = set_value(0, "Esta clave esta repetida", 100, 200.0932);
	printf("Resultado set 0 (repetida, resultado esperado -1) : %d;\n", set_res);



    /***************GET*************/
    printf("\n/***************GET*************/\n");
    int get_res; 
    get_res = get_value(1, value1_get, &value2_get, &value3_get);
	printf("GET 1 (RESULTADO ESPERADO 0) \n  Codigo error: %d \n  Value1: %s \n  Value2: %d \n  Value3: %lf \n", get_res, value1_get, value2_get,value3_get);
	strcpy(value1_get, "");
	value2_get = 0; 
	value3_get = 0.0; 
    get_res = get_value(100, value1_get, &value2_get, &value3_get);
	printf("GET 100 (RESULTADO ESPERADO 0) \n  Codigo error: %d \n  Value1: %s \n  Value2: %d \n  Value3: %lf \n", get_res, value1_get, value2_get,value3_get);
	strcpy(value1_get, "");
	value2_get = 0; 
	value3_get = 0.0; 
	get_res = get_value(1234, value1_get, &value2_get, &value3_get);
	printf("GET 1234 (RESULTADO ESPERADO -1) \n  Codigo error: %d \n  Value1: %s \n  Value2: %d \n  Value3: %lf \n", get_res, value1_get, value2_get,value3_get);



    /***************MODIFY*************/
    printf("\n/***************MODIFY*************/\n");
	int mod_res; 
	mod_res = modify_value(100, "Ahora modificado", 1111, 1.111); 
	printf("Resultado modify 100 (resultado esperado 0): %d \n", mod_res); 
	get_res = get_value(100, value1_get, &value2_get, &value3_get);
	printf("GET (RESULTADO ESPERADO 0 Y QUE EL CONTENIDO HAYA CAMBIADO) \n  Codigo error: %d \n  Value1: %s \n  Value2: %d \n  Value3: %lf \n", get_res, value1_get, value2_get,value3_get);
	mod_res = modify_value(3412344, "Ahora modificado 2, da error puesto que la clave no existe", 2314, 1332.2232); 
	printf("Resultado modify 3412344 (resultado esperado -1, dicha clave no existe en la db): %d \n", mod_res);



	/***************DELETE*************/
	printf("\n/***************DELETE*************/\n");
	int del_res; 
	del_res = delete_key(1540); 
	printf("Resultado delete 1540 (resultado esperado 0):  %d \n", del_res); 
	del_res = delete_key(210212); 
	printf("Resultado delte 210212 (resultado esperado -1): %d \n", del_res); 



	/***************EXIST*************/
	printf("\n/***************EXIST*************/\n"); 
	int ex_res; 
	ex_res = exist(100); 
	printf("Resultado exist 100 (resultado esperado 1): %d \n", ex_res); 
	ex_res = exist(1540); 
	printf("Resultado exist 1540 (resultado esperado 0, eliminado antes): %d \n", ex_res); 



	/***************COPY*************/
	printf("\n/***************COPY*************/\n"); 
	int copy_res; 
	copy_res = copy_key(1, 320); 
	printf("Resultado copy 1 a 320 (resultado esperado 0): %d \n", copy_res);
	strcpy(value1_get, "");
	value2_get = 0; 
	value3_get = 0.0; 
	get_res = get_value(320, value1_get, &value2_get, &value3_get); 
	printf("GET 320 (resultado esperado 0 y contenido igual al 1) \n  Codigo error: %d \n  Value1: %s \n  Value2: %d \n  Value3: %lf \n", get_res, value1_get, value2_get,value3_get);
	strcpy(value1_get, "");
	value2_get = 0; 
	value3_get = 0.0; 
	get_res = get_value(1, value1_get, &value2_get, &value3_get); 
	printf("GET 1 (resultado esperado 0 y contenido igual al 320) \n  Codigo error: %d \n  Value1: %s \n  Value2: %d \n  Value3: %lf \n", get_res, value1_get, value2_get,value3_get);
	copy_res = copy_key(33209, 2020); 
	printf("Resultado copy 33209 a 2020 (resultado esperado -1): %d \n", copy_res);


}

