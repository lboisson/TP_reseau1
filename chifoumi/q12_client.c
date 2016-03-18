
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
	int clientSocket, err;
	socklen_t serverAddrLen = sizeof(serverAddress);
	char buffer[BUFFER_SIZE];
	int len;

	//arguments du programme
	if (argc != 2) {
		printf("%s <hostname>\n", argv[0]);
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
	clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
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

	//traitement
	puts("Entrez les messages que vous souhaitez envoyer ou /quit pour quitter.");
	while (1) {
		printf(">> ");
		scanf("%[^\n]s", buffer);
		getchar();
		if (strcmp(buffer, "/quit") == 0) {
			break;
		}
		len = strlen(buffer);
		if (len > 0) {
			err = sendto(clientSocket, buffer, len + 1, 0, (struct sockaddr *) &serverAddress, serverAddrLen);
			if (err <= 0) {
				puts("Cannot send data.");
				break;
			}
			err = recvfrom(clientSocket, buffer, 1, 0, (struct sockaddr *) &serverAddress, &serverAddrLen);
			if (err <= 0) {
				puts("Cannot receive data.");
				break;
			}
			printf("From server: %hhu\n\n", buffer[0]);
		}
	}

	//fin du programme
	close(clientSocket);
	puts("Connection closed.");
	exit(EXIT_SUCCESS);
}