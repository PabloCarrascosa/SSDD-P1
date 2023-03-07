#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "claves.h"



int init(void)
{
	mqd_t q_server;   // Cola del servidor
	mqd_t q_client;   // Cola del cliente
	
	Request req;
	Response res;       

	struct mq_attr attr;
	char queuename[MAX_SIZE];

   	attr.mq_maxmsg = 1;  
	attr.mq_msgsize = sizeof(Response);

	q_server = mq_open("/SERVER", O_WRONLY);
	if (q_server == -1){
		perror("mq_open 2");
		return (-1);
	}

	sprintf(queuename,  "/Cola-%d", getpid());
	q_client = mq_open(queuename, O_CREAT|O_RDONLY, 0700, &attr);
	if (q_client == -1) {
		mq_close(q_client); 
		perror("mq_open 1");
		return (-1);
	}

	strcpy(req.name, queuename);
	req.op = INIT; 

	if (mq_send(q_server, (const char *) &req, sizeof(req), 0) < 0){
		perror("mq_send");
		return (-1);
	}	
    if (mq_receive(q_client, (char *) &res, sizeof(Response), 0) < 0){
		perror("mq_recv");
		return (-1);
	}	
	mq_close(q_server); 
	mq_close(q_client);
	mq_unlink(req.name); 
	return (res.status);  
}

int set_value(int key, char *value1, int value2, double value3)
{
	mqd_t q_server;   // Cola del servidor
	mqd_t q_client;   // Cola del cliente
	
	Request req;
	Response res;       

	struct mq_attr attr;
	char queuename[MAX_SIZE];

   	attr.mq_maxmsg = 1;  
	attr.mq_msgsize = sizeof(Response);

	q_server = mq_open("/SERVER", O_WRONLY);
	if (q_server == -1){
		perror("mq_open 2");
		return (-1);
	}

	sprintf(queuename,  "/Cola-%d", getpid());
	q_client = mq_open(queuename, O_CREAT|O_RDONLY, 0700, &attr);
	if (q_client == -1) {
		mq_close(q_client); 
		perror("mq_open 1");
		return (-1);
	}

	strcpy(req.name, queuename);
	req.op = SET_VALUE; 
	req.data.key = key; 
	strcpy(req.data.value1, value1);
	req.data.value2 = value2; 
	req.data.value3 = value3; 


	if (mq_send(q_server, (const char *) &req, sizeof(req), 0) < 0){
		perror("mq_send");
		return (-1);
	}	
    if (mq_receive(q_client, (char *) &res, sizeof(Response), 0) < 0){
		perror("mq_recv");
		return (-1);
	}	
	mq_close(q_server); 
	mq_close(q_client);
	mq_unlink(req.name); 
	return (res.status); 
}

int get_value(int key, char *value1, int *value2, double *value3)
{
	mqd_t q_server;   // Cola del servidor
	mqd_t q_client;   // Cola del cliente
	
	Request req;
	Response res;       

	struct mq_attr attr;
	char queuename[MAX_SIZE];
   
	attr.mq_maxmsg = 1;     
	attr.mq_msgsize = sizeof(Response);

	q_server = mq_open("/SERVER", O_WRONLY);
	if (q_server == -1){
		perror("mq_open 2");
		return (-1);
	}

	sprintf(queuename,  "/Cola-%d", getpid());
	q_client = mq_open(queuename, O_CREAT|O_RDONLY, 0700, &attr);
	if (q_client == -1) {
		mq_close(q_client); 
		perror("mq_open 1");
		return (-1);
	}


	strcpy(req.name, queuename);
	req.op = GET_VALUE; 
	req.data.key = key; 


	if (mq_send(q_server, (const char *) &req, sizeof(req), 0) < 0){
		perror("mq_send");
		return (-1);
	}	
    if (mq_receive(q_client, (char *) &res, sizeof(Response), 0) < 0){
		perror("mq_recv");
		return (-1);
	}	

	if(res.status){
		strcpy(value1, res.data.value1); 
		*value2 = res.data.value2;
		*value3 = res.data.value3;  
	}

	mq_close(q_server); 
	mq_close(q_client);
	mq_unlink(req.name); 
	return (res.status); 
}

int modify_value(int key, char *value1, int value2, double value3)
{
	mqd_t q_server;   // Cola del servidor
	mqd_t q_client;   // Cola del cliente
	
	Request req;
	Response res;       

	struct mq_attr attr;
	char queuename[MAX_SIZE];
   
   	attr.mq_maxmsg = 1;  
	attr.mq_msgsize = sizeof(Response);

	q_server = mq_open("/SERVER", O_WRONLY);
	if (q_server == -1){
		perror("mq_open 2");
		return (-1);
	}

	sprintf(queuename,  "/Cola-%d", getpid());
	q_client = mq_open(queuename, O_CREAT|O_RDONLY, 0700, &attr);
	if (q_client == -1) {
		mq_close(q_client); 
		perror("mq_open 1");
		return (-1);
	}


	strcpy(req.name, queuename);
	req.op = MODIFY_VALUE; 
	req.data.key = key; 
	strcpy(req.data.value1, value1);
	req.data.value2 = value2; 
	req.data.value3 = value3; 


	if (mq_send(q_server, (const char *) &req, sizeof(req), 0) < 0){
		perror("mq_send");
		return (-1);
	}	
    if (mq_receive(q_client, (char *) &res, sizeof(Response), 0) < 0){
		perror("mq_recv");
		return (-1);
	}

	mq_close(q_server); 
	mq_close(q_client);
	mq_unlink(req.name); 
	return (res.status);  
}

int delete_key(int key)
{
	mqd_t q_server;   // Cola del servidor
	mqd_t q_client;   // Cola del cliente
	
	Request req;
	Response res;       

	struct mq_attr attr;
	char queuename[MAX_SIZE];
   
   	attr.mq_maxmsg = 1;  
	attr.mq_msgsize = sizeof(Response);

	q_server = mq_open("/SERVER", O_WRONLY);
	if (q_server == -1){
		perror("mq_open 2");
		return (-1);
	}

	sprintf(queuename,  "/Cola-%d", getpid());
	q_client = mq_open(queuename, O_CREAT|O_RDONLY, 0700, &attr);
	if (q_client == -1) {
		mq_close(q_client); 
		perror("mq_open 1");
		return (-1);
	}


	strcpy(req.name, queuename);
	req.op = DELETE_KEY;
	req.data.key = key; 

	if (mq_send(q_server, (const char *) &req, sizeof(req), 0) < 0){
		perror("mq_send");
		return (-1);
	}	
    if (mq_receive(q_client, (char *) &res, sizeof(Response), 0) < 0){
		perror("mq_recv");
		return (-1);
	}	

	mq_close(q_server); 
	mq_close(q_client);
	mq_unlink(req.name); 
	return (res.status);  
}

int exist(int key)
{
	mqd_t q_server;   // Cola del servidor
	mqd_t q_client;   // Cola del cliente
	
	Request req;
	Response res;       

	struct mq_attr attr;
	char queuename[MAX_SIZE];
   
   	attr.mq_maxmsg = 1;  
	attr.mq_msgsize = sizeof(Response);

	q_server = mq_open("/SERVER", O_WRONLY);
	if (q_server == -1){
		perror("mq_open 2");
		return (-1);
	}

	sprintf(queuename,  "/Cola-%d", getpid());
	q_client = mq_open(queuename, O_CREAT|O_RDONLY, 0700, &attr);
	if (q_client == -1) {
		mq_close(q_client); 
		perror("mq_open 1");
		return (-1);
	}


	strcpy(req.name, queuename);
	req.op = EXIST;
	req.data.key = key; 

	if (mq_send(q_server, (const char *) &req, sizeof(req), 0) < 0){
		perror("mq_send");
		return (-1);
	}	
    if (mq_receive(q_client, (char *) &res, sizeof(Response), 0) < 0){
		perror("mq_recv");
		return (-1);
	}	

	mq_close(q_server); 
	mq_close(q_client);
	mq_unlink(req.name); 
	return (res.status);
}

int copy_key(int key1, int key2)
{
	mqd_t q_server;   // Cola del servidor
	mqd_t q_client;   // Cola del cliente
	
	Request req; 

	Response res;       

	struct mq_attr attr;
	char queuename[MAX_SIZE];
   
   	attr.mq_maxmsg = 1;  
	attr.mq_msgsize = sizeof(Response);

	q_server = mq_open("/SERVER", O_WRONLY);
	if (q_server == -1){
		perror("mq_open 2");
		return (-1);
	}

	sprintf(queuename,  "/Cola-%d", getpid());
	q_client = mq_open(queuename, O_CREAT|O_RDONLY, 0700, &attr);
	if (q_client == -1) {
		mq_close(q_client); 
		perror("mq_open 1");
		return (-1);
	}


	strcpy(req.name, queuename);
	req.op = GET_VALUE;
	req.data.key = key1; 

	if (mq_send(q_server, (const char *) &req, sizeof(req), 0) < 0){
		perror("mq_send");
		return (-1);
	}	
    if (mq_receive(q_client, (char *) &res, sizeof(Response), 0) < 0){
		perror("mq_recv");
		return (-1);
	}	

	if(!res.status){
		mq_close(q_server); 
		mq_close(q_client);
		mq_unlink(req.name); 
		return (res.status);
	}

	req.data.key = key2; 
	strcpy(req.data.value1, res.data.value1); 
	req.data.value2 = res.data.value2; 
	req.data.value3 = res.data.value3; 

	if (mq_send(q_server, (const char *) &req, sizeof(req), 0) < 0){
		perror("mq_send");
		return (-1);
	}	
    if (mq_receive(q_client, (char *) &res, sizeof(Response), 0) < 0){
		perror("mq_recv");
		return (-1);
	}	

	mq_close(q_server); 
	mq_close(q_client);
	mq_unlink(req.name); 
	return (res.status);
}