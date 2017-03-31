#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <commons/config.h>
#include <commons/string.h>
#include <pthread.h>

//Llamada para ejecutar: ./Cliente IP nroDePuerto ej: ./Cliente 127.0.0.1 7070

void* manejo_memoria();
void* manejo_kernel();

int main(int argc , char **argv){

	pthread_t thread_id_kernel;
	pthread_t thread_id_memoria;

	if(pthread_create( &thread_id_kernel, NULL,  manejo_kernel, NULL) < 0)
	{
		perror("Error al crear el Hilo");
		return 1;
	}

	if(pthread_create( &thread_id_memoria, NULL, manejo_memoria, NULL) < 0)
	{
		perror("Error al crear el Hilo");
		return 1;
	}

	while(1){}

    return EXIT_SUCCESS;
}

void* manejo_kernel(){
	int sock;
	struct sockaddr_in server;
	char message[1000] = "";
	char server_reply[2000] = "";

	char* ip = "127.0.0.1";
	int port_kernel = 5000;

	//Creacion de Socket
	sock = socket(AF_INET , SOCK_STREAM , 0);

	if (sock == -1){
		printf("Error. No se pudo crear el socket de conexion\n");
	    return 0;
	}

	puts("Socket de conexion a Kernel creado correctamente\n");

	server.sin_addr.s_addr = inet_addr(ip);
	server.sin_family = AF_INET;
	server.sin_port = htons(port_kernel);

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

void* manejo_memoria(){
	int sock;
	struct sockaddr_in server;
	char message[1000] = "";
	char server_reply[2000] = "";

	char* ip = "127.0.0.1";
	int port_memoria = 5003;

	//Creacion de Socket
	sock = socket(AF_INET , SOCK_STREAM , 0);

	if (sock == -1){
		printf("Error. No se pudo crear el socket de conexion\n");
		return 0;
	}

	puts("Socket de conexion a Memoria creado correctamente\n");

	server.sin_addr.s_addr = inet_addr(ip);
	server.sin_family = AF_INET;
	server.sin_port = htons(port_memoria);

	//Conexion al Servidor
	if (connect(sock, (struct sockaddr *)&server , sizeof(server)) < 0)	    {
		perror("Fallo el intento de conexion al servidor\n");
		return 0;
	}

	puts("Conectado al servidor\n");

	//Loop para seguir comunicado con el servidor
	while(1){}

	close(sock);
	return 0;
}
