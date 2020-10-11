#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

#define MAXPENDING 5    /* Max solicitudes de conexion */
#define TAMBUFFER 32

void mensajeError(char *mensaje) { 
    perror(mensaje); 
    exit(1); 
}

void echo(int sock) {
    char buffer[TAMBUFFER];
    int recibidos = -1;
    
    /* Lee el mensaje del Cliente */
    if ((recibidos = recv(sock, buffer, TAMBUFFER, 0)) < 0) {
        mensajeError("Error al recibir información del cliente");
    }

    /* Enviar la respuesta y verificar por mas mensajes de entrada */
    while (recibidos > 0) {
        /* Enviar datos de vuelta */
        if (send(sock, buffer, recibidos, 0) != recibidos) {
            mensajeError("Falló el envio de datos con el cliente");
        }

        /* Verifica si hay mas mensajes */
        if ((recibidos = recv(sock, buffer, TAMBUFFER, 0)) < 0) {
                mensajeError("Falló al recibir datos adicionales del cliente");
        }
    }
    close(sock);
}

int main(int argc, char *argv[]) {
    int serversock, clientsock;
    struct sockaddr_in echoserver, echoclient;

    if (argc != 2) {
        fprintf(stderr, "USAR: %s <puerto>\n",argv[0]);
        exit(1);
    }

    /* Crear el Socket TCP */
    if ((serversock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        mensajeError("Falló al crear el socket");
    }

    /* Cargo la estructura del servidor */
    memset(&echoserver, 0, sizeof(echoserver));       /* Limpiar la Estructura */
    echoserver.sin_family = AF_INET;                  /* Internet/IP */
    echoserver.sin_addr.s_addr = htonl(INADDR_ANY);   /* Direccion Entrada */
    echoserver.sin_port = htons(atoi(argv[1]));       /* puerto servidor */

    /* Enlazar (Bind) el socket al Servidor */
    if (bind(serversock, (struct sockaddr *) &echoserver, sizeof(echoserver)) < 0) {
        mensajeError("Falló al enlazar el socket al servidor");
    }

    /* Escuchar el socket */
    if (listen(serversock, MAXPENDING) < 0) {
        mensajeError("Falló al iniciar la escucha del socket");
    }

    /* Run until cancelled */
    while (1) {
        unsigned int clientlen = sizeof(echoclient);
    
        /* Wait for client connection */
        if ((clientsock = accept(serversock, (struct sockaddr *) &echoclient, &clientlen)) < 0) {
            mensajeError("Failed to accept client connection");
        }

        if(fork() == 0) {
            close(serversock);
            fprintf(stdout, "Cliente conectado: %s:%d\n", inet_ntoa(echoclient.sin_addr), ntohs(echoclient.sin_port));
            echo(clientsock);
            exit(0);
        }
        close(clientsock);
    }
}
