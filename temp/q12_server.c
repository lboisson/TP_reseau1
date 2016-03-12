
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "common.h"

#define MAX_CLIENTS 5

int main() {
	//variables
	struct sockaddr_in serverAddress, clientAddress;
	char * ipClient;//pour l'affichage de l'adresse
	int serverSocket, err;
	socklen_t clientAddrLen = sizeof(clientAddress);
	char buffer[BUFFER_SIZE];
	unsigned char sum;
	int i, len;

	//création du socket
	serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
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

	while (1) {
		//traitement
		recvfrom(serverSocket, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &clientAddress, &clientAddrLen);
		ipClient = inet_ntoa(clientAddress.sin_addr);
		printf("From %s: %s\n", ipClient, buffer);
		sum = 0;
		len = strlen(buffer);
		for (i = 0 ; i < len ; i++) {
			sum += buffer[i];
		}
		sendto(serverSocket, &sum, 1, 0, (struct sockaddr *) &clientAddress, clientAddrLen);
	}
}