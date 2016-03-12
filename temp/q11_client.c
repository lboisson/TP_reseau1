
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "common.h"
#include "chifoumi.h"

int main(int argc, char * argv[]) {
	//variables
	struct sockaddr_in clientAddress, serverAddress;
	struct hostent * serverHost;
	int clientSocket, err;
	void * line;
	choice_t * serverChoice, playerChoice;
	score_t * score;
	win_t * win;
	end_t * end;

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
		puts("Communication error");
		close(clientSocket);
		exit(EXIT_FAILURE);
	}
	free(line);
	send(clientSocket, "CHIFOUMI", 9, 0);
	line = read_line(clientSocket);
	if (line == NULL || strcmp(line, "OK") != 0) {
		puts("Communication error");
		close(clientSocket);
		exit(EXIT_FAILURE);
	}
	free(line);
	memcpy(playerChoice.CHOICE, "CHOICE", 6);
	playerChoice.SPACE = ' ';
	playerChoice.ZERO = '\0';
	while (1) {
		printf("Entrez %hhu pour pierre, %hhu pour feuille et %hhu pour ciseau : ", PIERRE, FEUILLE, CISEAU);
		scanf("%hhu", &playerChoice.choice);
		while (playerChoice.choice > 2) {
			puts("Invalide.");
			printf("Entrez %hhu pour pierre, %hhu pour feuille et %hhu pour ciseau : ", PIERRE, FEUILLE, CISEAU);
			scanf("%hhu", &playerChoice.choice);
		}
		switch (playerChoice.choice) {
			case PIERRE:
				puts("Vous avez choisi PIERRE.");
				break;
			case FEUILLE:
				puts("Vous avez choisi FEUILLE.");
				break;
			case CISEAU:
				puts("Vous avez choisi CISEAU.");
				break;
		}
		send(clientSocket, &playerChoice, sizeof(choice_t), 0);
		line = read_line(clientSocket);
		if (line == NULL || !start_with(line, "CHOICE")) {
			puts("Communication error");
			close(clientSocket);
			exit(EXIT_FAILURE);
		}
		serverChoice = line;
		switch (serverChoice->choice) {
			case PIERRE:
				puts("Le serveur a choisi PIERRE.");
				break;
			case FEUILLE:
				puts("Le serveur a choisi FEUILLE.");
				break;
			case CISEAU:
				puts("Le serveur a choisi CISEAU.");
				break;
		}
		free(line);
		send(clientSocket, "OK", 3, 0);
		line = read_line(clientSocket);
		if (line == NULL || !start_with(line, "WIN")) {
			puts("Communication error");
			close(clientSocket);
			exit(EXIT_FAILURE);
		}
		send(clientSocket, "OK", 3, 0);
		win = line;
		switch (win->winner) {
			case PLAYER:
				puts("Vous l'emportez.");
				break;
			case SERVER:
				puts("Le serveur l'emporte.");
				break;
			case NONE:
				puts("Egalité.");
				break;
		}
		free(line);
		line = read_line(clientSocket);
		if (line == NULL || !start_with(line, "SCORE")) {
			puts("Communication error");
			close(clientSocket);
			exit(EXIT_FAILURE);
		}
		send(clientSocket, "OK", 3, 0);
		score = line;
		printf("Scores :\t\tVous %hhu\t\tLe serveur %hhu\n", score->player_score, score->server_score);
		free(line);
		line = read_line(clientSocket);
		if (line == NULL || !start_with(line, "END")) {
			puts("Communication error");
			close(clientSocket);
			exit(EXIT_FAILURE);
		}
		end = line;
		send(clientSocket, "OK", 3, 0);
		if (end->winner == PLAYER) {
			puts("Vous avez gagné la partie.");
			free(line);
			break;
		} else if (end->winner == SERVER) {
			puts("Le serveur a gagné la partie.");
			free(line);
			break;
		}
		free(line);
	}

	//fin du programme
	close(clientSocket);
	puts("Connection closed.");
	exit(EXIT_SUCCESS);
}