#include <stdio.h>
#include <stdlib.h>

#define LIGNE 25
#define COLONNE 20
#define MAX 100

/** Declaration d'une structure contenant la valeur de la case, et une valeur pour verifie qu'elle est vide**/
struct grille {
	int val;
};
typedef struct grille create[LIGNE][COLONNE];

/// Declaration d'un tableau de valeur entre 1-12
int valp[MAX];
/// Affiche toutes les pièces pour l'utilisateurs, ainsi que son point principale
void affpiece();
/// Fonction de remplissage du tableau valp[MAX]
void createvalp();
void shuffle();
/// Fonction d'initialisation de la grille Tetris
void initialise(create);
void affiche(create);
/// Fonction d'entrée, pour entrer les valeurs de la ligne et de la colonne
int entrer(create, int, int);
/// Toutes les pièces du jeu
void allpp(create, int, int, int);
/// Vérifie les cases
int verifier(create, int, int, int);
//int decalage(create, int, int);
/// Enlève une ligne ou une colonne remplie
int points(create);
