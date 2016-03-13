/* serveur TCP qui recoit une chaine de characteres.
le numéro de port est passé comme argument */

#include <unistd.h> /* read write */
#include <stdlib.h> /* exit(0) */
#include <string.h> /* bzero */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> /* en-tête définit la structure sockaddr_in */

/*affiche un message d'erreur sur la sortie d'erreur standard*/
void error(char *msg) {
  perror(msg);
  exit(1);
}

int main(int argc, char *argv[]) {

  int sockfd, newsockfd, numero_port;
  char buffer[256];
  struct sockaddr_in serv_addr, cli_addr;

  /* dans le cas ou il n'y a pas suffisament d'arguments à la fonction*/
  if (argc < 2)	{
    fprintf(stderr,"usage : %s <port>\n", argv[0]);
    exit(1);
  }

  /*Creation d'un socket, en IPv4 (AF_INET)*/
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    error("impossible de creer le socket");
  }

  /*initialise serv_addr avec des 0 */
  bzero((char *) &serv_addr, sizeof(serv_addr));
  /* char to int */
  numero_port = atoi(argv[1]);

  /*definit le protocole internet*/
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(numero_port);/* définition du port */

  /* bind entre le socket et la definition serveur dans serv_addr */
  if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
    error("erreur : bind impossible");
  }

  /* attend les connections sur le socket. Limite la file a 5 connections. */
  listen(sockfd,5);

  socklen_t clilen = sizeof(cli_addr);
  newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);
  /* extraire la première connexion de la file des connexions en attente */
  /* créez un nouveau socket avec le même protocole  */
  /* cli_addr contient TOUT */

  if (newsockfd < 0){
    error("erreur sur la fonction accept");
  }

  /* le buffer est initialisé à 0*/
  bzero(buffer,256);

  if (read(newsockfd,buffer,255) < 0) {
    error("impossible de lire dans le socket");
  }

  printf("message recu: %s\n",buffer);

  if (write(newsockfd,"message recu par le serveur.", strlen("message recu par le serveur.") < 0)){
    error("impossible d'ecrire dans le socket");
  }
  close(newsockfd);
  close(sockfd);

  return 0;
}
