/* Exemple de client UDP */

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SUCCESS 0
#define ERROR 1
#define MAX_MSG 100

int main(int argc, char* argv[])
{

    int sockfd;
    int port;
    int i;
    struct sockaddr_in servaddr;

    //Verification du nombre d'arguments
    if(argc < 4)
    {
        printf("Usage : %s <server> <port> <data1> <data2> ... <dataN>\n", argv[0]);
        exit(ERROR);
    }

    //Creation de la socket
    sockfd=socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd < 0)
    {
        perror("Cannot open socket : ");
        exit(ERROR);
    }

    //Initialisation des champs de la structure servaddr
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr=inet_addr(argv[1]);
    port = atoi(argv[2]);
    servaddr.sin_port=htons(port);

    for(i=3; i < argc; i++)
    {
        //Envoi du mot
        sendto(sockfd,argv[i],strlen(argv[i]),0, (struct sockaddr *)&servaddr,sizeof(servaddr));


    }

    exit(SUCCESS);

}
