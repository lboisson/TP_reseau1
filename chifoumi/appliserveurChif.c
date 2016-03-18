#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

void gagnant(int leClient,int leServeur)
{
	const char *tirage[3]={"Feuille","Ciseau","Pierre"};
	printf("Client : %s\n",tirage[leClient-1]);
	printf ("Serveur : %s\n",tirage[leServeur-1]);

	switch (leClient)
	{
		case 1:
			switch (leServeur)
			{
				case 1:
					printf("Match Nul !\n");
					break;
				case 2:
					printf("Le Serveur gagne !\n");
					break;
				case 3:
					printf("Le Serveur perd !\n");
					break;
				default:
					printf("ERROR\n");
					exit(0);
			};
			break;
		case 2:
			switch (leServeur)
			{
				case 1:
					printf("Le Serveur perd !\n");
					break;
				case 2:
					printf("Match Nul !\n");
					break;
				case 3:
					printf("Le Serveur gagne !\n");
					break;
				default:
					printf("ERROR\n");
					exit(0);
			};
			break;
		case 3:
			switch (leServeur)
			{
				case 1:
					printf("Le Serveur gagne !\n");
					break;
				case 2:
					printf("Le Serveur perd !\n");
					break;
				case 3:
					printf("Match Nul !\n");
					break;
				default:
					printf("ERROR\n");
					exit(0);
			};
			break;
		default:
			printf("ERROR\n");
			exit(0);
	}
} 
 
int main(int argc, char *argv[])
{
    int sd, newSd;
    int server_len ; 
    int rc = 0, choixServeur, pid ; 
    unsigned client_len;
    struct sockaddr_in cliAddr, servAddr;
 
    sd = socket(AF_INET, SOCK_STREAM, 0);
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htons(INADDR_ANY);
    servAddr.sin_port = htons(7735) ;
    server_len = sizeof(servAddr);
 
    rc = bind(sd, (struct sockaddr *) &servAddr, server_len);
    rc = listen(sd, 5);
    
    printf("Attente de connexion...\n"); 
 
    client_len = sizeof(cliAddr);
    
    while(1)
    {
	    newSd = accept(sd, (struct sockaddr *) &cliAddr, &client_len);
	    
	    if((pid = fork()) == 0)
	    {
	    	printf("--> Connexion du client établie\n");

		int jeuC;
		printf("Début du jeu Pierre-Feuille-Ciseau\n");

		rc = read(newSd, &jeuC, 1);       
		srand(time(NULL));
		choixServeur = (rand() % 3)+1;
		gagnant(jeuC,choixServeur);
		jeuC  = choixServeur;
		write(newSd, &jeuC, 1);

	    }
	    
	    else
	    	printf("Erreur au niveau du fork !\n") ;
    }

 
    printf("Fermeture du serveur... Au revoir !\n"); 
    close(newSd);
    return 0;
}
