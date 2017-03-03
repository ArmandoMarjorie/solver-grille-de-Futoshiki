#ifndef DEF_RESOLUTION
#define DEF_RESOLUTION
	
	#include <stdio.h>
	#include <stdlib.h>
	#include "CSP.h"
	#include <time.h>

	typedef enum{none,alea,heuF,dom_deg}heuristique;
	typedef struct
	{
		int* pile; 
		int *valeur;
		int numCase;
		int sommet;
		double nbNoeud;
		double nbContrainte;
		double temps;
	}ALGO;

		//Fonction pour la pile
	/*empile la dernière case*/
	void empiler(ALGO *a);
	/*depile la dernière cellule
		-renvoie -1 si la pile est vide
		-renvoie le numéro de la case sinon */
	int depiler(ALGO *a);
	/*rechercher si la cellule est dans la pile
		-renvoie 1 si elle est dans la pile
		-renvoie 0 sinon */
	int rechercherPile(ALGO *a,int numero);
	
		/* constructeur destructeur ALGO*/
	/*Constructeur pour algo 
		-Renvoie Algo si succès 
		-Sinon le programme est fermé */
	ALGO* initALGO(CSP* csp);
	/*Desalloue a et le met à 0*/
	void libererAlgo(ALGO *a);
		//Fonction commune au resolution
	/*Choisi une valeur pour la cellule*/
	int choixValeur(CSP *csp,ALGO *a);
	/*Test toutes les contraintes associées à la cellule courante et à une cellule déjà affectée
		-Renvoie -1 si toutes les contrainte ne sont pas respectées
		-Renvoie 1 sinon */
	int contrainte(CSP *csp,ALGO* a);
	/*Test si une relation naire est possible et si oui regarde si elle est respectée
		-Renvoie 0 si au moins une cellule (sauf la cellule courante) de la relation n'a pas été instanciée
		-Renvoie -1 si la relation n'est pas respectée
		-Renvoie 1 si elle est respectée*/
	int testRelationN(ALGO *a,CSP *csp,int numeroRelation,int caseCourante);

		//BACKTRACK
	/*Execute l'algorithme de Backtrack
		-Renvoie -1 si pas de solution
		-Renvoie 1 sinon */	
	int backtrack(CSP *csp,heuristique h);

		//FORWARD-CHECKING
	/*Execute l'algorithme de Forward-Checking
		-Renvoie -1 si pas de solution
		-Renvoie 1 sinon */	
	int FC(CSP *csp,heuristique h);
	/*Cette fonction réduit les domaines des cellule non-instanciées
	-	-Renvoie -1 si un domaine devient vide
		-Renvoie 1 sinon */
	int reduireDomaine(ALGO *a,CSP *csp);
	/*Cette fonction restaure le domaine des cellule non instanciées*/
	void restaurerDomaine(ALGO *a,CSP *csp);

		//RESULTAT
	/*Affiche le resultat sous forme de grille de Futoshiki*/
	void afficheResultat(CSP* csp,ALGO *a);

		// HEURISTIQUE
	/*CHoisi un heuristique et renvoie son résultat*/
	int choixHeuristique(heuristique h,CSP *csp,ALGO* a);
	/*Heuristique none
		-Renvoie -1 si il n'y a plus de variable possible
		-Renvoie 0 si on est au début de la pile
		-Renvoie cellule courante +1 */
	int heuristiqueNone(CSP *csp,ALGO* a);
	/*Heuristique dom/deg
		-Renvoie -1 si il n'y as pas de possibilité
		-Renvoie la meilleur variable en fonction domaine/nombre contrainte*/
	int heuristiqueDom_deg(CSP *csp,ALGO* a);
	/*Heuristique Futoshiki
		-Renvoie -1 si il n'y as pas de possibilité
		-Renvoie la variable au plus petit domaine*/
	int heuristiqueFutoshiki(CSP *csp,ALGO* a);
	/*Heuristique aleatoire
		-Renvoie -1 si il n'y as pas de possibilité
		-Renvoie une variable aléatoire sinon*/
	int heuristiqueAleatoire(CSP *csp,ALGO* a);

	/*Calcul et renvoie le nombre de contrainte d'une cellule*/
	int nombreContrainte(CSP *csp,int cellule);
#endif