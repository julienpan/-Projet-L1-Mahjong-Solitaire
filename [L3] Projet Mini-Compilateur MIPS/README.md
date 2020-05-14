Projet compilateur Python vers MIPS en Racket

Possibilité de compiler vers MIPS les instructions suivantes :
	- Les opérations ahrithmétiques : a + b, a * b, a / b ...
	- Les opération logiques : or, and, xor, <<, >>
	- Les opérations de comparaison : <, >, <=, >=, !=, ==
	- Les déclaration de fonction
	- Les tests if, while, for ne fonctionnent pas correctement : "if" impossible avec une variable
																  									      "for" seulement pour des utilisation simple -> for i in range(0, 10)

Reconnaissance des variable non défini, si oui, renvoie "Error: this name is not defined"

