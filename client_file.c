#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define SUCCESS 0
#define ERROR 1
#define SERVER_PORT 1500
#define MAX_MSG 100

int main(int argc, char * argv[])
{

    int sd, rc;
    struct sockaddr_in localAddr, servAddr;
    struct hostent * h;

    FILE *file; //Fichier à envoyer

    if(argc != 3)
    {
        printf("Usage : %s <server> <file ref>\n", argv[0]);
        exit(ERROR);
    }

    h = gethostbyname(argv[1]);
    if(h == NULL)
    {
        printf("%s : Uncknow host '%s'", argv[0], argv[1]);
        exit(ERROR);
    }

    //Initialisation des champs de la structure servAddr
    servAddr.sin_family = h->h_addrtype;
    memcpy((char *)&servAddr.sin_addr.s_addr, h->h_addr_list[0], h->h_length);
    servAddr.sin_port = htons(SERVER_PORT);

    //Creation de la socket
    sd = socket(AF_INET, SOCK_STREAM, 0);
    if(sd < 0)
    {
        perror("Cannot open socket : ");
        exit(ERROR);
    }

    //Initialisation des champs de la structure servAddr
    localAddr.sin_family = AF_INET; //internet
    localAddr.sin_addr.s_addr = htonl(INADDR_ANY); //tout le monde peut se connecter au client
    localAddr.sin_port = htons(0); //port d'écoute du client (0 : numéro de port logiciel attribué par le SE)

    //Bind
    rc = bind(sd, (struct sockaddr *) &localAddr, sizeof(localAddr));
    if(rc < 0)
    {
        printf("%s : cannot bind port TCP %u\n", argv[0], SERVER_PORT);
        perror("Error : ");
        exit(ERROR);
    }

    //Connexion au serveur
    rc = connect(sd, (struct sockaddr *) &servAddr, sizeof(servAddr));
    if(rc < 0)
    {
        perror("Cannot connect : ");
        exit(ERROR);
    }

    /* Envoi des données */

    //On ouvre le fichier à envoyer
    file = NULL;
    file = fopen ( argv[2], "r" );

    if( file != NULL )
    {
        char line[MAX_MSG-1];

        while ( fgets ( line, sizeof line, file ) != NULL )
        {
            rc = send(sd, line, strlen(line)+1, 0);

            if(rc < 0)
            {
                printf("Cannot send data : ");
                close(sd);
                exit(ERROR);
            }
        }

        fclose ( file );
    }
    else
    {
        perror("Cannot open file : ");
        exit(ERROR);
    }

    return(SUCCESS);
}
