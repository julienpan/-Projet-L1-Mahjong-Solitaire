#include "tetris.h"

int main() {
	create tab;
	int nb, i;
	int piece;
	nb = 0, i = 0;
	createvalp();
	shuffle();

	initialise(tab);
	affiche(tab);
	affpiece();
	while (nb < 100) {
		piece = valp[i];
		printf("La piece a jouee est la piece %d\n", piece);
		entrer(tab, piece, 2);
		affiche(tab);
		affpiece();
		i++;
		nb++;
	}
}

