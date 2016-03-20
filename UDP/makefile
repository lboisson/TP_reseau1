
client : appliclient.c serveur
	gcc -Wall appliclient.c -o client

serveur : appliserveur.c 
	gcc -Wall appliserveur.c -o serveur



all: 	client
	@echo  "\n--> Executables client et serveur générés\n"

clean :
	@echo  "\n--> Suppression des executables"
	rm -f client
	rm -f serveur
	@echo  "[OK]\n"
