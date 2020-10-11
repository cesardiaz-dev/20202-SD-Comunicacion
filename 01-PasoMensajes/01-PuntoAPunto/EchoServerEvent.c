#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/select.h>

#define MAXPENDING 5    /* Max solicitudes de conexion */
#define TAMBUFFER 32
#define TAMARRAY   6

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
    /* Enviar datos de vuelta */
    if (send(sock, buffer, recibidos, 0) != recibidos) {
        mensajeError("Falló el envio de datos con el cliente");
    }
}

int main(int argc, char *argv[]) {
    int serversock, clientsock;
    struct sockaddr_in echoserver, echoclient;

    fd_set allset;
    fd_set rset;
    int maxfd;

    int client[TAMARRAY];
    int maxi;
    int nready;
    int i, sockfd;


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

    /* Inicializar los valores del select*/
    maxi = -1;
    maxfd = serversock;
    for(i=0; i<TAMARRAY; i++)
        client[i] = -1;
    FD_ZERO(&allset);
    FD_SET(serversock, &allset);

    /* Run until cancelled */
    while (1) {
        rset = allset;

        nready = select(maxfd+1, &rset, NULL, NULL, NULL);

        if(FD_ISSET(serversock, &rset)){
            unsigned int clientlen = sizeof(echoclient);
    
            /* Wait for client connection */
            if ((clientsock = accept(serversock, (struct sockaddr *) &echoclient, &clientlen)) < 0) {
                mensajeError("Failed to accept client connection");
            }
            fprintf(stdout, "Cliente conectado: %s:%d\n", 
                            inet_ntoa(echoclient.sin_addr), 
                            ntohs(echoclient.sin_port));

            nready--;

            for(i=0; i<TAMARRAY; i++)
                if(client[i] < 0) {
                    client[i] = clientsock;
                    break;
                }

            if(i == TAMARRAY)
                mensajeError("Demasiados clientes\n");

            FD_SET(clientsock, &allset);
            if(clientsock > maxfd)
                maxfd = clientsock;

            if(i > maxi)
                maxi = i;
        }

        for(i=0; (i <= maxi) && (nready > 0); i++) {
            sockfd = client[i];
            if((sockfd > 0) && (FD_ISSET(sockfd, &rset))) {
                echo(sockfd);
                close(sockfd);
                FD_CLR(sockfd, &allset);
                client[i] = -1;
                nready--;
            }
        }

    }
}
