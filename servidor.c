#include <mqueue.h>
#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include "claves.h"

pthread_mutex_t mutex_request;
int request_not_copied = true;
pthread_cond_t request_cond;
mqd_t server_q;
struct mq_attr attr;

int init(){
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

int set_value(int key, char *value1, int value2, double value3){
	DIR* dir = opendir("key_db");
	FILE* keydata;
	char path[1024];
	char full_path[2048];
	sprintf(path,"%d.txt", key);
	sprintf(full_path, "key_db/%s", path);

	if (dir){
		struct dirent *entry;
		while((entry = readdir(dir))!= 0){
			if (strcmp(entry->d_name, path) == 0){
				closedir(dir);
				return -1;
			}
		}
		keydata = fopen(full_path, "w");
		fprintf(keydata, "%d;%s;%d;%f",
		key, 
		value1, 
		value2, 
		value3);
		if (fclose(keydata) < 0){
			closedir(dir);
			return -1;
		}
		closedir(dir);
		return 0;
	}
	else{
		return -1;
	}
}
/*
int get_value(int key, char *value1, int *value2, double *value3){

}
*/
int modify_value(int key, char *value1, int value2, double value3){
	DIR* dir = opendir("key_db");
	FILE* keydata;
	char path[1024];
	char full_path[2048];
	sprintf(path, "%d.txt", key);
	sprintf(full_path, "key_db/%s", path);
	if (dir){
		struct dirent *entry;
		while((entry = readdir(dir))!= 0){
			if (strcmp(entry->d_name, path) == 0){
				keydata = fopen(full_path, "w");
				fprintf(keydata, "%d;%s;%d;%f",
				key, 
				value1, 
				value2, 
				value3);
				if (fclose(keydata) < 0){
					closedir(dir);
					return -1;
				}
				closedir(dir);
				return 0;
			}
		}
		closedir(dir);
		return -1;
	}
	else{
		return -1;
	}
}

int delete_key(int key){
	DIR* dir = opendir("key_db");
	char path[1024];
	char full_path[2048];
	sprintf(path, "%d.txt", key);
	sprintf(full_path, "key_db/%s", path);
	if (dir){
		struct dirent *entry;
		while((entry = readdir(dir))!= 0){
			if (strcmp(entry->d_name, path) == 0){
				
				unlink(full_path);
				closedir(dir);
				return 0;
			}
		}
		closedir(dir);
		return -1;
	}
	else{
		return -1;
	}
}
int exist(int key){
	DIR* dir = opendir("key_db");
	char path[1024];
	sprintf(path, "%d.txt", key);
	if (dir){
		struct dirent *entry;
		while((entry = readdir(dir))!= 0){
			if (strcmp(entry->d_name, path) == 0){
				closedir(dir);
				return 1;
			}
		}
		closedir(dir);
		return -1;
	}
	else{
		return -1;
	}
}

/* Función llamada para tratar las peticiones recibidas */
void tratar_peticion(void *req){
	Request request;
	Response response;
	mqd_t client_q;
	
	pthread_mutex_lock(&mutex_request);
	request = (*(Request *)req);
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
	Request req;
	pthread_t thid;
	pthread_attr_t t_attr;

	attr.mq_maxmsg = 10;
	attr.mq_msgsize = sizeof(Request);


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
