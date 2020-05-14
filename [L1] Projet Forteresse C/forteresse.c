#include <stdio.h>
#define ligne 6
#define colonne 6

char pion[ligne][colonne];
int hauteur[ligne][colonne];


struct CASE {
  char pion;
  int hauteur;
};

typedef struct CASE CASE;



void initialise(){
  
  int l,c;
  for(l=0;l<=ligne;l++){
    for(c=0;c<=colonne;c++){
      hauteur[l][c] = 0;
      pion[l][c] = ' ';
    }
  }
  
}



void plusgrand(int lgn, int col, CASE jadverse){
  
  CASE h;
  int hbas, hhaut, hgauche, hdroite;
  int pbas, phaut, pgauche, pdroite;

  h.hauteur = hauteur[lgn][col];
  
  hbas = hauteur[lgn-1][col];  
  hhaut = hauteur[lgn+1][col];
  hgauche = hauteur[lgn][col-1];
  hdroite = hauteur[lgn][col+1];

  pbas = pion[lgn-1][col];  
  phaut = pion[lgn+1][col];
  pgauche = pion[lgn][col-1];
  pdroite = pion[lgn][col+1];

  


  //pression pion gauche plus grande que soi
  if(h.hauteur<hgauche && hgauche >= 1 && pgauche == jadverse.pion){
      hauteur[lgn][col] = 0;
      pion[lgn][col] = ' ';
      printf("La forteresse (%d,%d) a été détruite !\n",lgn,col);     
  }

  //pression pion droite plus grande que soi
  if(h.hauteur<hdroite && hdroite >= 1 && pdroite == jadverse.pion){
      hauteur[lgn][col] = 0;
      pion[lgn][col] = ' ';
      printf("La forteresse (%d,%d) a été détruite !\n",lgn,col);     
  }

  //pression pion bas plus grande que soi
  if(h.hauteur<hbas && hbas >= 1 && pbas == jadverse.pion){
      hauteur[lgn][col] = 0;
      pion[lgn][col] = ' ';
      printf("La forteresse (%d,%d) a été détruite !\n",lgn,col);     
  }

  //pression pion haut plus grande que soi
  if(h.hauteur<hhaut && hhaut >= 1 && phaut == jadverse.pion){
      hauteur[lgn][col] = 0;
      pion[lgn][col] = ' ';
      printf("La forteresse (%d,%d) a été détruite !\n",lgn,col);     
  }


  //pression plus grande que pion gauche
  if(h.hauteur>hgauche && hgauche >= 1 && pgauche == jadverse.pion){
      hauteur[lgn][col-1] = 0;
      pion[lgn][col-1] = ' ';
      printf("La forteresse (%d,%d) a été détruite !\n",lgn,col-1);
      
  }

  //pression plus grande que pion droite
  if(h.hauteur>hdroite  && hdroite >=1 && pdroite == jadverse.pion){
      hauteur[lgn][col+1] = 0;
      pion[lgn][col+1] = ' ';
      printf("La forteresse (%d,%d) a été détruite !\n",lgn,col+1);
	
  }

  //pression plus grande que pion bas
  if(h.hauteur>hbas && hbas >=1  && pbas == jadverse.pion){
      hauteur[lgn-1][col] = 0;
      pion[lgn-1][col] = ' ';
      printf("La forteresse (%d,%d) a été détruite !\n",lgn-1,col);
	
  }

  //pression plus grande que pion haut
  if(h.hauteur>hhaut && hhaut >=1  && phaut == jadverse.pion){
      hauteur[lgn+1][col] = 0;
      pion[lgn+1][col] = ' ';
      printf("La forteresse (%d,%d) a été détruite !\n",lgn+1,col);
	
  }
}



void encercle(int lgn, int col, CASE mj, CASE jadverse){


  
  int bas, haut, gauche, droite;
  int hhg, hbg, hhd, hbd;
  int hbas2, hhaut2, hdroite2, hgauche2;
  
  char pd, pg, ph, pb;
  char phg, pbg, phd, pbd;
  char pbas2,phaut2,pdroite2,pgauche2;
  

  
  hhg = hauteur[lgn+1][col-1];
  hbg = hauteur[lgn-1][col-1];
  hhd = hauteur[lgn+1][col+1];
  hbd = hauteur[lgn-1][col+1];

  phg = pion[lgn+1][col-1];
  pbg = pion[lgn-1][col-1];
  phd = pion[lgn+1][col+1];
  pbd = pion[lgn-1][col+1];

  ph = pion[lgn+1][col];
  pb = pion[lgn-1][col];
  pg = pion[lgn][col-1];
  pd = pion[lgn][col+1];
  

  bas = hauteur[lgn-1][col];
  hbas2 = hauteur[lgn-2][col];
  pbas2 = pion[lgn-2][col];
  
  haut = hauteur[lgn+1][col];
  hhaut2 = hauteur[lgn+2][col];
  phaut2 = pion[lgn+2][col];
  
  gauche = hauteur[lgn][col-1];
  hgauche2 = hauteur[lgn][col-2];
  pgauche2 = pion[lgn][col-2];
  
  droite = hauteur[lgn][col+1];
  hdroite2 = hauteur[lgn][col+2];
  pdroite2 = pion[lgn][col+2];



  
  //haut et gauche
  if((hhg >= 1 && phg == mj.pion && ph == jadverse.pion ) && (hauteur[lgn+1][col] > 0)){

      hauteur[lgn+1][col] = 0;
      pion[lgn+1][col] = ' ';
      printf("La forteresse (%d,%d) a été détruite !\n",lgn+1,col);
  }

  //bas et gauche
  if((hbg >= 1 && pbg == mj.pion && pb == jadverse.pion) && (hauteur[lgn-1][col] > 0)){

      hauteur[lgn-1][col] = 0;
      pion[lgn-1][col] = ' ';
      printf("La forteresse (%d,%d) a été détruite !\n",lgn-1,col);
  }

  //haut et droite
  if((hhd >= 1 && phd == mj.pion && ph == jadverse.pion) && (hauteur[lgn+1][col] > 0)){

      hauteur[lgn+1][col] = 0;
      pion[lgn+1][col] = ' ';
      printf("La forteresse (%d,%d) a été détruite !\n",lgn+1,col);
  }

  //bas et droite
  if((hbd >= 1 && pbd == mj.pion && pb == jadverse.pion) && (hauteur[lgn-1][col] > 0)){

      hauteur[lgn-1][col] = 0;
      pion[lgn-1][col] = ' ';
      printf("La forteresse (%d,%d) a été détruite !\n",lgn-1,col);
  }


  
  
  //haut et bas
  if((hbas2 >= 1 && pbas2 == mj.pion && pb == jadverse.pion) && (hauteur[lgn-1][col] > 0)){

      hauteur[lgn-1][col] = 0;
      pion[lgn-1][col] = ' ';
      printf("La forteresse (%d,%d) a été détruite !\n",lgn-1,col);
  }


  //bas et haut
  if((hhaut2 >= 1 && phaut2 == mj.pion && ph == jadverse.pion) && (hauteur[lgn+1][col] > 0)){

      hauteur[lgn+1][col] = 0;
      pion[lgn+1][col] = ' ';
      printf("La forteresse (%d,%d) a été détruite !\n",lgn+1,col);
  }


  //droite et gauche
  if((hgauche2 >= 1 && pgauche2 == mj.pion && pg == jadverse.pion) && (hauteur[lgn][col-1] > 0)) {

      hauteur[lgn][col-1] = 0;
      pion[lgn][col-1] = ' ';
      printf("La forteresse (%d,%d) a été détruite !\n",lgn,col-1);
  }

  //gauche et droite
  if((hdroite2 >= 1 && pdroite2 == mj.pion && pd == jadverse.pion) && (hauteur[lgn][col+1] > 0)){

      hauteur[lgn][col+1] = 0;
      pion[lgn][col+1] = ' ';
      printf("La forteresse (%d,%d) a été détruite !\n",lgn,col+1);
  }

}




void majaff(int lgn, int col, CASE mj, CASE jadverse){

  int x,y;


  
  for (x = ligne; x >= 1; --x){
    printf(" -----------------------------------------");
    printf("\n %d | ", x);
    
    for (y = 1; y <= colonne; ++y){
      printf(" %c%d |",pion[x][y],hauteur[x][y]);
    }
      printf("\n");
  }
  
  printf(" ------------------------------------------");
  printf("\n");


   printf("    ");
  for (x = 1; x <= ligne; ++x){
    printf("  %d  ", x);
  }

  printf("\n");


  encercle(lgn,col,mj,jadverse);
  plusgrand(lgn,col,jadverse);
  
}



void joueurA(){
  
  int lgn, col;
  CASE joueura;
  CASE joueurb;
  joueura.pion = 'a';
  joueurb.pion = 'b';
  
  
  printf("Au tour du joueur A :\n");
  printf("Entrez le numéro d'une ligne et d'une colonne :\n");
  scanf("%d %d",&lgn, &col);

  
  if((col > colonne) || (col <= 0) || (lgn <= 0) || (lgn > ligne)){
    printf("Cette case n'existe pas ou est hors du tableau qui est de taille \"%d x %d\" \n",ligne,colonne);
    joueurA();
    printf("Réessayez:\n");
  }


  
  if(pion[lgn][col] != joueurb.pion){
     
    if(hauteur[lgn][col] <= 2){
      pion[lgn][col] = joueura.pion;
      hauteur[lgn][col]++;
      
    }
    
    else{
      printf("La hauteur maximale a été atteinte\n");
      printf("Réessayez:\n");
      joueurA();
    }
  }
  else{
      printf("Cette case est déjà occupée par le joueur adverse.\n");
      printf("Réessayez:\n");
      joueurA();
    }
  majaff(lgn,col,joueura,joueurb);
}
 



void joueurB(){
  
  int lgn, col;
  CASE joueurb;
  CASE joueura;
  joueura.pion = 'a';
  joueurb.pion = 'b';
  
  printf("Au tour du joueur B :\n");
  printf("Entrez le numero d'une ligne et d'une colonne :\n");   
  scanf("%d %d",&lgn, &col);

  if((col > colonne) || (col <= 0) || (lgn <= 0) || (lgn > ligne)){
    printf("Cette case n'existe pas ou est hors du tableau qui est de taille \"%d x %d\" \n",ligne,colonne);
    printf("Réessayez:\n");
    joueurB();
     }
     
     if(pion[lgn][col] != joueura.pion){
       
       if(hauteur[lgn][col] <= 2){
	 pion[lgn][col] = joueurb.pion;
	 hauteur[lgn][col]++;
       }
 
       else{
	 printf("La hauteur maximale a été atteinte\n");
	 printf("Réessayez:\n");
	 joueurB();
       }
     }
     
       else{
	 printf("Cette case est déjà occupée par le joueur adverse.\n");
	 printf("Réessayez:\n");
	 joueurB();
       }
     majaff(lgn,col,joueurb,joueura);
}




void ordinateur(int lgn, int col){

  CASE joueura,joueurb;
  joueura.pion = 'a';
  joueurb.pion = 'b';
  

    if(pion[lgn][col] != joueura.pion){
    
      if(hauteur[lgn][col] <= 2){
	pion[lgn][col] = joueurb.pion;
	hauteur[lgn][col]++;
      }
    }
  
  
  printf("L'ordinateur joue : \n");
  majaff(lgn,col,joueurb,joueura);
}







void findujeu(){


  int nbpiona, nbpionb;
  int lgn,col;

  
  for(lgn = 1; lgn <= ligne; ++lgn){
    for(col = 1; col <= colonne; ++col){
      if(pion[lgn][col] == 'a'){
	nbpiona++;
      }
      if(pion[lgn][col] == 'b'){
	nbpionb++;
      }
    }
  }
  
  if(nbpiona == ligne*colonne){
    printf("Le joueur A gagne la partie\n");
  }
  if(nbpionb == ligne*colonne){
    printf("Le joueur B gagne la partie\n");
  }
}
  

void jouer(){

  int i;
  
  //cette fonction sert aussi a changer de tour
  
  for(i=2;i<100;i++){
    if(i%2 == 0){
      joueurA();
    }
    else{
      joueurB();
    }
    findujeu();
  }
}

 void jouerordi(){

   int i,lgn,col;
   lgn = 1;
   col = 1;
  
  //cette fonction sert aussi a changer de tour
  
  for(i=2;i<100;i++){
    if(i%2 == 0){
      joueurA();
    }
    else{
      if(lgn > 6){
	col++;
	lgn = 1;
      ordinateur(lgn++,col);
      }
      else{
	ordinateur(lgn++,col);
      }
    }
    findujeu();
  }
}


  
void choisir_jeu(int nb){

  printf("Bienvenue dans le jeu Forteresse\n");
  printf("1- Joueur A vs Joueur B\n");
  printf("2- Jouer contre l'ordinateur\n");
  printf("Choisissez le mode de jeu :\n");
  scanf("%d",&nb);

  if(nb == 1){  
    initialise();
    jouer();
  }
  if(nb == 2){
    initialise();
    jouerordi();
  }

  if(nb != 1 || nb != 2){
    printf("Veuillez rentrer une valeur valide\n");
    choisir_jeu(nb);
  }
}

		 


int main(){
  int nb;
  choisir_jeu(nb);
}











