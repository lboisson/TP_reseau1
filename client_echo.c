#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define SUCCESS 0
#define ERROR 1
#define SERVER_PORT 1500
#define MAX_MSG 100

int main(int argc, char * argv[])
{

    int sd, rc;
    static char rcv_msg[MAX_MSG];

    struct sockaddr_in localAddr, servAddr;
    struct hostent * h;

    struct timeval tv_envoi, tv_retour;
    struct timezone tz;
    long long diff;

    //Verification du nombre d'arguments
    if(argc != 2)
    {
        printf("Usage : %s <server>\n", argv[0]);
        exit(ERROR);
    }

    //Recupere la structure de type hostent pour l'hote passe en param.
    h = gethostbyname(argv[1]);
    if(h == NULL)
    {
        printf("%s : Uncknow host '%s'", argv[0], argv[1]);
        exit(ERROR);
    }

    //Initialisation des champs de la structure srvAddr
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

    //Bind
    localAddr.sin_family = AF_INET;
    localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    localAddr.sin_port = htons(0);

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

    //Recuperation du temps pre-envoi du message "echo"
    gettimeofday(&tv_envoi, &tz);

    //Envoi du message "echo"
    rc = send(sd, "echo", strlen("echo")+1, 0); // +1 pour le \0
    if(rc < 0)
    {
        printf("Cannot send data : ");
        close(sd);
        exit(ERROR);
    }

    printf("%s : data send (%s)\n", argv[0], "echo");

    //Reception du message retourne par le serveur
    rc = recv(sd,rcv_msg,MAX_MSG,0);
    if(rc < 0)
    {
        printf("Cannot receive data : ");
        close(sd);s
        exit(ERROR);
    }

    //Recuperation du temps post-reception du retour
    gettimeofday(&tv_retour, &tz);

    printf("%s : data received (%s)\n", argv[0], rcv_msg);

     //Calcul de la difference entre les deux temps
    diff=(tv_retour.tv_sec-tv_envoi.tv_sec) * 1000000L + (tv_retour.tv_usec-tv_envoi.tv_usec);

    printf("Duree A/R : %llu usec\n",diff);

    return(SUCCESS);
}
