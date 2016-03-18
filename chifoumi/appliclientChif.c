#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
 
void menu()
{
	printf("\tChoisissez\n");
	printf("\t1 : Papier\n");
	printf("\t2 : Ciseaux\n");
	printf("\t3 : Pierre\n");
	printf("\t4 : Quitter\n");
}

void aide()
{
	printf("Entrez 1, 2, 3 pour jouer, 4 pour quitter !\n");
	printf("Feuille > Pierre & Pierre > Ciseau & Ciseau > Feuille\n");
	menu();
}

void gagnant(int leClient,int leServeur, int *scoreClient, int *scoreServeur)
{
	const char *tirage[3]={"Papier","Ciseau","Pierre"};
	printf ("Le client a tire %s , ",tirage[leServeur-1]);
	switch (leClient)
	{
		case 1:
			switch (leServeur)
			{
				case 1:
					printf("Match Nul !\n");
					break;
				case 2:
					printf("Vous perdez !\n");
					*scoreServeur += 1 ;
					break;
				case 3:
					printf("Vous gagnez !\n");
					*scoreClient += 1 ;
					break;
				default:
					printf("ERROR !\n");
					exit(0);
			};
			break;
		case 2:
			switch (leServeur)
			{
				case 1:
					printf("Vous gagnez !\n");
					*scoreClient += 1 ;
					break;
				case 2:
					printf("Match Nul !\n");
					break;
				case 3:
					printf("Vous perdez !\n");
					*scoreServeur += 1 ;
					break;
				default:
					printf("ERROR !\n");
					exit(0);
			};
			break;
		case 3:
			switch (leServeur)
			{
				case 1:
					printf("Vous perdez !\n");
					*scoreServeur += 1 ;
					break;
				case 2:
					printf("Vous gagnez !\n");
					*scoreClient += 1 ;
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
    printf("Bienvenue dans le jeu Pierre-Feuille-Ciseau\n");
    menu();
 
    int sd, len, res ;
    struct sockaddr_in localAddr;
    int rc, n = 0 ;
    int choix , tmp;
    int scoreClient = 0, scoreServeur = 0 ;
    
    char num[1];
 
    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd == -1) { 
        perror("Socket create failed.\n") ; 
        return -1 ; 
    } 
     
    localAddr.sin_family = AF_INET;
    localAddr.sin_addr.s_addr = inet_addr("127.0.0.1");/*On ne communique qu'avec nous-même, dans cet essai*/
    localAddr.sin_port = htons(7735);
    len = sizeof(localAddr);
 
    rc = connect(sd, (struct sockaddr *)&localAddr, len);
    if(rc == -1)
    {
        perror("Erreur !\n");
        exit(-1);
    }
 
	do
	{
		printf("\tVotre choix : ");
		scanf("%1s",num);		
		fflush(stdin); /*On vide le tampon ; sinon risque de probleme avec scanf*/	
		if (isdigit(num[0]))
		{
			choix= num[0] - '0';	
		}
		else
		{
			printf("Entrez un nombre valide ! \n");
			aide();
			continue;
		}
		if ((choix <= 3) && (choix > 0))
		{
			tmp=choix;
			res = write(sd, &choix, 1);
			if (res == -1) break ; 
			read(sd, &choix, 1);
			gagnant(tmp,choix, &scoreClient, &scoreServeur);
			n++ ;
		}
		else if(choix == 4 || n == 3)
		{
			printf("Fin du jeu.\n");
			break ;
		}
		 else
		{
			aide();
			continue;
		}

	}
    	while (n < 3);
    	
	if(scoreClient > scoreServeur)
    		printf("Vous avez gagne cette manche !\n") ;
    	else if(scoreClient == scoreServeur)
    		printf("MATCH NUL !\n") ;
    	else
    		printf("Le serveur a gagne cette manche !\n") ;
    	
    close(sd);
 
    exit(0);
}
