#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>

enum {
  MaxLigne = 1024,              // longueur max d'une ligne de commandes
  MaxMot = MaxLigne / 2,        // nbre max de mot dans la ligne
  MaxDirs = 100,		// nbre max de repertoire dans PATH
  MaxPathLength = 512,		// longueur max d'un nom de fichier
};

int decouper(char *, char *, char **, int);

// Cherche une index null
int search(int * mot){
  int i;
  for(i = 0; i < 20; i++){
    if(mot[i] == 1){
      return i;
    }
  }
  return 0;
}
// Decale les arguments mais ne fonctionne pas
// pour plusieurs commande à la suite
void shift(char * mot[], int i, int nb){
  int j;
  for(j = i; j <= nb - 4; j += 2){
    mot[j] = mot[j+2];

    mot[j+1] = mot[j+3];
  }
  mot[nb-3] = NULL;
  mot[nb-2] = NULL;
}

int main(int argc, char * argv[]){

  char ligne[MaxLigne];
  char pathname[MaxPathLength];
  char * cmd[MaxLigne]; // Commande ";"
  char * mot[MaxMot];
  char * dirs[MaxDirs];
  char * dir; // Commande "cd"
  int i, tmp, dir2;
  int pid, proc; // Commande "&"
  proc = 1;
  int nbmot, nb;
  nb = 20;
  int status;
  int pids[nb];
  int in, out;
  int f1, f2;
  int test = 0;
  int erreur;
  char * prompt[MaxMot];
  *prompt = getcwd(pathname, MaxPathLength);
  /*int stdout = dup(1); // Commande "|" non réussi
  int pipefd[2];
  pipe(pipefd);
  int nbmot2;*/

  printf("\033[33;1m\t ___ ___  ____  ____   ____   _____ __ __    ___  _      _     \n");
  printf("\t|   |   ||    ||    | |    | / ___/|  |  |  /  _]| |    | |    \n");
  printf("\t| _   _ | |  | |  _  | |  | (   |_ |  |  | /  [_ | |    | |    \n");
  printf("\t|  |_/  | |  | |  |  | |  |  |__  ||  _  ||    _]| |___ | |___ \n");
  printf("\t|   |   | |  | |  |  | |  |  /  | ||  |  ||   [_ |     ||     |\n");
  printf("\t|   |   | |  | |  |  | |  |  |    ||  |  ||     ||     ||     |\n");
  printf("\t|___|___||____||__|__||____|  |___||__|__||_____||_____||_____|\n");
  printf("\n\n\n");



  /* Decouper PATH en repertoires */
  decouper(getenv("PATH"), ":", dirs, MaxDirs);

  /* Lire et traiter chaque ligne de commande */
  for(printf("\033[35;1m%s$ ", *prompt); fgets(ligne, sizeof ligne, stdin) != 0; printf("\033[35;1m%s$ ", *prompt)){
    nbmot = decouper(ligne, " \t\n", mot, MaxMot);
/*  decouper(ligne, " \t\n", cmd, MaxMot);      // L'implémentation de la commande ";"
    for(i = 0; cmd[i] != 0; i++){               // n'est pas parfait
      decouper(cmd[i], " \t\n", mot, MaxMot);*/


    for(i = 0;i < nb; i++){
      if(pids[i] != 1){
        pid_t return_pid = waitpid(pids[i], &status, WNOHANG)
	      if(return_pid == pids[i])
          pids[i] = 1;
      }
    }

    if (mot[0] == 0)            // ligne vide
      continue;

    // Commande "exit"
    if(strcmp(mot[0], "exit") == 0)
      break; // on sort de la boucle puis return 0

    // Commande "cd"
    if(strcmp(mot[0],"cd") == 0){
      if(mot[1] == NULL){
        dir = getenv("HOME"); // cd sans argument
      if(dir == 0)
        dir = getcwd(pathname, MaxPathLength);

      }
      else if (mot[2] != NULL){
        fprintf(stderr, "usage : %s [dir]\n",mot[0]); // Erreur "cd"
        continue;
      }
      else dir = mot[1];
        dir2 = chdir(dir); // change de répertoire
        *prompt = getcwd(pathname, MaxPathLength); // Change le prompt
        if(dir2 < 0)
          perror(dir);
        continue;
      }

    tmp = fork();               // lancer le processus enfant
    i = search(pids);
    pids[i] = tmp;
    pid = tmp;

    if (tmp < 0){
      perror("fork");
      continue;
    }
    // Commande "&" sans attendre l'enfant
    if(*mot[nbmot-2] == '&'){
      printf("[%d]%d\n",proc, pid);
      mot[nbmot-2]= NULL;
      proc++;
      if(tmp != 0){
	       continue;
      }
    }
    else{
      if (tmp != 0){
      // parent : attendre la fin de l'enfant
	     while(wait(0) != tmp)
	      ;
	     for(i=0;i<nb;i++){
	       if(pids[i] == tmp){
	       pids[i]=1;
	      }
	     }
	      continue;
      }
    }
    // Commndes des redirections
    for(i = 0;i < nbmot-2; i++){
      if(*mot[i] == '<'){
        in = i;
        f1 = open(mot[in + 1], O_RDONLY);
      }
      if(*mot[i] == '>'){
        out = i;
        f2 = open(mot[out + 1], O_CREAT|O_WRONLY, 0666);
        tmp++;
      }
    }
    if(tmp != 0){
      for(i = 0; i < nbmot-2; i++){
        if(*mot[i] == '<'){
          shift(mot, i, nbmot);
          nbmot = nbmot - 2;
          dup2(f1, 0);
	        i--;
        }
        if(*mot[i] == '>'){
          shift(mot, i, nbmot); // Pour avoir les arguments à la suite
          nbmot = nbmot -2;
          i--;
          dup2(f2, 1);
        }
      }
      close(f2);
      close(f1);
    }
    else if(strcmp(mot[i], ">>") == 0){
			out = open(mot[i+1], O_WRONLY | O_APPEND | O_CREAT, 0666);
			if (out < 0)
				return 1;

			if(dup2(out, 1) < 0)
				return 1;

			close(out);
			mot[i] = NULL;
			mot[i+1] = NULL;
			i += 2;
		}

    // Redirection d'erreur ne fonctionne pas correctement
    else if(strcmp(mot[i], "2>") == 0){
			out = open(mot[i+1], O_WRONLY | O_CREAT, 0666);
			if (out < 0)
				return 1;

			if(dup2(out, 2) < 0)
				return 1;

			close(out);
			mot[i] = NULL;
			mot[i+1] = NULL;
			i += 2;
		}
    else if(strcmp(mot[i], "2>>") == 0){
			out = open(mot[i+1], O_WRONLY | O_CREAT | O_APPEND, 0666);

			if(out < 0){
        return 1;
			}

			if(dup2(erreur, 2) < 0)
				return 1;

			close(erreur);
			mot[i] = NULL;
			mot[i+1] = NULL;
			i += 2;
		}
		else
			i++;

				// enfant : exec du programme
    for(i = 0; dirs[i] != 0; i++){
      snprintf(pathname, sizeof pathname, "%s/%s", dirs[i], mot[0]);
      execv(pathname, mot);
    }
                                // aucun exec n'a fonctionne
    fprintf(stderr, "%s: not found\n", mot[0]);
    exit(1);
  }

  printf("\033[33;1mA bientot !\n");
  return 0;

}
