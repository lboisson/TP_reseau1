#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define SUCCESS 0
#define ERROR 1

#define END_LINE 0x0
#define SERVER_PORT 1500
#define MAX_MSG 100

/*function readline*/
int read_line(int newSd, char *line_to_return){
	
	static int rcv_ptr=0;
	static char rcv_msg[MAX_MSG];
	static int n;
	int offset;
	
	offset = 0;
	
	while(1){
		if(rcv_ptr == 0){
			
			/*read data from socket	*/
			memset(rcv_msg, 0x0, MAX_MSG);	/*init buffer	*/
			n = recv(newSd, rcv_msg, MAX_MSG, 0);	/*wait for data	*/
			
			if(n<0){
				perror("cannot receive data");
				return ERROR;
			}
			else if (n == 0){
				printf("connection closed by client\n");
				close(newSd);
				return ERROR;
			}
		}
		
		/*	if new data is read on socket	*/
		/*	OR	*/
		/*	if another line is still on buffer	*/
		
		/*copy line into 'line_to_return'	*/
		while(*(rcv_msg+rcv_ptr) != END_LINE && rcv_ptr<n){
			memcpy(line_to_return + offset, rcv_msg+rcv_ptr, 1);
			offset++;
			rcv_ptr++;
		}
		
		/*end of line + end of buffer => return line */
		if(rcv_ptr == n-1){
			/*set last byte to END LINE	*/
			*(line_to_return+offset)=END_LINE;
			rcv_ptr=0;
			return ++offset;
		}
		
		/*end of line but still some data in buffer => return line */
		if(rcv_ptr < n-1){
			/*set last byte to END LINE	*/
			*(line_to_return+offset)=END_LINE;
			rcv_ptr++;
			return ++offset;
		}
		
		/*	end of buffer but line is not ended => */
		/*	wait for more data to arrive on socket	*/
		if(rcv_ptr == n){
			rcv_ptr = 0;
		}
	}	/*while */
}

int main (int argc, char *argv[]){
	int sd, newSd, cliLen;
	struct sockaddr_in cliAddr, servAddr;
	char line[MAX_MSG];
	
	/*create socket	*/
	sd = socket(AF_INET, SOCK_STREAM, 0);
	if(sd<0){
		perror("cannot open socket ");
		return ERROR;
	}
	
	/*bind server port*/
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(SERVER_PORT);
	
	if(bind(sd, (struct sockaddr *) &servAddr, sizeof(servAddr))<0){
		perror("cannot bind port");
		return ERROR;
	}
	
	listen(sd, 5);
	
	while(1){
		printf("%s: waiting for data on port TCP %u\n", argv[0], SERVER_PORT);
		
		cliLen = sizeof(cliAddr);
		newSd = accept(sd, (struct sockaddr *) &cliAddr, &cliLen);
		if(newSd<0){
			perror("cannot accept connection ");
			return ERROR;
		}
		
		/*init line*/
		memset(line, 0x0, MAX_MSG);
		
		/*receive segments*/
		while(read_line(newSd,line)!=ERROR){
			printf("%s: received from %s:TCP%d : %s\n", argv[0], inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port), line);
			
			/*init line*/
			memset(line, 0x0, MAX_MSG);
		}	/*while (real_line)	*/
	}	/* while (1)	*/
}

				
	
			
				
				
				
			
			
			
			
