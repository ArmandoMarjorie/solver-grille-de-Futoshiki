#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "resolution.h"
#include "CSP.h"

void usage();
/*Explique comment utiliser le programme*/
void erreur(char* message);
/*affiche un message d'erreur et quitte le programme*/
int veriferAlgo(char *choixAlgo);
/*verifie si algo donné en argument est compatible
	-Si oui retourne 1
	-Si non retourne 0*/
int verifierHeuristique(char *choixHeuristique,heuristique *h);
/*verifie si heuristique donné en argument est compatible
	-Si oui retourne 1 et l'heuristique h prend la valeur du choix
	-Si non retourne 0*/

	
/* FUTOSHIKI !
Mode d'emploi du programme : 
Un makefile est disponible pour la compilation. Pour exécuter le programme, tapez : 
./Futoshiki grilles/Nom de la grille.fut {--bBT, --bFC, --nBT, --nFC} {--none, --alea, --heuF, --dom_deg} 
 
Pour plus d'informations sur la commande, tapez :
./Futoshiki -h
*/
int main(int argc, char* args[])
{
	FILE* f=NULL;
	CSP *csp = NULL;
	int taille,solution;
	heuristique h;
	srand(time(NULL));
	if(argc-1 >= 1 && !strcmp(args[1],"-h")) // si l'utilisateur a tapé Futoshiki -h
	{
		usage(); // on donne le mode d'emploi 
		return  EXIT_SUCCESS; // on quitte le programme
	}
	if(argc-1 != 3) // si il manque un argument 
		erreur("Pas le bon nombre d'argument \nTaper Futoshiki -h pour plus d'information\n");
	
	if(!veriferAlgo(args[2])) // si le choix de la méthode de résolution n'existe pas
		erreur("Choix d'algo inconnu inconnu\nTaper Futoshiki -h pour plus d'information\n");
	
	if(!verifierHeuristique(args[3],&h)) //si le choix de l'heuristique n'existe pas
		erreur("Choix de l'heuristique inconnu\nTaper Futoshiki -h pour plus d'information\n");

	f=fopen(args[1],"r"); // on ouvre le fichier contenant la grille
	if(f==NULL) // si echec de l'ouverture 
		erreur("Erreur fichier inexistant \n");
	if(fscanf(f,"%d",&taille) == 0) // si le premier argument n'est pas la taille 
		erreur("Fichier incorrect \n");
		
	if(args[2][2] == 'n') // si on est dans une méthode naire
	{
		csp = creerCSP(taille,f,NAIRE);
		printf("Creation csp naire: ok\n");	
	}
	else if(args[2][2] == 'b')// si on est dans une méthode binaire
	{
		csp = creerCSP(taille,f,BINAIRE);
		printf("Creation csp binaire: ok\n");
	}
	if(args[2][3] == 'B') // si on a choisi Backtrack
	{
		solution = backtrack(csp,h);
		if(solution == -1) // si pas de solution
			printf("Pas de solution\n");
	}
	else if(args[2][3] == 'F') // si on a choisi Forward-Checking
	{
		solution = FC(csp,h);
		if(solution == -1) //si pas de solution
			printf("Pas de solution\n");
	}
	libererCSP(csp);
	return 0;
}

/*Explique comment utiliser le programme*/
void usage()
{
	printf("NOM \n");
	printf("\tFutoshiki - Resoudre des grilles de Futoshiki\n\n");
	printf("SYNOPSIS\n");
	printf("\tFutoshiki [-h] fichier {--bBT, --nBT, --bFC, --nFC} {--none, --alea, --heuF, --dom_deg}\n\n");
	printf("DESCRIPTION \n");
	printf("\tLa commande Futoshiki resout une grille de Futoshiki passe en argument\n\n");
	printf("OPTIONS\n");
	printf("\t -h\n\t\tAffiche un message d'aide sur la sortie standard et termine normalement \n\n");
	printf("\t --bBT\n\t\tResout en utilisant l'algorithme Backtrack binaire\n");
	printf("\t --nBT\n\t\tResout en utilisant l'algorithme Backtrack naire\n");
	printf("\t --bFC\n\t\tResout en utilisant l'algorithme Forward Checking binaire\n");
	printf("\t --nFC\n\t\tResout en utilisant l'algorithme Forward Checking naire\n\n");
	printf("\t --none\n\t\tResout en utilisant l'heuristique none (pas vraiment d'heuristique) \n");
	printf("\t --alea\n\t\tResout en utilisant une heuristique aléatoire\n");
	printf("\t --heuF\n\t\tResout en utilisant une heuristique pour le Futoshiki\n");
	printf("\t --dom_deg\n\t\tResout en utilisant une heuristique dom/deg\n");
}

/*affiche un message d'erreur et quitte le programme*/
void erreur(char* message)
{
	fprintf(stderr,"%s",message); 
		exit(EXIT_FAILURE);
}

/*verifie si algo donné en argument est compatible
	-Si oui retourne 1
	-Si non retourne 0*/
int veriferAlgo(char *choixAlgo)
{
	if(!strcmp(choixAlgo,"--nFC")) // FC naire
		return 1;
	if(!strcmp(choixAlgo,"--bFC")) //FC binaire
		return 1;
	if(!strcmp(choixAlgo,"--bBT")) //BT binaire
		return 1;
	if(!strcmp(choixAlgo,"--nBT"))//BT naire
		return 1;
	
	return 0;
}

/*verifie si heuristique donné en argument est compatible
	-Si oui retourne 1 et l'heuristique h prend la valeur du choix
	-Si non retourne 0*/
int verifierHeuristique(char *choixHeuristique, heuristique *h)
{
	if(!strcmp(choixHeuristique,"--none")) // pas d'heuristique
	{
		*h = none;
		return 1;
	}
	if(!strcmp(choixHeuristique,"--alea"))//heuristique aleatoire
	{
		*h = alea;
		return 1;
	}
	if(!strcmp(choixHeuristique,"--heuF"))//heuristique pour foward-checking
	{
		*h = heuF;
		return 1;
	}
	if(!strcmp(choixHeuristique,"--dom_deg"))//heuristique dom/deg
	{
		*h = dom_deg;
		return 1;
	}
	return 0;
}