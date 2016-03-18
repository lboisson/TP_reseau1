
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "common.h"

#define MAX_CLIENTS 5

int main() {
	//variables
	struct sockaddr_in serverAddress, clientAddress;
	char * ipClient;//pour l'affichage de l'adresse
	int serverSocket, clientSocket, err;
	socklen_t clientAddrLen = sizeof(clientAddress);
	char * line;

	//création du socket
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket < 0) {
		puts("Cannot open socket.");
		exit(EXIT_FAILURE);
	}
	puts("Socket openned.");

	//liaison au port
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddress.sin_port = htons(SERVER_PORT);
	err = bind(serverSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress));
	if (err < 0) {
		printf("Cannot bind port %hu.\n", SERVER_PORT);
		close(serverSocket);
		exit(EXIT_FAILURE);
	}
	printf("Port %hu bound.\n", SERVER_PORT);

	//écoute du port
	err = listen(serverSocket, MAX_CLIENTS);
	if (err < 0) {
		printf("Cannot listen port %hu.\n", SERVER_PORT);
		close(serverSocket);
		exit(EXIT_FAILURE);
	}
	printf("Listening port %hu.\n", SERVER_PORT);

	while (1) {
		//attente de connection
		clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddress, &clientAddrLen);
		if (clientSocket < 0) {
			puts("Cannot accept connection.");
			close(serverSocket);
			exit(EXIT_FAILURE);
		}
		ipClient = inet_ntoa(clientAddress.sin_addr);
		printf("New connection from %s.\n", ipClient);

		//traitement
		line = read_line(clientSocket);
		if (line == NULL || strcmp(line, "HELLO") != 0) {
			puts("Communication error.");
			close(clientSocket);
			break;
		}
		free(line);
		send(clientSocket, "OK", 3, 0);
		line = read_line(clientSocket);
		if (line == NULL || strcmp(line, "ECHO") != 0) {
			puts("Communication error.");
			close(clientSocket);
			break;
		}
		free(line);
		send(clientSocket, "REPLY", 6, 0);

		//fermeture de la connection
		while (read_line(clientSocket) != NULL);
		printf("Connection with %s closed.\n", ipClient);
	}
}