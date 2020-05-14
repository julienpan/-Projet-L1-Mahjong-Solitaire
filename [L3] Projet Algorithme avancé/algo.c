#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

/*------------------ BRESENHAM ------------------*/

void affiche(int x, int y){
	printf("%d \t %d\n", x, y);
}
void bresenham(int u, int v){
	int x, y, delta, incH, incD;
	incH = v << 1;
	delta = incH - u;
	incD = delta - u;
	for(x = 0, y = 0; x <= u; x++){
		//affiche(x, y);
		if(delta > 0){
			y++;
			delta += incD;
		}
		else 
			delta += incH;		
	}
}
/*---------------- CASTLE PITTEWAY --------------*/

char * castle(int dx, int dy){
	int i = 1;
	dx -= dy;
	char* s = malloc(sizeof(char));
	s = "0";
	char* t = malloc(sizeof(char));
	t = "1";
	char * res;
	while(dx != dy){
		if(dx > dy){
			dx -= dy;
			res = (char *) malloc(1 + strlen(s) + strlen(t));
			strcpy(res, s);
			strcat(res, t);
			t = res;
		}
		else{
			dy -= dx;
			res = (char *) malloc(1 + strlen(s) + strlen(t));
		
			strcpy(res, s);
			strcat(res, t);
			s = res;
		}
	}	
	char * resFinal = (char *) malloc(1 + strlen(s) + strlen(t));
	strcpy(resFinal, s);
	strcat(resFinal, t);
	char * resFinalDxFois = (char *) malloc(dx * (strlen(s)+ strlen(t)));	
	strcpy(resFinalDxFois, resFinal);
	while(i < dx){
		strcat(resFinalDxFois, resFinal);
		i++;
	}
	//printf("%s \n", resFinalDxFois);	
	return resFinalDxFois;
}

// Temps
int main(void){
	clock_t start_t, end_t, start2_t, end2_t;
	float time_t, time2_t;
	start_t = clock();
	bresenham(1000000, 900000);
	end_t = clock();
	time_t = (float)(end_t - start_t)/CLOCKS_PER_SEC;
	printf("temps d'execution de l'algorithme de Bresenham: %f ms\n", time_t);

	start2_t = clock();
	char * d = castle(1000000, 900000);
	end2_t = clock();
	time2_t = (float)(end2_t - start2_t)/CLOCKS_PER_SEC;
	printf("temps d'execution de l'algorithme de Castle et Pitteway: %f ms\n\n", time2_t);
	return 0;
}


// Pour obtenir le graphe du temps avec gnuplot

/*int main(void){
	int i;
	clock_t start_t, end_t, start2_t, end2_t;
	float time_t, time2_t;	
	
	for(i = 0; i < 100000; i++){		
		start_t = clock();
		bresenham(100000, 90000);
		end_t = clock();
		time_t = (float)(end_t - start_t)/CLOCKS_PER_SEC;
		printf("%f \n", time_t);
	}
	for(i = 0; i < 100000; i++){
		start2_t = clock();
		castle(100000, 90000);
		end2_t = clock();
		time2_t = (float)(end2_t - start2_t)/CLOCKS_PER_SEC;
		printf("%f\n", time2_t);
	}
	return 0;
}*/
