#include "tetris.h"

int verifier(create tab,int x, int y, int piece);
int points(create tab);


void affpiece() {
  printf("\033[1mLes pieces du jeu sont : \n\n");
  printf("\033[1m1.  o   o   2.o x o   3. o o o   4. o o o\n");
  printf("\033[1m    o   o     o   o      x              x\n");
  printf("\033[1m    o x o     o   o      o o o      o o o\n");
  printf("\n");
  printf("\033[1m5.  o     6. x o o  7.     o  8. o o x\n");
  printf("\033[1m    x o o    o         o o x         o\n");
  printf("\033[1m\n");
  printf("\033[1m9.     o        10. x   11. x o   12. x\n");
  printf("\033[1m       o                              o\n");
  printf("\033[1m   o o x o o                           \n");
  printf("\033[1m       o                               \n");
  printf("\033[1m       o                               \n");

  printf("\n\n");
  printf("Les coordonnees entrees sont situees sur la croix de la piece.\n\n");
}


void createvalp() {
  int i, nb;
  nb = 1;
  for (i = 0; i < MAX; i++) {
    valp[i] = nb;
    ++nb;
    printf("%d\n", valp[i]);
    if (nb == 13) {
      nb = 1;
    }
  }
}

/** Fonction qui permet de mélanger les valeurs du tableau pour avoir un aléatoire **/
void shuffle() {
  int i, x, temp;
  for (i = 1; i <= MAX - 1; i++) {
    x = rand() % i + 1;
    temp = valp[i];
    valp[i] = valp[x];
    valp[x] = temp;
  }
}


void initialise(create tab) {
  int lig, col;
  for (lig = 0; lig < LIGNE; lig++)
    for (col = 0; col < COLONNE; col++) {
      tab[lig][col].val = 0;
		
    }
}

/* Fonction d'affichage */
void affiche(create tab) {
  int lig, col;
  int count = 0;
  int num = 0;  
  printf("\033[1m    0       1       2       3       4       5       6       7       8       9       10      11      12      13      14      15      16      17      18      19\n");
  for (lig = 0; lig < LIGNE; lig++)
    for (col = 0; col < COLONNE; col++) {
      printf("|   %d   ", tab[lig][col].val);
      ++count;
      if (count == COLONNE) {
	printf("%d\n", num);
	num = num + 1;
	count = 0;
      }
    }
}


int entrer(create tab, int piece, int err) {
  int x, y;
  if(err == 1)
    printf("Erreur de placement\n");
	
  if(err == 0)
    printf("Les coordonnées sont incorrectes ! \n");

  printf("\033[1mChoisissez la ligne (0-24) et la colonne (0-19) : \n");
  scanf("%d %d", &x, &y);
  if (((x > 24) || (x < 0)) || (y > 19) || (y < 0)) {
    return entrer(tab, piece, 0);
  }
  else {
    return verifier(tab, x, y, piece);
  }
}


void allp(create tab, int x, int y, int piece){
  switch(piece){
  case 1:
    tab[x][y].val = 5;
    tab[x][y + 1].val = 5;
    tab[x][y - 1].val = 5;
    tab[x - 1][y + 1].val = 5;
    tab[x - 2][y + 1].val = 5;
    tab[x - 1][y - 1].val = 5;
    tab[x - 2][y - 1].val = 5;
    break;
  case 2:
    tab[x][y].val = 5;
    tab[x][y + 1].val = 5;
    tab[x][y - 1].val = 5;
    tab[x + 1][y - 1].val = 5;
    tab[x + 2][y - 1].val = 5;
    tab[x + 1][y + 1].val = 5;
    tab[x + 2][y + 1].val = 5;
    break;
  case 3:
    tab[x][y].val = 5;
    tab[x + 1][y].val = 5;
    tab[x - 1][y].val = 5;
    tab[x + 1][y + 1].val = 5;
    tab[x + 1][y + 2].val = 5;
    tab[x - 1][y + 1].val = 5;
    tab[x - 1][y + 2].val = 5;
    break;
  case 4:
    tab[x][y].val = 5;
    tab[x + 1][y].val = 5;
    tab[x + 1][y - 1].val = 5;
    tab[x + 1][y - 2].val = 5;
    tab[x - 1][y].val = 5;
    tab[x - 1][y - 1].val = 5;
    tab[x - 1][y - 2].val = 5;
    break;
  case 5:
    tab[x][y].val = 5;
    tab[x - 1][y].val = 5;
    tab[x][y + 1].val = 5;
    tab[x][y + 2].val = 5;
    break;
  case 6:
    tab[x][y].val = 5;
    tab[x + 1][y].val = 5;
    tab[x][y + 1].val = 5;
    tab[x][y + 2].val = 5;

    break;
  case 7:
    tab[x][y].val = 5;
    tab[x][y - 1].val = 5;
    tab[x][y - 2].val = 5;
    tab[x - 1][y].val = 5;
    break;
  case 8:
    tab[x][y].val = 5;
    tab[x][y - 1].val = 5;
    tab[x][y - 2].val = 5;
    tab[x + 1][y].val = 5;
    break;
  case 9:
    tab[x][y].val = 5;
    tab[x][y - 1].val = 5;
    tab[x][y - 2].val = 5;
    tab[x + 1][y].val = 5;
    tab[x + 2][y].val = 5;
    tab[x - 1][y].val = 5;
    tab[x - 2][y].val = 5;
    tab[x][y + 1].val = 5;
    tab[x][y + 2].val = 5;
    break;
  case 10:
    tab[x][y].val = 5;
    break;
  case 11:
    tab[x][y].val = 5;
      tab[x][y + 1].val = 5;
    break;
  case 12:
    tab[x][y].val = 5;
    tab[x + 1][y].val = 5;
    break;  
  }
}



int verifier(create tab, int x, int y, int piece) {
  if (piece == 1) {
    if ((tab[x][y].val != 5) && (tab[x][y + 1].val != 5) && (tab[x][y - 1].val != 5) && (tab[x - 1][y + 1].val != 5) && (tab[x - 2][y + 1].val != 5) && (tab[x - 1][y - 1].val != 5) && (tab[x - 2][y - 1].val != 5)){
      allp(tab,x,y,piece);
      return points(tab);
    }
		
    return entrer(tab, piece, 1);
  }
  if (piece == 2) {
    if ((tab[x][y].val != 5) && (tab[x][y + 1].val != 5) && (tab[x][y - 1].val != 5) && (tab[x + 1][y - 1].val != 5) && (tab[x + 2][y - 1].val != 5) && (tab[x + 1][y + 1].val != 5) && (tab[x + 2][y + 1].val != 5)){
      allp(tab,x,y,piece);
      return points(tab);
    }
	
    return entrer(tab, piece, 1);
  }
  if (piece == 3) {
    if ((tab[x][y].val != 5) && (tab[x + 1][y].val != 5) && (tab[x - 1][y].val != 5) && (tab[x + 1][y + 1].val != 5) && (tab[x + 1][y - 2].val != 5) && (tab[x - 1][y + 1].val != 5) && (tab[x - 1][y + 2].val != 5)){
      allp(tab,x,y,piece);
      return points(tab);
    }
		
    return entrer(tab, piece, 1);
  }
  if (piece == 4) {
    if ((tab[x][y].val != 5) && (tab[x + 1][y].val != 5) && (tab[x + 1][y - 1].val != 5) && (tab[x + 1][y - 2].val != 5) && (tab[x - 1][y].val != 5) && (tab[x - 1][y - 1].val != 5) && (tab[x - 1][y - 2].val != 5)) {
      allp(tab,x,y,piece);
      return points(tab);
    }
		
    return entrer(tab, piece, 1);
  }
  if (piece == 5) {
    if ((tab[x][y].val != 5) && (tab[x - 1][y].val != 5) && (tab[x][y + 1].val != 5) && (tab[x][y + 2].val != 5)) {
      allp(tab,x,y,piece);
      return points(tab);
    }

		
    return entrer(tab, piece, 1);
  }
  if (piece == 6) {
    if ((tab[x][y].val != 5) && (tab[x + 1][y].val != 5) && (tab[x][y + 1].val != 5) && (tab[x][y + 2].val != 5)){
      allp(tab,x,y,piece);
      return points(tab);
    }
		
    return entrer(tab, piece, 1);
  }	
  if (piece == 7) {
    if ((tab[x][y].val != 5) && (tab[x][y - 1].val != 5) && (tab[x][y - 2].val != 5) && (tab[x - 1][y].val != 5)){
      allp(tab,x,y,piece);
      return points(tab);
    }

		
    return entrer(tab, piece, 1);
  }
	
  if (piece == 8) {
    if ((tab[x][y].val != 5) && (tab[x][y - 1].val != 5) && (tab[x][y - 2].val != 5) && (tab[x + 1][y].val != 5)){
      allp(tab,x,y,piece);
      return points(tab);
    }

		
    return entrer(tab, piece, 1);
  }
	
  if (piece == 9) {
    if ((tab[x][y].val != 5) && (tab[x][y - 1].val != 5) && (tab[x][y - 2].val != 5) && (tab[x + 1][y].val != 5) && (tab[x + 2][y].val != 5) && (tab[x - 1][y].val != 5) && (tab[x - 2][y].val != 5) && (tab[x][y + 1].val != 5) && (tab[x][y + 2].val != 5)){
      allp(tab,x,y,piece);
      return points(tab);
    }

		
    return entrer(tab, piece, 1);
  }
  if (piece == 10) {
    if (tab[x][y].val != 5) {
      allp(tab,x,y,piece);
      return points(tab);
    }
	
    return entrer(tab, piece, 1);	
  }

  if (piece == 11) {
    if ((tab[x][y].val != 5) && (tab[x][y + 1].val != 5)){
      allp(tab,x,y,piece);
      return points(tab);
    }
		
    return entrer(tab, piece, 1);
  }
	
  if (piece == 12) {
    if ((tab[x][y].val != 5) && (tab[x+1][y].val != 5)){
      allp(tab,x,y,piece);
      return points(tab);
    }
		
    return entrer(tab, piece, 1);
  }	
  return 0;
}





int points(create tab) {
  int lig, col;
  int count;
  count = 0;
	
  for (lig = 0; lig < LIGNE; lig++)
    for (col = 0; col < COLONNE; col++) {
      if (tab[lig][col].val != 0) {
	++count;
	if (count == 20) {
	  for (col = 0; col < COLONNE; col++) {				
	    tab[lig][col].val = 0;
	    
	  }
	  count = 0;
	}
      }
      else {
	count = 0;
      }
    }

}
