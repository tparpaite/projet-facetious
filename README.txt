###############
# projet-i1-c #
###############

PARPAITE Thibault  
MENANTEAU Yoann

ENSEIRB-MATMECA  
Projet réalisé en novembre-décembre-janvier 2016 dans le cadre de l'UE Projets de première année d'informatique.   
L'objectif de ce projet était d'implémenter le jeu Facetious Pelican ainsi que différents solveurs ayant une efficacité croissante.


#############
# Nota Bene #
#############

La "baseline" correspondant à l'implémentation du jeu Facetious Pelican se trouve dans le répertoire ./src/facetious_pelican
(plateau de jeu, notion de position, de contrainte et d'affectation) 

Les différents solveurs correspondant aux achievements 1, 2, 3 et 4 se trouvent dans le répertoire ./src/

NOTE : Pour tester les solveurs avec un exemple prédéfini (board classique), lancer ./test_solver et ./test_solver_z3 (cf. compilation)

###############
# Compilation #
###############

Créer un répertoire build à la racine et se placer dedans
	$ mkdir build
	$ cd build

Générer les fichiers Makefile
	$ cmake ..

Lancer la compilation
        $ make install

Pour lancer les exécutables tests suivant, il faut tout d'abord se place dans le répertoire bin/tests
     	$ cd bin/tests

Ensuite, pour lancer un test (par exemple ./test_solver_cmp qui compare les resultats des differents solveurs)
       	$ ./test_solver_cmp


#################
# Documentation #
#################

Se placer dans le répertoire build.
   	$ cd build

Pour générer la documentation (~ 30 s)
	$ make doc

Puis pour la visualiser, l'ouvrir avec un navigateur web. Par exemple
     	$ firefox ./doc/html/index.html
