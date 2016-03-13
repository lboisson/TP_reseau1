/* client TCP pour envoyer une chaine de characteres.
L'adresse du serveur et le port sont passés comme argument */

#include <stdlib.h> /*exit(0)*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> /* en-tête définit la structure sockaddr_in */
#include <string.h> /* Lenght */
#include <netdb.h> /* gethostbyname */
#include <unistd.h> /* read write */

/*affiche un message d'erreur sur la sortie d'erreur standard*/
void error(char *msg) {
  perror(msg);
  exit(0);
}

int main(int argc, char *argv[])
{
  int sockfd, numero_port;
  struct sockaddr_in serv_addr;
  struct hostent *serveur;

  char buffer[256];

  /* dans le cas ou il n'y a pas suffisament d'arguments à la fonction*/
  if (argc < 3){
    fprintf(stderr,"usage : %s <hostname> <port>\n", argv[0]);
    exit(0);
  }

  /* char to int du 2nd argument */
  numero_port = atoi(argv[2]);

  /*Creation d'un socket, en IPv4 (AF_INET)*/
  sockfd = socket(AF_INET, SOCK_STREAM, 0); // SOCK_STREAM = TCP
  if (sockfd < 0){
    error("impossible de creer le socket");
  }

  /* adresse du serveur*/
  serveur = gethostbyname(argv[1]);
  if (serveur == NULL) {
    error("le serveur n'existe pas");
  }

  /* initialise serv_addr avec des 0 */
  bzero((char *) &serv_addr, sizeof(serv_addr));
  /*definit le protocole internet*/
  serv_addr.sin_family = AF_INET;
  /*copie l'adresse du serveur dans serv_addr*/
  bcopy((char *)serveur->h_addr,(char *)&serv_addr.sin_addr.s_addr,serveur->h_length);
  /*definit le port dans serv_addr */
  serv_addr.sin_port = htons(numero_port);

  /*Connection au serveur*/
  if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0){
    error("impossible de se connecter au serveur");
  }

  /* Communication*/
  printf("Saisissez votre message: ");
  bzero(buffer,256);         //le buffer est initialisé à 0
  fgets(buffer,255,stdin);   //fgets lit l'input, et le place dans buffer

  /* Envoie du message au serveur*/
  if ( write(sockfd,buffer,strlen(buffer))<0){
    error("erreur : pendant la lecture depuis le socket");
  }
  bzero(buffer,256);    //le buffer est initialisé

  /* reponse du serveur */
  if (read(sockfd,buffer,255) < 0){
    error("erreur : pendant l'écriture depuis le socket");
  }
  printf("%s\n",buffer);

  close(sockfd);
  return 0;
}
