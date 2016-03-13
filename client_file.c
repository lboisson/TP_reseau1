/* client TCP le numéro de l'adresse et le port est passé comme argument */

#include <stdlib.h> /*exit(0)*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> /* en-tête définit la structure sockaddr_in */
#include <string.h> /* Lenght */
#include <netdb.h> /* gethostbyname */
#include <unistd.h> /* read write */

#define CHUNK 1024 /* read 1024 bytes at a time */

void error(char *msg) {
  perror(msg);
  exit(0);
}

int main(int argc, char *argv[])
{
  int sockfd, portno;
  struct sockaddr_in serv_addr;
  struct hostent *server;

  char buffer[256];

  /* usage */
  if (argc < 3){
    fprintf(stderr,"usage : %s <hostname> <port>\n", argv[0]);
    exit(0);
  }

  /* char to int */
  portno = atoi(argv[2]);

  /*Creation d'un socket*/
  sockfd = socket(AF_INET, SOCK_STREAM, 0); // SOCK_STREAM = TCP
  if (sockfd < 0){
    error("impossible de creer le socket");
  }


  server = gethostbyname(argv[1]);/* adresse du serveur*/
  if (server == NULL) {
    error("le serveur n'existe pas");
  }

  /* RAZ de l'adresse serv */
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);
  serv_addr.sin_port = htons(portno);/* définition du port */

  if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0){
    error("impossible de se connecter au serveur");
  }

  /* Dialog */
  char buf[CHUNK];
  FILE *file;
  size_t nread;
  printf("Saisissez le nom du fichier: ");
  bzero(buffer,256);
  scanf("%s", buffer );

  if (write(sockfd,buffer,strlen(buffer)) < 0){
    error("erreur : pendant la lecture depuis le socket");
  }

  if (read(sockfd,buffer,255) < 0){
    error("ERROR reading from socket");
  }

  file = fopen(buffer, "r");
  if (file) {
    while ((nread = fread(buf, 1, sizeof buf, file)) > 0){
      fwrite(buf, 1, nread, stdout);
      if (write(sockfd, buf, nread) != nread) {
        puts("impossible d'envoyer le fichier");
        fclose(file);
        exit(1);
      }
    }
    fclose(file);
  }
  close(sockfd);
  return 0;
}
