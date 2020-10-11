#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

#define TAMBUFFER 32

void mensajeError(char *mensaje) { 
    perror(mensaje); 
    exit(1); 
}

void main(int argc, char *argv[]) {
    int sock;
    struct sockaddr_in echoserver;
    char buffer[TAMBUFFER];
    unsigned int echolen;
    int recibidos = 0;

    if (argc != 4) {
        fprintf(stderr, "Usar: %s <IP Servidor> <puerto> <palabra>\n",argv[0]);
        exit(1);
    }

    /* Crear el Socket TCP */
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        mensajeError("Falló al crear el socket");
    }

    /* Cargo la estructura del servidor */
    memset(&echoserver, 0, sizeof(echoserver));       /* Limpiar la Estructura */
    echoserver.sin_family = AF_INET;                  /* Internet/IP */
    echoserver.sin_addr.s_addr = inet_addr(argv[1]);  /* Direccion del Servidor */
    echoserver.sin_port = htons(atoi(argv[2]));       /* puerto servidor */

    /* Establece la conexion con el servidor  */
    if (connect(sock, (struct sockaddr *) &echoserver, sizeof(echoserver)) < 0) {
        mensajeError("Falló al conectar con el Servidor");
    }

    /* Envio la palabra al servidor */
    echolen = strlen(argv[3]);
    if (send(sock, argv[3], echolen, 0) != echolen) {
        mensajeError("Falló al enviar la información al servidor");
    }

    /* Recibe la palabra del servidor */
    fprintf(stdout, "Recibido: ");
    while (recibidos < echolen) {
        int bytes = 0;
        if ((bytes = recv(sock, buffer, TAMBUFFER-1, 0)) < 1) {
            mensajeError("Falló al recibir datos del servidor");
        }
        recibidos += bytes;
        buffer[bytes] = '\0';        /* Pone un null para terminar la cadena*/
        fprintf(stdout, buffer);
    }
    fprintf(stdout, "\n");
    close(sock);
    exit(0);
}
