#include "m.h"

// Création des valeurs des 144 cartes
void createvalp(void){
  int i, valeur, count;
  valeur = 1;
  count = 0;
  for(i = 0; i < max; i++){
    if(count == 4){
      valeur++;
      count = 0;
    }
    valp[i] = valeur;
    count++;
  }
}
// Permet d'afficher les valeurs des paires et leurs coordonnées
// faites dans la fonction run
void affichetab2(create2 tab2){
  int i;
  for(i = 0; i < 100; i++)
    printf("%d = [%d][%d][%d] : %d\n", i,
    tab2[i].col, tab2[i].lig, tab2[i].hau, tab2[i].value);
}

// Mélange le tableau valp
void shuffle(void){
  int i, x, temp;
  for(i = 1; i <= max - 1; i++){
    x = rand() % i + 1;
    temp = valp[i];
    valp[i] = valp[x];
    valp[x] = temp;
  }
}

// Vérifie si la carte est libre et visible
bool isOpen(create tab, int c, int l, int h){
  if(tab[c][l][h].vf > 1){
    if((c == 3) && (l == 13) && (h == 0))
      if(tab[c-1][l+1][h].vf > 1){
        return false;
      }

    if((c == 3) && (l == 15) && (h == 0))
      if(tab[c-1][l-1][h].vf > 1){
        return false;
      }
    // Tuiles à droite
    if((c == 15) && (l == 14) && (h == 0))
      if(tab[c+1][l][h].vf > 1){
        return false;
      }


    if((c == 14) && (l == 13) && (h == 0))
      if(tab[c+1][l+1][h].vf > 1){
        return false;
      }


    if((c == 14) && (l == 15) && (h == 0))
      if(tab[c+1][l-1][h].vf > 1){
        return false;
      }

    //Exceptions pour la couche en dessous du pion à la couche 5
    if((c == 8) && (l == 13) && (h == 3))
      if(tab[c][l+1][h+1].vf > 1){
        return false;
      }

    if((c == 9) && (l == 13) && (h == 3))
      if(tab[c-1][l+1][h+1].vf > 1){
        return false;
      }

    if((c == 8) && (l == 15) && (h == 3))
      if(tab[c][l-1][h+1].vf>1){
        return false;
      }

    if((c == 9) && (l == 15) && (h == 3))
      if(tab[c-1][l-1][h+1].vf>1){
        return false;
      }

    if((c == 8) && (l == 14) && (h == 4))
      if(tab[c][l][h].vf > 1){
        return true;
      }


    if(tab[c][l][h+1].vf > 1){
      return false;
    }
    else{
      if((tab[c+1][l][h].vf > 1) && (tab[c-1][l][h].vf > 1))
          return false;

      else{
        return true;
      }
    }
  }
  return true;
}

// Initialise le mahjong
void initialisetab(create tab){
  int c,l,h;
  int i, x;
  x = 0;

  for(c = 0; c < colonne; c++)
  for(l = 0; l < ligne; l++)
  for(h = 0; h < hauteur; h++){
    tab[c][l][h].value = 0;
    tab[c][l][h].vf = 0;
  }

  // Couche 1
  for(i = 3; i <= 14; i++){
    tab[i][10][0].value = -1;
    tab[i][13][0].value = -1;
    tab[i][15][0].value = -1;
    tab[i][18][0].value = -1;
  }

  for(i = 5; i <= 12; i++){
    tab[i][11][0].value = -1;
    tab[i][17][0].value = -1;
  }

  for(i = 4; i <= 13; i++){
    tab[i][12][0].value = -1;
    tab[i][16][0].value = -1;
  }

  tab[2][14][0].value = -1;
  tab[15][14][0].value = -1;
  tab[16][14][0].value = -1;

  // Couche 2

  for(i = 6; i <= 11; i++){
    tab[i][11][1].value = -1;
    tab[i][12][1].value = -1;
    tab[i][13][1].value = -1;
    tab[i][15][1].value = -1;
    tab[i][16][1].value = -1;
    tab[i][17][1].value = -1;
  }

  // Couche 30

  for(i = 7; i <= 10; i++){
    tab[i][12][2].value = -1;
    tab[i][13][2].value = -1;
    tab[i][15][2].value = -1;
    tab[i][16][2].value = -1;
  }

  // Couche 4

  for(i = 8; i <= 9; i++){
    tab[i][13][3].value = -1;
    tab[i][15][3].value = -1;
  }

  // Couche 5

  tab[8][14][4].value = -1;

  for(c = 0; c < colonne; c++)
  for(l = 0; l < ligne; l++)
  for(h = 0; h < hauteur; h++)
    if(tab[c][l][h].value != 0){
      tab[c][l][h].value = valp[x];
      tab[c][l][h].vf = 2;
      x++;
  }
}

// Affiche toutes les cartes du mahjong
void affiche(create tab){
  int c,l,h;
  printf("Toutes les cartes du mahjong : \n\n");
  for(c = 0; c < colonne; c++)
  for(l = 0; l < ligne; l++)
  for(h = 0; h < hauteur; h++)
  if((tab[c][l][h].value != 0) && (tab[c][l][h].vf == 2)){
    printf("%d\t", tab[c][l][h].value);
    }
  printf("\n\n");
}

// Affiche toutes les cartes visibles et libres du mahjong
void affichelibre(create tab){
  int c,l,h;
  printf("Toutes les cartes visibles et libres du mahjong\n\n");
  for(c = 0; c < colonne; c++)
  for(l = 0; l < ligne; l++)
  for(h = 0; h < hauteur; h++)
  if((isOpen(tab,c,l,h) == true) && (tab[c][l][h].vf == 2)){
    printf("%d\t",tab[c][l][h].value);
    }
  printf("\n\n");
}

// Réinitialise le tableau en cas de blocage
int reinitialise(create tab){
  int c, l, h;
  for(c = colonne; c > 0; c--)
  for(l = ligne; l > 0; l--)
  for(h = hauteur; h > 0; h--)
  if(tab[c][l][h].vf == 1){
    tab[c][l][h].vf = 2;
  }
}

// Vérifie s'il reste encore des paires
bool checknb(create tab, int valeur){
  int c, l ,h;
  int c2, l2, h2;
  if(valeur > 36)
    return false;

  for(c = 0; c < colonne; c++)
  for(l = 0; l < ligne; l++)
  for(h = 0; h < hauteur; h++)
  if(isOpen(tab, c, l, h) == true){
    if((tab[c][l][h].value == valeur) && (tab[c][l][h].vf == 2)){
      c2 = c;
      l2 = l;
      h2 = h;
      for(c = 0; c < colonne; c++)
      for(l = 0; l < ligne; l++)
      for(h = 0; h < hauteur; h++)
      if((tab[c][l][h].value != 0) && (isOpen(tab, c, l, h) == true)
      && (tab[c][l][h].vf == 2))
        if((c != c2) || (l != l2) || (h != h2))
          if(valeur == tab[c][l][h].value){
            return true;
          }
        }
    }
  return checknb(tab, valeur + 1);
}
/*
// Deuxième run en choisissant une autre paire comme début

int run2(create tab, create2 tab2, int pos, int valeur){
  int c, l, h;
  int c2, l2, h2;
  int temp;
  if(tab2[pos].value == 0)
    return 0;

  temp = tab2[pos].value;
  for(c = 0; c < colonne; c++)
  for(l = 0; l < ligne; l++)
  for(h = 0; h < hauteur; h++)
  if(isOpen(tab, c, l, h) == true){
    if((tab[c][l][h].value == valeur) && (tab[c][l][h].vf == 2)
    && (tab[c][l][h].value != temp)){
      c2 = c;
      l2 = l;
      h2 = h;
      for(c = 0; c < colonne; c++)
      for(l = 0; l < ligne; l++)
      for(h = 0; h < hauteur; h++)
      if((tab[c][l][h].value != 0) && (isOpen(tab, c, l, h) == true)
      && (tab[c][l][h].vf == 2))
        if((c != c2) || (l != l2) || (h != h2))
          if((valeur == tab[c][l][h].value) && (valeur != temp)){

            printf("Carte 1 : [%d][%d][%d] : %d\n", c, l, h, tab[c][l][h].value);
            printf("Carte 2 : [%d][%d][%d] : %d\n", c2, l2, h2,
            tab[c2][l2][h2].value);

            printf("\n");
            tab[c][l][h].vf = 1;
            tab[c2][l2][h2].vf = 1;
          }
      }
  }
  return run2(tab, tab2, pos, valeur - 1);
}
*/

// Permet de trouver les paires
int run(create tab, create2 tab2, int pos, int valeur){
  int c, l, h;
  int c2, l2, h2;
  if(valeur < 0){
    affichelibre(tab);
    affiche(tab);
    if(checknb(tab, 1) == true)
      return run(tab, tab2, pos, 36);
    else{
      reinitialise(tab);
      printf("Blocage\n");
      return 0; // run2(tab, tab2, 0, 36);
    }
  }
  for(c = 0; c < colonne; c++)
  for(l = 0; l < ligne; l++)
  for(h = 0; h < hauteur; h++)
  if(isOpen(tab, c, l, h) == true){
    if((tab[c][l][h].value == valeur) && (tab[c][l][h].vf == 2)){
      c2 = c;
      l2 = l;
      h2 = h;

      for(c = 0; c < colonne; c++)
      for(l = 0; l < ligne; l++)
      for(h = 0; h < hauteur; h++)
      if((tab[c][l][h].value != 0) && (isOpen(tab, c, l, h) == true)
      && (tab[c][l][h].vf == 2))
        if((c != c2) || (l != l2) || (h != h2))
          if(valeur == tab[c][l][h].value){
            printf("Carte 1 : [%d][%d][%d] : %d\n", c, l, h, tab[c][l][h].value);
            printf("Carte 2 : [%d][%d][%d] : %d\n", c2, l2, h2,
            tab[c2][l2][h2].value);

            printf("\n");
            tab2[pos].value = tab[c][l][h].value;
            tab2[pos].col = c;
            tab2[pos].lig = l;
            tab2[pos].hau = h;
            pos++;
            tab[c][l][h].vf = 1;
            tab[c2][l2][h2].vf = 1;
          }
      }
  }
  return run(tab, tab2, pos, valeur - 1);
}
