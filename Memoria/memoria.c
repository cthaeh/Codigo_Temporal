
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <commons/config.h>
#include <commons/string.h>

typedef struct {
	int puerto;
	int marcos;
	int marco_size;
	int entradas_cache;
	int cache_x_proc;
	char* reemplazo_cache;
	int retardo_memoria;
} Configuracion;

Configuracion* leer_configuracion(char* directorio);

//Llamada para ejecutar: ./Cliente IP nroDePuerto ej: ./Cliente 127.0.0.1 7070

int main(int argc , char **argv)
{
    int sock;
    struct sockaddr_in server;
    char message[1000] = "";
    char server_reply[2000] = "";

    int socket_desc, client_sock, c;

    struct sockaddr client;

    if (argc <= 1)
    {
    	printf("Error. Parametros incorrectos \n");
    	return EXIT_FAILURE;
    }

    Configuracion* config = malloc (sizeof(Configuracion));

	config = leer_configuracion(argv[1]);
	printf("Mi archivo dice: PUERTO = %d .\n RETARDO_MEMORIA = %d . \n REEMPLAZO = %s . \n", config->puerto, config->retardo_memoria, config->reemplazo_cache);
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if(socket_desc == -1)
	{
		printf("Error. No se pudo crear el socket de conexion");
	}
	puts("Socket de conexion creado correctamente");

	// struct de administracion del socket
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(config->puerto);

	// Asigno la IP y puerto al socket
	if(bind(socket_desc,(struct sockaddr *)&server, sizeof(server)) < 0)
	{
		perror("Error en el bind");
		return 1;
	}

	// El servidor se queda esperando a que se conecten los clientes
	listen(socket_desc, 3);

	while((client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c))){

	}


	if (client_sock < 0)
	{
		perror("Fallo en la conexion");
		return 1;
	}

	return 0;
}

Configuracion* leer_configuracion(char* directorio){
	char* path = string_new();

    string_append(&path,directorio);
    string_append(&path,"/Memoria");


	t_config* config_memoria = config_create(path);

	Configuracion* config = malloc(sizeof(Configuracion));

	config->cache_x_proc = config_get_int_value(config_memoria, "CACHE_X_PROC");
	config->entradas_cache = config_get_int_value(config_memoria, "ENTRADAS_CACHE");
	config->marco_size = config_get_int_value(config_memoria, "MARCO_SIZE");
	config->marcos = config_get_int_value(config_memoria, "MARCOS");
	config->puerto = config_get_int_value(config_memoria, "PUERTO");
	config->reemplazo_cache = config_get_string_value(config_memoria, "REEMPLAZO_CACHE");
	config->retardo_memoria = config_get_int_value(config_memoria, "RETARDO_MEMORIA");

	return config;
}
