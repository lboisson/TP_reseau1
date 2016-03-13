/* serveur TCP le numéro de port est passé comme argument */
#include <unistd.h> /* read write */
#include <stdlib.h> /* exit(0) */
#include <string.h> /* bzero */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> /* en-tête définit la structure sockaddr_in */

void error(char *msg) {
  perror(msg);
  exit(1);
}

int main(int argc, char *argv[]) {

  int sockfd, newsockfd, portno;
  char buffer[256];
  struct sockaddr_in serv_addr, cli_addr;
  int n;

  if (argc < 2)	{
    fprintf(stderr,"ERROR, no port provided\n");
    exit(1);
  }
  /* Creation d'un socket*/
  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd < 0) {
    error("ERROR opening socket");
  }
  /* RAZ de l'adresse serv */
  bzero((char *) &serv_addr, sizeof(serv_addr));
  /* char to int */
  portno = atoi(argv[1]);

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);/* définition du port */

  /* liaison entre le socker crée précédemment et la définition du serveur juste au dessus */
  if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
    error("ERROR on binding");
  }

  listen(sockfd,5);/* 5 client max */

  socklen_t clilen = sizeof(cli_addr);
  newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);
  /* extraire la première connexion de la file des connexions en attente */
  /* créez un nouveau socket avec le même protocole  */
  /* cli_addr contient TOUT */

  if (newsockfd < 0){
    error("ERROR on accept");
  }

  bzero(buffer,256);
  /* RAZ de buffer */
  bzero(buffer,256);/*raz*/
  n = read(newsockfd,buffer,255);
  if (n < 0) {
    error("ERROR reading from socket");
  }

  system("mkdir -p tmp");

  char fileName[100];

  memset (fileName, 0, sizeof (fileName));
  strcpy(fileName, "./tmp/");
  printf("file : %s ", buffer);
  strcat(fileName, buffer);
  printf("will be copyed in %s\n", fileName);

  FILE * file = fopen(fileName, "w");

  if (write(newsockfd,"OK",2) < 0){
    error("ERROR writing to socket");
  }

  bzero(buffer,256);
  n = read(newsockfd,buffer,255);
  while (n != 0) {
    if (n < 0) {
      error("ERROR reading from socket");
    }
    fwrite(buffer, sizeof(char), n , file); // n = sizeof(buffer)
    n = read(newsockfd,buffer,255);
  }
  fclose(file);
  printf("File copied.\n");
  close(newsockfd);
  close(sockfd);

  return 0;
}
