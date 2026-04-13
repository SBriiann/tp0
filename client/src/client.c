#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	// 1. Iniciamos el logger con la función que definimos abajo
	logger = iniciar_logger();

	// 2. Loggeamos el mensaje que pide la guía
	log_info(logger, "Hola! Soy un log");


	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	// 3. Iniciamos la config
	config = iniciar_config();

	// 4. Extraemos los valores del archivo "cliente.config"
	// Usamos las variables que declaramos al principio del main
	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");
	valor = config_get_string_value(config, "CLAVE");

	// 5. Loggeamos el valor de la CLAVE
	log_info(logger, valor);


	/* ---------------- LEER DE CONSOLA ---------------- */

	// 6. Llamamos a la función para que el usuario escriba
	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo

	// 7. Creamos una conexión hacia el servidor usando la IP y Puerto del config
	conexion = crear_conexion(ip, puerto);
	enviar_mensaje(valor, conexion);

	// 8. Enviamos el paquete (se completa en la siguiente parte del TP)
	paquete(conexion);

	// 9. Liberamos toda la memoria antes de terminar
	terminar_programa(conexion, logger, config);

	return 0;
}

/* ----------------------- DEFINICIÓN DE FUNCIONES ----------------------- */

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger;

	// Crea el logger. Archivo: tp0.log, Nombre: CLIENTE, Consola: SI, Nivel: INFO
	nuevo_logger = log_create("tp0.log", "CLIENTE", true, LOG_LEVEL_INFO);

	if (nuevo_logger == NULL) {
		perror("Error al crear el logger");
		exit(1);
	}

	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;

	// Intenta abrir el archivo de configuración
	nuevo_config = config_create("cliente.config");

	if (nuevo_config == NULL) {
		perror("Error al cargar el archivo de config");
		exit(2);
	}

	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	// Leemos la primera línea de la consola
	leido = readline("> ");

	// Mientras el usuario no mande un mensaje vacío
	while (leido != NULL && strcmp(leido, "") != 0) {
		// Loggeamos lo que escribió el usuario
		log_info(logger, leido);
		
		// ¡Importante! Liberamos la memoria de la línea leída
		free(leido);
		
		// Volvemos a leer para la siguiente iteración
		leido = readline("> ");
	}

	// Liberamos la memoria de la última lectura (la que fue vacía)
	free(leido);
}

void paquete(int conexion)
{

	char* leido;
	t_paquete* mi_paquete;

	mi_paquete = crear_paquete();

leido = readline("> ");

    while (leido != NULL && strcmp(leido, "") != 0) {
        
        // 3. En lugar de solo anotar en el log, lo METEMOS en la caja
        // Usamos strlen(leido) + 1 para que viaje el final del texto (\0)
        agregar_a_paquete(mi_paquete, leido, strlen(leido) + 1);

        // 4. Liberamos la memoria de lo que leímos
        free(leido);

        // 5. Volvemos a pedir otra línea
        leido = readline("> ");
    }
    
    // 6. ¡Libertad! El usuario mandó Enter vacío, liberamos ese último leido
    free(leido);

    // 7. AHORA SÍ: Despachamos la caja completa al servidor
    enviar_paquete(mi_paquete, conexion);

    // 8. Tiramos la caja vacía porque ya la enviamos
    eliminar_paquete(mi_paquete);


	// Esta parte se completa en la Parte 3 del TP
	// Por ahora la dejamos vacía para que el código compile
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	// Cerramos y liberamos el logger
	if (logger != NULL) {
		log_destroy(logger);
	}

	// Cerramos y liberamos la configuración
	if (config != NULL) {
		config_destroy(config);
	}

	liberar_conexion(conexion);

	// La conexión se liberará cuando se implemente crear_conexion
}
