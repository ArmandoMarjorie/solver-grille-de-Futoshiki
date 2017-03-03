#include "resolution.h"

//Fonction pour la pile

/*empile la dernière case*/
void empiler(ALGO *a)
{
	a->sommet ++;
	a->pile[a->sommet] = a->numCase;
}

/*depile la dernière cellule
	-renvoie -1 si la pile est vide
	-renvoie le numéro de la case sinon */
int depiler(ALGO *a)
{
	a->sommet --;
	if(a->sommet < -1)
		return -1;
	return a->pile[a->sommet +1];
}

/*rechercher si la cellule est dans la pile
	-renvoie 1 si elle est dans la pile
	-renvoie 0 sinon */
int rechercherPile(ALGO *a,int numero)
{
	int i;
	for(i=0;i<a->sommet+1;i++)
	{
		if(a->pile[i] == numero)
		{
			return 1;
		}
	}
	return 0;
}

/* constructeur destructeur ALGO*/
/*Constructeur pour algo 
	-Renvoie Algo si succès 
	-Sinon le programme est fermé */
ALGO* initALGO(CSP* csp)
{
	ALGO *a = NULL;
	a = malloc(sizeof(ALGO)); 
	if(a == NULL)
	{
		fprintf(stderr,"Erreur malloc fonction %s",__func__);
		exit(EXIT_FAILURE);
	}
	a->pile = NULL;
	a->valeur = NULL;
	a->pile = malloc(sizeof(int) * csp->nbCases);
	if(a->pile == NULL)
	{
		fprintf(stderr,"Erreur malloc fonction %s",__func__);
		exit(EXIT_FAILURE);
	}
	a->valeur = malloc(sizeof(int) * csp->nbCases);
	if(a->valeur == NULL)
	{
		fprintf(stderr,"Erreur malloc fonction %s",__func__);
		exit(EXIT_FAILURE);
	}
	a->sommet = -1;
	a->numCase = 0;
	a->nbNoeud = 0;
	a->nbContrainte = 0;
	return a;
}

/*Desalloue a et le met à 0*/
void libererAlgo(ALGO *a)
{
	free(a->valeur);
	free(a->pile);
	free(a);
	a=NULL;
}

//Fonction commune au resolution
/*Choisi une valeur pour la cellule*/
int choixValeur(CSP *csp,ALGO *a)
{
	int i;
	for(i=a->valeur[a->numCase]+1;i<csp->taille+1;i++)
	{
		if(csp->domaine[a->numCase][i] == -1)
		{
			a->nbNoeud++;
			return i;
		}
	}
	return -1;
}
/*Test toutes les contraintes associées à la cellule courante et à une cellule déjà affectée
	-Renvoie -1 si toutes les contrainte ne sont pas respectées
	-Renvoie 1 sinon */
int contrainte(CSP *csp,ALGO* a)
{
	int i,ancienneCase;
	for(i=0;i<a->sommet +1;i++)
	{
		ancienneCase = a->pile[i];
		switch(csp->relationBinaire[a->numCase][ancienneCase])
		{
			case DIFFERENT:
				if(a->valeur[a->numCase] == a->valeur[ancienneCase])
				{
					a->nbContrainte++;
					return -1;
				}
				break;
			case SUPERIEUR:
				if(a->valeur[a->numCase] <= a->valeur[ancienneCase])
				{
					a->nbContrainte++;
					return -1;
				}
				break;
			case INFERIEUR:
				if(a->valeur[a->numCase] >= a->valeur[ancienneCase])
				{
					a->nbContrainte++;
					return -1;
				}
				break;
			default:
				break;
		}
	}
	if(csp->aire == NAIRE) // si le csp est naire on teste aussi les variables naire
	{
		for(i=0;i<2*csp->taille;i++) //on parcours le tableau de relation naire
		{
			if(csp->relationNaire[i][a->numCase] == 1) // si la cellule instanciée est dans la relation
			{
				if(testRelationN(a,csp,i,a->numCase)== -1) //on regarde si on peut faire la relation naire ( toutes les autres cellules instanciées)
					return -1;
			}
		}
	}
	return 1;
}

/*Test si une relation naire est possible et si oui regarde si elle est respectée
	-Renvoie 0 si au moins une cellule (sauf la cellule courante) de la relation n'a pas été instanciée
	-Renvoie -1 si la relation n'est pas respectée
	-Renvoie 1 si elle est respectée*/
int testRelationN(ALGO *a,CSP *csp,int numeroRelation,int caseCourante)
{
	int i,j;
	for(j=0;j<csp->nbCases;j++) //on parcours toutes les cases 
	{
		if(j==caseCourante) //si cellule courante
			continue;
		if(csp->relationNaire[numeroRelation][j] == 1) // la cellule j est aussi dans la relation 
		{
			if(rechercherPile(a,j) == 0) //on regarde si la cellule j à déja été instanciée
				return 0;
		}
	}
	a->nbContrainte++; 
	for(j=0;j<csp->nbCases;j++) // on fait la contrainte de différence
	{
		if(csp->relationNaire[numeroRelation][j] == 1)
		{
			for(i=j+1;i<csp->nbCases;i++)
			{
				if(csp->relationNaire[numeroRelation][i] == 1)
				{
					if(a->valeur[j] == a->valeur[i])
						return -1;
				}
			}
		}
	}
	return 1;
}

//BACKTRACK
/*Execute l'algorithme de Backtrack
	-Renvoie -1 si pas de solution
	-Renvoie 1 sinon */	
int backtrack(CSP *csp,heuristique h)
{
	ALGO *a = NULL;
	a = initALGO(csp);
	time_t depart,arrive;
	time(&depart);  //début du chrono 
	a->numCase = choixHeuristique(h,csp,a); //choix de la cellule courante 
	a->valeur[a->numCase] = 0; //valeur de la cellule courante à 0
	while(a->numCase != -1) //tant qu'il y a une case à instanciée
	{
		a->valeur[a->numCase] = choixValeur(csp,a); // la cellule courante prend une valeur 
		while(!(a->valeur[a->numCase] == -1 || contrainte(csp,a) == 1)) // tant que la on à une valeur possible et que les contrainte ne sont pas respectées
		{
			a->valeur[a->numCase] = choixValeur(csp,a); //on change de valeur
		}
		if(a->valeur[a->numCase] == -1) // si plus de valeur possible 
		{
			if(a->sommet == -1) // si la pile est vide on n'a pas de solution
			{
				libererAlgo(a);
				return -1;
			}
			a->numCase = depiler(a);//sinon on dépile l'ancienne cellule
		}
		else
		{
			empiler(a);
			a->numCase = choixHeuristique(h,csp,a);//on choisit une autre cellule
			if(a->numCase != -1) //si on a une cellule possible
				a->valeur[a->numCase] = 0; // on la met à 0 
		}
	}
	time(&arrive);//fin du chrono
	a->temps = difftime(arrive,depart);
	afficheResultat(csp,a);//on affiche le resultat 
	libererAlgo(a);
	return 1;
}

//FORWARD-CHECKING
/*Execute l'algorithme de Forward-Checking
	-Renvoie -1 si pas de solution
	-Renvoie 1 sinon */	
int FC(CSP *csp,heuristique h)
{
	ALGO *a = NULL;
	a = initALGO(csp);
	time_t depart,arrive;
	time(&depart);
	a->numCase = choixHeuristique(h,csp,a);
	a->valeur[a->numCase] = 0;
	while(a->numCase != -1)//tant qu'il y a une case à instanciée
	{
		a->valeur[a->numCase] = choixValeur(csp,a);
		while(!(a->valeur[a->numCase] == -1 || contrainte(csp,a) == 1))// tant que la on à une valeur possible et que les contrainte ne sont pas respectées
		{
			a->valeur[a->numCase] = choixValeur(csp,a);
		}
		if(a->valeur[a->numCase] == -1)// si plus de valeur possible 
		{
			if(a->sommet == -1)
			{
				libererAlgo(a);
				return -1;
			}
			a->numCase = depiler(a);
			restaurerDomaine(a,csp);
		}
		else
		{
			if(reduireDomaine(a,csp) == -1)
				restaurerDomaine(a,csp);
			else
			{
				empiler(a);
				a->numCase = choixHeuristique(h,csp,a);
				if(a->numCase != -1)
					a->valeur[a->numCase] = 0;
			}
		}
	}
	time(&arrive);
	a->temps = difftime(arrive,depart);
	afficheResultat(csp,a);
	libererAlgo(a);
	return 1;
}
/*Cette fonction réduit les domaines des cellule non-instanciées
-	-Renvoie -1 si un domaine devient vide
	-Renvoie 1 sinon */
int reduireDomaine(ALGO *a,CSP *csp)
{
	int i,j;
	for(i=0;i<csp->nbCases;i++) //on  parcours les cases 
	{
		if(rechercherPile(a,i)== 1) //si la case est dans la pile
			continue;
		if(csp->relationBinaire[a->numCase][i] == DIFFERENT && csp->domaine[i][a->valeur[a->numCase]] == -1) //si on à une relation de différence et la valeur de cellule courante disponible
		{
			csp->domaine[i][a->valeur[a->numCase]] = a->numCase+1; //on enleve la valeur de la cellule courante du domaine de la variable i
			csp->domaine[i][0]--; //on reduit le domaine
			if(csp->domaine[i][0] == 0)
				return -1;
		}
		else if(csp->relationBinaire[a->numCase][i] == SUPERIEUR) // si on a une relation de supériorité
		{
			for(j=a->valeur[a->numCase];j<csp->nbCases;j++)
			{
				if(csp->domaine[i][a->valeur[a->numCase]] == -1)
				{
					csp->domaine[i][j] = a->numCase+1;
					csp->domaine[i][0]--;
						if(csp->domaine[i][0] == 0)
							return -1;
				}
			}
		}
		else if(csp->relationBinaire[a->numCase][i] == INFERIEUR)//si on a une relation infériorité
		{
			for(j=1;j<a->valeur[i];j++)
			{
				if(csp->domaine[i][a->valeur[a->numCase]] == -1)
				{
					csp->domaine[i][j] = a->numCase+1;
					csp->domaine[i][0]--;
					if(csp->domaine[i][0] == 0)
						return -1;
				}
			}
		}
	}
	return 1;
}

/*Cette fonction restaure le domaine des cellule non instanciées*/
void restaurerDomaine(ALGO *a,CSP *csp)
{
	int i,j;
	for(i=0;i<csp->nbCases;i++)
	{
		if(rechercherPile(a,i)== 1)
			continue;
		if(csp->relationBinaire[a->numCase][i] == DIFFERENT && csp->domaine[i][a->valeur[a->numCase]] == a->numCase+1)
		{
			csp->domaine[i][a->valeur[a->numCase]] = -1;
			csp->domaine[i][0]++;
		}
		else if(csp->relationBinaire[a->numCase][i] == SUPERIEUR)
		{
			for(j=a->valeur[a->numCase];j<csp->nbCases;j++)
			{
				if(csp->domaine[i][a->valeur[a->numCase]] == a->numCase+1)
				{
					csp->domaine[i][j] = -1;
					csp->domaine[i][0]++;
				}
			}
		}
		else if(csp->relationBinaire[a->numCase][i] == INFERIEUR)
		{
			for(j=1;j<a->valeur[i];j++)
			{
				if(csp->domaine[i][a->valeur[a->numCase]] == a->numCase+1)
				{
					csp->domaine[i][j] = -1;
					csp->domaine[i][0]++;
				}
			}
		}
	}
}

//RESULTAT
/*Affiche le resultat sous forme de grille de Futoshiki*/
void afficheResultat(CSP* csp,ALGO *a)
{
	int i,j;
	printf("Nombre de noeud :%.0f\n",a->nbNoeud);
	printf("Nombre de contrainte testee :%.0f\n",a->nbContrainte);
	printf("Resolution en %f\n",a->temps);
	for(i=0;i<csp->taille;i++)
	{
		for(j=0;j<csp->taille;j++)
		{
			printf("%2d",a->valeur[numeroCellule(i,j,csp->taille)]);
			if(j < csp->taille-1)
			{
				if(csp->relationBinaire[numeroCellule(i,j,csp->taille)][numeroCellule(i,j+1,csp->taille)] == INFERIEUR)
					printf("<");
				else if(csp->relationBinaire[numeroCellule(i,j,csp->taille)][numeroCellule(i,j+1,csp->taille)] == SUPERIEUR)
					printf(">");
				else
					printf(" ");
			}
		}
		printf("\n");
		if(i+1 < csp->taille)
		{
			for(j=0;j<csp->taille;j++)
			{
				if(csp->relationBinaire[numeroCellule(i,j,csp->taille)][numeroCellule(i+1,j,csp->taille)] == SUPERIEUR)
					printf(" V ");
				else if(csp->relationBinaire[numeroCellule(i,j,csp->taille)][numeroCellule(i+1,j,csp->taille)] == INFERIEUR)
					printf(" ^ ");
				else
					printf("   ");
			}
		}
		printf("\n");
	}
	printf("\n");
}


// HEURISTIQUE
/*CHoisi un heuristique et renvoie son résultat*/
int choixHeuristique(heuristique h,CSP *csp,ALGO* a)
{
	switch(h)
	{
		case none:
			return heuristiqueNone(csp,a);
		break;
		case alea:
			return heuristiqueAleatoire(csp,a);
		break;
		case heuF:
			return heuristiqueFutoshiki(csp,a);
		break;
		case dom_deg:
			return heuristiqueDom_deg(csp,a);
		default:
			break;
	}
	return -1;
}

/*Heuristique none
	-Renvoie -1 si il n'y a plus de variable possible
	-Renvoie 0 si on est au début de la pile
	-Renvoie cellule courante +1 */
int heuristiqueNone(CSP *csp,ALGO* a)
{
	int result;
	if(a->sommet == -1) // cas du début
		return 0;
	result = a->pile[a->sommet] +1;
	if(a->sommet == csp->nbCases-1) //plus de variables possible
		return -1;
	else
		return result; // return ancienne variable +1;
}

/*Heuristique dom/deg
	-Renvoie -1 si il n'y as pas de possibilité
	-Renvoie la meilleur variable en fonction domaine/nombre contrainte*/
int heuristiqueDom_deg(CSP *csp,ALGO* a)
{
	int imin=csp->nbCases+1,i;
	if(a->sommet == csp->nbCases-1) //plus de variables possible
		return -1;
	for(i=0;i<csp->nbCases;i++)
	{
		if(csp->domaine[i][0]/nombreContrainte(csp,i) < imin && rechercherPile(a,i)==0)
			imin = i;
	}
	return imin;
}

/*Heuristique Futoshiki
	-Renvoie -1 si il n'y as pas de possibilité
	-Renvoie la variable au plus petit domaine*/
int heuristiqueFutoshiki(CSP *csp,ALGO* a)
{
	int imin=csp->nbCases+1,i;
	if(a->sommet == csp->nbCases-1) //plus de variables possible
		return -1;
	for(i=0;i<csp->nbCases;i++)
	{
		if(csp->domaine[i][0] == 1 && rechercherPile(a,i)==0)
			return i;
		if(csp->domaine[i][0] < imin && rechercherPile(a,i)==0)
			imin = i;
	}
	return imin;
}

/*Heuristique aleatoire
	-Renvoie -1 si il n'y as pas de possibilité
	-Renvoie une variable aléatoire sinon*/
int heuristiqueAleatoire(CSP *csp,ALGO* a)
{
	int result,existe=1;
	if(a->sommet  == csp->nbCases-1)
		return -1;
	while(existe)
	{
		result = rand()%csp->nbCases;
		existe = rechercherPile(a,result);
	}
	return result;
}

/*Calcul et renvoie le nombre de contrainte d'une cellule*/
int nombreContrainte(CSP *csp,int cellule)
{
	int nbContrainte=0;
	int i;
	for(i = 0;i<csp->nbCases;i++)
	{
		if(csp->relationBinaire[cellule][i] != NONE)
			nbContrainte++;
	}
	if(csp->aire == NAIRE)
	{
		for(i=0;i<csp->taille *2;i++)
		{
			if(csp->relationNaire[i][cellule] == 1)
				nbContrainte++;
		}
	}
	return nbContrainte;
}