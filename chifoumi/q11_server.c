
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "common.h"
#include "chifoumi.h"

#define MAX_CLIENTS 5

int main() {
	//variables
	struct sockaddr_in serverAddress, clientAddress;
	char * ipClient;//pour l'affichage de l'adresse
	int serverSocket, clientSocket, err;
	socklen_t clientAddrLen = sizeof(clientAddress);
	void * line;
	choice_t * playerChoice, serverChoice;
	score_t score;
	win_t win;
	end_t end;

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
		srand(time(NULL));
		memcpy(serverChoice.CHOICE, "CHOICE", 6);
		serverChoice.SPACE = ' ';
		serverChoice.ZERO = '\0';
		memcpy(score.SCORE, "SCORE", 5);
		score.SPACE = ' ';
		score.ZERO = '\0';
		score.player_score = 0;
		score.server_score = 0;
		memcpy(win.WIN, "WIN", 3);
		win.SPACE = ' ';
		win.ZERO = '\0';
		memcpy(end.END, "END", 3);
		end.SPACE = ' ';
		end.ZERO = '\0';
		line = read_line(clientSocket);
		if (line == NULL || strcmp(line, "HELLO") != 0) {
			puts("Communication error.");
			close(clientSocket);
			break;
		}
		free(line);
		send(clientSocket, "OK", 3, 0);
		line = read_line(clientSocket);
		if (line == NULL || strcmp(line, "CHIFOUMI") != 0) {
			puts("Communication error.");
			close(clientSocket);
			break;
		}
		free(line);
		send(clientSocket, "OK", 3, 0);
		while (1) {
			line = read_line(clientSocket);
			if (line == NULL || !start_with(line, "CHOICE")) {
				break;
			}
			playerChoice = line;
			serverChoice.choice = rand() % 3;
			send(clientSocket, &serverChoice, sizeof(choice_t), 0);
			if (((playerChoice->choice + 1) % 3) == serverChoice.choice) {
				win.winner = SERVER;
				score.server_score++;
			} else if (((serverChoice.choice + 1) % 3) == playerChoice->choice) {
				win.winner = PLAYER;
				score.player_score++;
			} else {
				win.winner = NONE;
			}
			free(line);
			line = read_line(clientSocket);
			if (line == NULL || strcmp(line, "OK") != 0) {
				break;
			}
			free(line);
			send(clientSocket, &win, sizeof(win_t), 0);
			line = read_line(clientSocket);
			if (line == NULL || strcmp(line, "OK") != 0) {
				break;
			}
			free(line);
			send(clientSocket, &score, sizeof(score_t), 0);
			line = read_line(clientSocket);
			if (line == NULL || strcmp(line, "OK") != 0) {
				break;
			}
			free(line);
			if (score.player_score == 3) {
				end.winner = PLAYER;
			} else if (score.server_score == 3) {
				end.winner = SERVER;
			} else {
				end.winner = NONE;
			}
			send(clientSocket, &end, sizeof(end_t), 0);
			line = read_line(clientSocket);
			if (line == NULL || strcmp(line, "OK") != 0) {
				break;
			}
			free(line);
		}

		//fermeture de la connection
		while (read_line(clientSocket) != NULL);
		printf("Connection with %s closed.\n", ipClient);
	}
}