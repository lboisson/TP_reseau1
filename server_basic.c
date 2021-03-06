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


#define SUCCESS 0
#define ERROR 1
#define SERVER_PORT 1500
#define MAX_MSG 100

#define END_LINE 0x0


//Declaration de la fonction read_line
int read_line (int newSd,char * line_to_return);


//////////
// MAIN //
//////////

int main(int argc,char *argv[])
{

    int sd,newSd;
    socklen_t cliLen;

    struct sockaddr_in cliAddr, servAddr;
    char line[MAX_MSG];

    //Creaction de la socket
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

    while (1)
    {
        printf("%s : waiting for data on port TCP %u \n",argv[0],SERVER_PORT);
        cliLen = sizeof(cliAddr);
	//Acceptation de la connexion
        newSd = accept(sd,(struct sockaddr *) &cliAddr,&cliLen);
        if(newSd<0)
        {
            perror("Cannot accept connection");
            exit(ERROR);
        }

        //Raz de la chaine de caracteres
        memset(line,0x0,MAX_MSG);

        //Reception des segments
        while(read_line(newSd,line)!= ERROR)
        {
            printf("%s: received from %s:TCP%d : %s \n",argv[0],inet_ntoa(cliAddr.sin_addr),ntohs(cliAddr.sin_port),line);
            //Raz de la chaine de caracteres
            memset(line,0x0,MAX_MSG);
        }

    }

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
                printf("Connection closed by client \n");
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
