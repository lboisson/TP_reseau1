/* Exemple de serveur UDP */

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SUCCESS 0
#define ERROR 1
#define MAX_MSG 100

int main(int argc, char** argv)
{

    int sockfd,n;
    int port;
    struct sockaddr_in servaddr,cliaddr;
    socklen_t len;
    char mesg[MAX_MSG];

    //Verification du nombre d'arguments
    if(argc != 2)
    {
        printf("Usage : %s <port>\n", argv[0]);
        exit(ERROR);
    }

    //Creaction de la socket
    sockfd=socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd<0)
    {
        perror("Cannot open socket ");
        exit(ERROR);
    }

    //Initialisation des champs de la structure servaddr
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    port = atoi(argv[1]);
    servaddr.sin_port=htons(port);

    //Bind
    if(bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr)) < 0)
    {
        perror("Can't bind local adresse ");
        return(ERROR);
    }

    for (;;)
    {
        len = sizeof(cliaddr);
	       //Reception des mots
        n = recvfrom(sockfd,mesg,MAX_MSG,0,(struct sockaddr *)&cliaddr,&len);
        mesg[n] = 0;
        printf("Recu :\n");
        printf("%s\n",mesg);

        if (getnameinfo(servaddr, len, mesg, sizeof(mesg),NULL, 0))
               printf("could not resolve hostname");
        else
               printf("host=%s\n", mesg);
    }

    exit(SUCCESS); //code mort
}
