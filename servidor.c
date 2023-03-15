#include <mqueue.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include "tipos.h"


#define MAX_SIZE 256

pthread_mutex_t mutex_request;
int request_not_copied = true;
pthread_cond_t request_cond;
mqd_t server_q;
struct mq_attr attr;


int exist(int key){ //Funciona
	char full_path[2048];
	sprintf(full_path, "key_db/%d.txt", key);
	FILE *keydata;
	keydata = fopen(full_path, "r");
	if (keydata == NULL){
		return 0;
	}
	else{
		fclose(keydata);
		return 1;
	}
}


int init(){ //Funciona
	DIR* dir = opendir("key_db");
	if (dir){
		struct dirent *entry;
		while((entry = readdir(dir))!= 0){     
			if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0){
				char path[1024];
				sprintf(path,"key_db/%s", entry->d_name);
				unlink(path);
			}
		}
	}
	else{
		if(mkdir("key_db", 0755)<0){ // o 0777
				closedir(dir);
				return -1;
			}
		}
		closedir(dir);
		return 0;
}


int set_value(int key, char *value1, int value2, double value3){ //Funciona
	char full_path[2048];
	sprintf(full_path, "key_db/%d.txt", key);
	FILE *keydata;
	if (exist(key)){
		return -1;
	}
	else{
		keydata = fopen(full_path, "w");
		fprintf(keydata, "%d;%s;%d;%f;",key, value1, value2, value3);
		fclose(keydata);
		return 0;
	}
}	


int get_value(int key, char *value1, int *value2, double *value3) 
{
	char full_path[2048];
	sprintf(full_path, "key_db/%d.txt", key);
	FILE *keydata;
	if (!exist(key)){
		return -1;
	}
	else{
		keydata = fopen(full_path, "r");
		fscanf(keydata, "%*d;%[^;];%d;%lf;", value1, value2, value3); 
		fclose(keydata);
		return 0;
	}
}


int modify_value(int key, char *value1, int value2, double value3){
	char full_path[2048];
	sprintf(full_path, "key_db/%d.txt", key);
	FILE *keydata;
	if (!exist(key)){
		return -1;
	}
	else{
		keydata = fopen(full_path, "w");
		fprintf(keydata, "%d;%s;%d;%f",key, value1, value2, value3);
		fclose(keydata);
		return 0;
	}
}


int delete_key(int key){ // Funciona
	char full_path[2048];
	sprintf(full_path, "key_db/%d.txt", key);
	if (!exist(key)){
		return -1;
	}
	else{
		unlink(full_path);
		return 0;
	}
}


int copy_key(int key1, int key2) {
    char path1[1024], path2[1024];
    char full_path1[2048], full_path2[2048];
    char copy_buff[1024];
    FILE* keydata1;
    FILE* keydata2;


    sprintf(path1, "%d.txt", key1);
    sprintf(path2, "%d.txt", key2);
    sprintf(full_path1, "key_db/%s", path1);
    sprintf(full_path2, "key_db/%s", path2);

    if (!exist(key1)) {
        return -1;
    }
	else{	
		keydata1 = fopen(full_path1, "r");
		keydata2 = fopen(full_path2, "w");
		while (fread(copy_buff, sizeof(char), sizeof(copy_buff), keydata1) > 0) {
			fwrite(copy_buff, sizeof(char), sizeof(copy_buff), keydata2);
		}
		fclose(keydata1);
		fclose(keydata2);
		return 0;
	}

}


/* Función llamada para tratar las peticiones recibidas */
void tratar_peticion(void *req){
	request_t request;
	response_t response;
	mqd_t client_q;
	
	pthread_mutex_lock(&mutex_request);
	request = (*(request_t *)req);
	request_not_copied = false;
	pthread_cond_signal(&request_cond);
	pthread_mutex_unlock(&mutex_request);

	switch(request.op){
		case INIT:
		/* FUNCIONALIDAD INIT */
			init();
			break;

		case SET_VALUE:
		/* FUNCIONALIDAD SET_VALUE */
			response.status = set_value(request.data.key, request.data.value1, request.data.value2, request.data.value3);
			break;
			
		/* FUNCIONALIDAD GET_VALUE */
		case GET_VALUE:
			response.status = get_value(request.data.key, response.data.value1, &response.data.value2, &response.data.value3); 
			break;
		/* FUNCIONALIDAD MODIFY_VALUE */
		case MODIFY_VALUE:
			response.status = modify_value(request.data.key, request.data.value1, request.data.value2, request.data.value3);
			break;

		/* FUNCIONALIDAD DELETE_KEY */
		case DELETE_KEY:
			response.status = delete_key(request.data.key);
			break;

		/* FUNCIONALIDAD EXIST */
		case EXIST:
			response.status = exist(request.data.key);
			break;

		case COPY_KEY:
			response.status = copy_key(request.data.key, request.key2);
			break;
			
	}
		
	client_q = mq_open(request.name, O_WRONLY);
	if (client_q < 0){
		printf("Error al abrir la cola del cliente\n");
		mq_close(server_q);
		mq_unlink("/SERVER");
	}
	else{
		if(mq_send(client_q, (const char *) &response, sizeof(response), 0) < 0){
			printf("Error al enviar la respuesta\n");
			mq_close(server_q);
			mq_unlink("/SERVER");
			mq_close(client_q);
		}
	}
	pthread_exit(0);

}


/* Abre la cola del servidor, recibe las peticiones, y espera a que se traten para mandar la respuesta */
int main(void){


	request_t req;
	pthread_t thid;
	pthread_attr_t t_attr;

	attr.mq_maxmsg = 10;
	attr.mq_msgsize = sizeof(request_t);


	server_q = mq_open("/SERVER", O_CREAT|O_RDONLY, 0700, &attr);
	if (server_q < 0){
		printf("Error al abrir la cola del servidor\n");
		return -1;
	}

	pthread_mutex_init(&mutex_request,NULL);
	pthread_cond_init(&request_cond, NULL);
	pthread_attr_init(&t_attr);

	pthread_attr_setdetachstate(&t_attr, PTHREAD_CREATE_DETACHED);
	while(1){

		if (mq_receive(server_q, (char *) &req, sizeof(req), 0) < 0){
			printf("Error recieving the request");
			fflush(stdout); 
			return -1;
		}
		if (pthread_create(&thid, &t_attr, (void *)tratar_peticion, (void*)&req)== 0){
			pthread_mutex_lock(&mutex_request);
			while(request_not_copied){
				pthread_cond_wait(&request_cond, &mutex_request);
			}
			request_not_copied = true;
			pthread_mutex_unlock(&mutex_request);
			
		}
	}
	// controlar señal para cerrar mutex

	return(0);
}
