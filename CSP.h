#ifndef DEF_CSP
#define DEF_CSP

	typedef enum { DIFFERENT, SUPERIEUR, INFERIEUR, NONE }symbole;
	typedef enum {BINAIRE, NAIRE}type;

	typedef struct
	{
		type aire; //binaire ou naire
		symbole **relationBinaire; //tableau de relation binaire
		int **relationNaire; //tableau de relation naire
		int **domaine; //tableau pour le domaine
		/* 
		-1 disponible
		 0 indisponible
		 >0 occupé(utile pour fc)  */
		int nbCases;
		int taille;
	}CSP;

	/*Crée le CSP avec la taille , le fichier de la grille et le type( binaire, naire)
	-Renvoie NULL si erreur
	-Sinon renvoie le csp*/
	CSP* creerCSP(int taille,FILE *f,type aire);
	/*Désalloue le csp et met c à null*/
	void libererCSP(CSP *c);
	/*Lit le Futoshiki*/
	void lireFutoshiki(CSP *c,FILE *f);
	/*Cette fonction lit les lignes(une ligne chiffre et une ligne symbole si possible)*/
	void lireLigne(CSP *c,FILE *f,int ligne);
	/*lit la valeur et remplit le domaine*/
	void lireValeur(CSP *c,FILE *f,int cellule);
	/*Remplit le domaine si la valeur est définie dans le fichier*/
	void valeurDefinie(CSP *c,int cellule,int valeur);
	/*Remplit le domaine si la valeur n'est pas definie dans le fichier*/
	void valeurNonDefinie(CSP *c,int cellule);
	/*Remplit relation binaire en fonction du symbole*/
	void lireSymbole(CSP *c,FILE* f, int ligneX , int colonneX,int ligneY, int colonneY);
	/*Remplit le tableau de relation binaire avec les relations de différence*/
	void differenceBinaire(CSP *c);
	/*Remplit le tableau de relation naire avec les relations de différence*/
	void differenceNaire(CSP *c);
	/*retourne le numéro de cellule*/
	int numeroCellule(int ligne ,int colonne,int taille);
#endif