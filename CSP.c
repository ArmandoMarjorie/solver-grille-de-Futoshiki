#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CSP.h"

/*Crée le CSP avec la taille , le fichier de la grille et le type( binaire, naire)
	-Renvoie NULL si erreur
	-Sinon renvoie le csp*/
CSP* creerCSP(int taille,FILE *f,type aire)
{
	CSP *csp=NULL;
	int i,j;
	csp=malloc(sizeof(CSP));
	if(csp==NULL)
		return NULL;
	csp->relationNaire = NULL;
	csp->aire = aire;
	csp->taille=taille;
	csp->nbCases=taille*taille;
	/*Partie allocation dynamique*/
	csp->relationBinaire = malloc(sizeof(symbole*)*csp->nbCases);
	csp->domaine = malloc(sizeof(int *)*csp->nbCases);
	for(i=0;i<csp->nbCases;i++)
	{
		csp->relationBinaire[i] = malloc(sizeof(symbole)*csp->nbCases);
		for(j=0;j<csp->nbCases;j++)
		{
			csp->relationBinaire[i][j] = NONE;
		}
		csp->domaine[i] = malloc(sizeof(int)*(taille+1));	
	}
	if(aire == NAIRE) // exclusif au csp naire 
	{
		csp->relationNaire = malloc(sizeof(int*)*csp->taille*2);
		for(i=0;i<csp->taille*2;i++)
		{
			csp->relationNaire[i] = malloc(sizeof(int)*csp->nbCases);
			for(j=0;j<csp->nbCases;j++)
			{
				csp->relationNaire[i][j] = 0;
			}
		}
		differenceNaire(csp);
	}
	else
		differenceBinaire(csp);
	lireFutoshiki(csp,f);
	return csp;
}

/*Désalloue le csp et met c à null*/
void libererCSP(CSP *c)
{
	int i;
	for(i=0;i<c->nbCases;i++)
	{
		free(c->relationBinaire[i]);
		free(c->domaine[i]);
	}
	free(c->relationBinaire);
	free(c->domaine);
	if(c->aire == NAIRE)
	{
		for(i=0;i<c->taille *2;i++)
			free(c->relationNaire[i]);
		
		free(c->relationNaire);
	}
	free(c);
	c = NULL;
}

/*Lit le Futoshiki*/
void lireFutoshiki(CSP *c,FILE *f)
{
	int i;
	for(i=0;i < c->taille;i++)
	{
		lireLigne(c,f,i); // lit les lignes
	}
}

/*Cette fonction lit les lignes(une ligne chiffre et une ligne symbole si possible)*/
void lireLigne(CSP *c,FILE *f,int ligne)
{
	
	int i;
	//ligne chiffre
	for(i = 0;i< (c->taille)-1 ;i++) // lit toutes les valeurs -1 et les symboles entre
	{
		lireValeur(c,f,(ligne* c->taille) + i);
		lireSymbole(c,f,ligne,i,ligne,i+1);
	}
	lireValeur(c,f,(ligne*c->taille) + i);// lit la dernière valeur
	fscanf(f,"\r\n");
	//ligne symbole
	if(ligne == c->taille -1) // si on est à la dernière ligne la ligne symbole n'existe pas
		return;
	for(i = 0; i< c->taille;i++) // on lit tout les symboles
	{
		lireSymbole(c,f,ligne,i,ligne+1,i);
	}
}

/*lit la valeur et remplit le domaine*/
void lireValeur(CSP *c,FILE *f,int cellule)
{	
	int valeur;
	fscanf(f,"%d",&valeur);
	if(valeur == 0) // si valeur vaut 0
		valeurNonDefinie(c,cellule);
	else
		valeurDefinie(c,cellule,valeur);
}

/*Remplit le domaine si la valeur est définie dans le fichier*/
void valeurDefinie(CSP *c,int cellule,int valeur)
{
	int i;
	c->domaine[cellule][0] = 1; // une seule valeur possible
	for(i = 1 ;i< (c->taille) + 1;i++)
	{
		c->domaine[cellule][i] = 0;// les autres valeurs ne sont pas disponible
	}
	c->domaine[cellule][valeur] = -1;// cette valeur devient disponible
}

/*Remplit le domaine si la valeur n'est pas definie dans le fichier*/
void valeurNonDefinie(CSP *c,int cellule)
{
	int i;
	c->domaine[cellule][0] = c->taille; //toutes les valeurs sont possible 
	for(i = 1;i< (c->taille) +1;i++)
	{
		c->domaine[cellule][i] = -1; //toutes les valeurs deviennent disponibles
	}
}

/*Remplit relation binaire en fonction du symbole*/
void lireSymbole(CSP *c,FILE* f, int ligneX , int colonneX,int ligneY, int colonneY)
{
	char signe;
	int x, y;
	x = numeroCellule(ligneX,colonneX,c->taille); // on calcule la première cellule concernée
	y = numeroCellule(ligneY,colonneY,c->taille); //on calcule la deuxième cellule concerné
	fscanf(f,"%c",&signe);
	switch(signe) //on regarde le signe
	{
		case '>': 
			c->relationBinaire[x][y] = SUPERIEUR;
			c->relationBinaire[y][x] = INFERIEUR;
		break;
		case '<': 
			c->relationBinaire[x][y] = INFERIEUR;
			c->relationBinaire[y][x] = SUPERIEUR;
		break;
		case 'v': 
			c->relationBinaire[x][y] = SUPERIEUR;
			c->relationBinaire[y][x] = INFERIEUR;
		break;
		case '^':
			c->relationBinaire[x][y] = INFERIEUR;
			c->relationBinaire[y][x] = SUPERIEUR;
		break;
		case '.':
		break;
		case ' ':
		break;
		default:
			fprintf(stderr,"Symbole [%d] inconnu\n",signe);
			exit(EXIT_FAILURE);
		break;	
	}
}

/*Remplit le tableau de relation binaire avec les relations de différence*/
void differenceBinaire(CSP *c)
{
	int i,j;
	for(i=0;i< c->nbCases;i++)
		for(j=0;j< c->nbCases;j++)
		{
			if(i==j)
				c->relationBinaire[i][j]=NONE;
			else if( (i%c->taille) == (j%c->taille) )
				c->relationBinaire[i][j]=DIFFERENT;
			else if( (i/c->taille) == (j/c->taille) )
				c->relationBinaire[i][j]=DIFFERENT;
		}
}

/*Remplit le tableau de relation naire avec les relations de différence*/
void differenceNaire(CSP *c)
{
	int i,j;
	/* Les lignes	*/
	for(i=0; i< c->taille ; i++)
	{
		for(j=0;j< c->taille ;j++)
			c->relationNaire[i][j + i*(c->taille)] = 1;
	}
	/* Les colonnes */
	for(i=0;i< c->taille; i++)
	{
		for(j=0;j< c->taille;j++)
			c->relationNaire[i+c->taille][i + j*(c->taille)] = 1;
	}
}

/*retourne le numéro de cellule*/
int numeroCellule(int ligne ,int colonne,int taille)
{
	return ligne*taille + colonne;
}
