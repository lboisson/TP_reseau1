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

#define END_LINE 0x0


//Declaration de la fonction read_line
int read_line (int newSd,char * line_to_return);


int main(int argc, char * argv[])
{

    int sd, rc;
    struct sockaddr_in servAddr;
    struct hostent * h;
    char line[MAX_MSG];

    if(argc != 2)
    {
        printf("Usage : %s <server>\n", argv[0]);
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

    //Connexion au serveur
    rc = connect(sd, (struct sockaddr *) &servAddr, sizeof(servAddr));
    if(rc < 0)
    {
        perror("Cannot connect : ");
        exit(ERROR);
    }

	//Reception des nombres
        while(read_line(sd,line)!= ERROR)
        {
            printf("%s: received from %s:TCP%d : %s \n",argv[0],inet_ntoa(servAddr.sin_addr),ntohs(servAddr.sin_port),line);
	    //Raz de line
            memset(line,0x0,MAX_MSG);
        }

    close(sd);

    return(SUCCESS);
}


/////////////////
// FUNCTION(S) //
/////////////////

int read_line (int newSd,char * line_to_return)
{
    static int rcv_ptr=0;
    static char rcv_msg[MAX_MSG];
    static int n;
    int offset;

    offset=0;

    while(1)
    {
        if(rcv_ptr==0)
        {
            /* read data from socket */
            memset(rcv_msg,0x0,MAX_MSG);/* init buffer */
            n = recv(newSd,rcv_msg,MAX_MSG,0); /* wait for data */
            if(n < 0)
            {
                perror("Cannot receive data ");
                return ERROR;
            }
            else if(n==0)
            {
                printf("Connection closed by serveur \n");
                close(newSd);
                return ERROR;
            }
        }

        /* if new data read on socket */
        /* OR */
        /* if another lise is still in buffer */
        /* copy ligne into 'line_to_return'*/
        while(*(rcv_msg+rcv_ptr)!=END_LINE && rcv_ptr<n)
        {
            memcpy(line_to_return+offset,rcv_msg+rcv_ptr,1);
            offset++;
            rcv_ptr++;
        }

        /* end of line + end of buffer => return line */
        if(rcv_ptr == n-1)
        {
            /* set last by to END_LINE */
            *(line_to_return+offset)=END_LINE;
            rcv_ptr=0;
            return ++offset;
        }


        /* end of line but still some data in buffer => return line */
        if(rcv_ptr < n-1)
        {
            /* set last by to END_LINE */
            *(line_to_return+offset)=END_LINE;
            rcv_ptr++;
            return ++offset;
        }

        /* end of buffer but line is not ended => */
        /* wait for more data to arrive on socket */
        if(rcv_ptr == n)
        {
            rcv_ptr = 0;
        }
    }
}
