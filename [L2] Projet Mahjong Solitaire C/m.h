#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ligne 20
#define colonne 20
#define hauteur 5
#define max 144

struct un{
  int value;
  int vf;
};
typedef struct un create [colonne][ligne][hauteur];

struct deux{
  int value;
  int col;
  int lig;
  int hau;
};
typedef struct deux create2 [100];

int valp[max];

void createvalp(void);
void shuffle(void);
bool isOpen(create, int, int, int);
void initialisetab(create);
void affiche(create);
void affichelibre(create);
void affichetab2(create2);
int reinitialise(create);
bool checknb(create, int);
int run(create, create2, int, int);
//int run2(create, create2, int, int);
