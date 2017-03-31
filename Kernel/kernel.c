#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include "configuracion.h"

void *handler_conexion(void *);
void* manejo_memoria();
void* manejo_filesystem();

Kernel* config;

int main(int argc, char **argv)
{
	int socket_desc, client_sock, c, port;
	struct sockaddr_in server, client;

    if (argc == 1)
    {
    	printf("Error. Parametros incorrectos \n");
    	return EXIT_FAILURE;
    }

    char* path = argv[1];

    Kernel* config = cargar_config(path);
	port = config->puerto_programa;

	// Creacion de socket
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if(socket_desc == -1)
	{
		printf("Error. No se pudo crear el socket de conexion");
	}
	puts("Socket de conexion creado correctamente");

	// struct de administracion del socket
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(port);

	// Asigno la IP y puerto al socket
	if(bind(socket_desc,(struct sockaddr *)&server, sizeof(server)) < 0)
	{
		perror("Error en el bind");
		return 1;
	}

	// El servidor se queda esperando a que se conecten los clientes
	listen(socket_desc, 3);

	c = sizeof(struct sockaddr_in);

	while((client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)))
	{
		//Se crea un hilo por cada conexion
		pthread_t thread_id;
		if(pthread_create( &thread_id, NULL,  handler_conexion, (void*) &client_sock) < 0)
		{
			perror("Error al crear el Hilo");
			return 1;
		}
	}
	if (client_sock < 0)
	{
		perror("Fallo en la conexion");
		return 1;
	}

	pthread_t thread_id_filesystem;
	pthread_t thread_id_memoria;

	if(pthread_create(&thread_id_filesystem, NULL, manejo_filesystem, NULL) < 0)
	{
		perror("Error al crear el Hilo");
		return 1;
	}

	if(pthread_create(&thread_id_memoria, NULL, manejo_memoria, NULL) < 0)
	{
		perror("Error al crear el Hilo");
		return 1;
	}

	return 0;
}

void *handler_conexion(void *socket_desc)
{
	// socket descriptor
	int client_socket = *(int*)socket_desc;
	int read_size;
	char *message, client_message[2000];

	// Envio de mensajes al cliente
	message = "Hola cliente\n";
	write(client_socket, message, strlen(message));

	// Compruebo si se realiza escritura en el socket cliente
	while((read_size = recv(client_socket, client_message, sizeof(client_message), 0)) > 0)
	{
		client_message[read_size] = '\0';
		// Envio al cliente el mensaje que escribio
		write(client_socket, client_message, strlen(client_message) + 1);
		memset(client_message, 0, sizeof(client_message));
	}
	if(read_size == 0)
	{
		puts("Cliente desconectado");
		fflush(stdout);
		close(client_socket);
	}
	else if(read_size == -1)
	{
		perror("Fallo en recv");
		fflush(stdout);
		close(client_socket);
	}
	return EXIT_SUCCESS;
}

void* manejo_memoria(){
	int sock;
	struct sockaddr_in server;
	char message[1000] = "";
	char server_reply[2000] = "";

	char* ip_memoria = config->ip_memoria;
	int port_memoria = config->puerto_memoria;

	//Creacion de Socket
	sock = socket(AF_INET , SOCK_STREAM , 0);

	if (sock == -1){
		printf("Error. No se pudo crear el socket de conexion\n");
	    return 0;
	}

	puts("Socket de conexion a Memoria creado correctamente\n");

	server.sin_addr.s_addr = inet_addr(ip_memoria);
	server.sin_family = AF_INET;
	server.sin_port = htons(port_memoria);

	//Conexion al Servidor
	if (connect(sock, (struct sockaddr *)&server , sizeof(server)) < 0){
		perror("Fallo el intento de conexion al servidor\n");
	    return 0;
	}

	puts("Conectado al servidor\n");

	//Loop para seguir comunicado con el servidor
	while(1){}

	close(sock);
	return 0;
}

void* manejo_filesystem(){
	int sock;
	struct sockaddr_in server;
	char message[1000] = "";
	char server_reply[2000] = "";

	char* ip_fs = config->ip_fs;
	int puerto_fs = config->puerto_fs;

	//Creacion de Socket
	sock = socket(AF_INET , SOCK_STREAM , 0);

	if (sock == -1){
		printf("Error. No se pudo crear el socket de conexion\n");
	    return 0;
	}

	puts("Socket de conexion a Memoria creado correctamente\n");

	server.sin_addr.s_addr = inet_addr(ip_fs);
	server.sin_family = AF_INET;
	server.sin_port = htons(puerto_fs);

	//Conexion al Servidor
	if (connect(sock, (struct sockaddr *)&server , sizeof(server)) < 0){
		perror("Fallo el intento de conexion al servidor\n");
	    return 0;
	}

	puts("Conectado al servidor\n");

	//Loop para seguir comunicado con el servidor
	while(1){}

	close(sock);
	return 0;
}
