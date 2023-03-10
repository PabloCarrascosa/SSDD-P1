#include <mqueue.h>
#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include "claves.h"

pthread_mutex_t mutex_request;
int request_not_copied = true;
pthread_cond_t request_cond;
mqd_t server_q;
struct mq_attr attr;

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
		// INICIALIZA EL ENTORNO PARA ALMACENAR LAS TUPLAS
		struct stat st = {0};
		if (stat("/key_db", &st) == -1) {
			mkdir("/some/directory", 0700);
		}
		case SET_VALUE:
		// LE DA VALORES VALORES A LA TUPLA
		case GET_VALUE:
		// OBTIENE VALORES DE TUPLA
		case MODIFY_VALUE:
		// CAMBIA VALORES DE TUPLA
		case DELETE_KEY:
		// ELIMINA LA TUPLA
		case EXIST:
		// COMPRUEBA LA TUPLA
		

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
	Response res;
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