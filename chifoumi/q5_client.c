
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "common.h"

int main(int argc, char * argv[]) {
	//variables
	struct sockaddr_in clientAddress, serverAddress;
	struct hostent * serverHost;
	int clientSocket, err, n;
	char * line, buffer[BUFFER_SIZE];
	FILE * file;

	//arguments du programme
	if (argc != 3) {
		printf("%s <hostname> <file>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	//résolution de l'hôte
	serverHost = gethostbyname(argv[1]);
	if (serverHost == NULL) {
		printf("Unknown host %s.\n", argv[1]);
		exit(EXIT_FAILURE);
	}
	printf("Host %s found.\n", argv[1]);
	serverAddress.sin_family = serverHost->h_addrtype;
	memcpy(&serverAddress.sin_addr.s_addr, serverHost->h_addr_list[0], serverHost->h_length);
	serverAddress.sin_port = htons(SERVER_PORT);

	//création du socket
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket < 0) {
		puts("Cannot open socket");
		exit(EXIT_FAILURE);
	}
	puts("Socket openned.");

	//liaison à un port
	clientAddress.sin_family = AF_INET;
	clientAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	clientAddress.sin_port = htons(0);
	err = bind(clientSocket, (struct sockaddr *) &clientAddress, sizeof(clientAddress));
	if (err < 0) {
		puts("Cannot bind port %hu.");
		close(clientSocket);
		exit(EXIT_FAILURE);
	}
	puts("Port bound.");

	//connection au serveur
	err = connect(clientSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress));
	if (err < 0) {
		printf("Cannot connect to server %s.\n", argv[1]);
		close(clientSocket);
		exit(EXIT_FAILURE);
	}
	printf("Connected to %s.\n", argv[1]);

	//traitement
	send(clientSocket, "HELLO", 6, 0);
	line = read_line(clientSocket);
	if (line == NULL || strcmp(line, "OK") != 0) {
		puts("Communication error.");
		close(clientSocket);
		exit(EXIT_FAILURE);
	}
	free(line);
	file = fopen(argv[2], "r");
	if (file == NULL) {
		printf("Unable to open file %s.\n", argv[2]);
		close(clientSocket);
		exit(EXIT_FAILURE);
	}
	line = malloc(6 + sizeof(argv[2]));
	sprintf(line, "FILE %s", argv[2]);
	send(clientSocket, line, 6 + sizeof(argv[2]), 0);
	free(line);
	line = read_line(clientSocket);
	if (line == NULL || strcmp(line, "OK") != 0) {
		puts("Communication error.");
		close(clientSocket);
		exit(EXIT_FAILURE);
	}
	free(line);
	while ((n = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
		if (send(clientSocket, buffer, n, 0) != n) {
			puts("Communication error.");
			close(clientSocket);
			exit(EXIT_FAILURE);
		}
	}
	printf("File %s transferred.\n", argv[2]);
	fclose(file);

	//fin du programme
	close(clientSocket);
	puts("Connection closed.");
	exit(EXIT_SUCCESS);
}