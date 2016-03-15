#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h> //pour l'initialisation du random


#define SUCCESS 0
#define ERROR 1
#define SERVER_PORT 1500
#define MAX_MSG 100


int main(int argc,char *argv[])
{

    int sd,newSd,i,rc;
    socklen_t cliLen;

    struct sockaddr_in cliAddr, servAddr;

    int  nombre_int;
    char nombre_string[10];

    //Initialisation du random
    srand( time(NULL));

    //Creation de la socket
    sd = socket(AF_INET,SOCK_STREAM,0);
    if(sd<0)
    {
        perror("Cannot open socket");
        exit(ERROR);
    }

    //Initialisation des champs de la structure servAddr
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons (SERVER_PORT);

    //Bind
    if(bind(sd,(struct sockaddr *) &servAddr, sizeof(servAddr))<0)
    {
        perror("Cannot bind port");
        exit(ERROR);
    }

    //Listen
    listen(sd,5);
    printf("Attente de connexion...\n");

    cliLen = sizeof(cliAddr);
    newSd = accept(sd,(struct sockaddr *) &cliAddr,&cliLen);
    if(newSd<0)
    {
        perror("Cannot accept connection");
        exit(ERROR);
    }
    printf("--> Connexion du client établie\n");

    //Envoi des 10 nombres aléatoires compris entre 0 et 49
    for(i=0; i < 10; i++)
    {
        //Génération d'un nombre aléatoire compris entre 0 et 49
        //random = a + (b-a)*((float)(rand()) / (float)(RAND_MAX));
        nombre_int =  rand() % 49;
        
        //Convertion du nombre en chaîne de caractère
        sprintf(nombre_string, "%d", nombre_int);
        //Note : On n'utilisera pas itoa car non standard

        rc = send(newSd, nombre_string, strlen(nombre_string)+1, 0); // +1 pour le \0

        if(rc < 0)
        {
            printf("Cannot send data");
            close(sd);
            exit(ERROR);
        }

        printf("%s : data %u send (%s)\n", argv[0], i, nombre_string);
    }

    close(newSd);

    return(SUCCESS);
}
