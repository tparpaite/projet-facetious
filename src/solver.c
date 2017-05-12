/**
 * \file solver.c
 * \brief Contains the definitions of the function used to run the solver
 * \author PARPAITE Thibault <br>
 * MENANTEAU Yoann
 * \date 02/01/2017
 */
 
#include <stdlib.h>
#include <stdio.h>
#include "list.h"
#include "solver.h"


/*********************************
 * PRIVATE STRUCTURE & FUNCTIONS *
 *********************************/


/* Nécessaire sinon warning à la compilation */
static void affect_destroy_cast(void *p) {
  affect_t a = (affect_t) p;
  return affect_destroy(a);
}


/**
 * \fn static int factorielle(int n)
 * \brief Compute the factorial for the integer n
 * \brief Complexity: O(n)
 * \param n The integer to compute
 * \return n!
 */
static int factorielle(int n) {
  if (n == 0)
    return 1;
  return n * factorielle(n - 1);
}


/**
 * \fn static void swap(int t[], int i, int j)
 * \brief Swap two elements placed in the indexes i and j of an array t
 * \brief Complexity: O(1)
 * \param t The array
 * \param i The first index
 * \param j The second index
 */
static void swap(int t[], int i, int j) {
  int tmp;
  tmp = t[i];
  t[i] = t[j];
  t[j] = tmp;
}


/**
 * \fn static void generate_permutation_bis(int *t, int n, int i, affect_t *affectation_a, int *indice_affectation_p)
 * \brief Generate all the permutations of an array t and compute all the possible affectations
 * \brief Complexity: O(n!)
 * \param t The array
 * \param n The affectation size
 * \param i The current index
 * \param affectation_a The affectation
 * \param indice_affectation_p Affectation index array
 */
static void generate_permutation_bis(int *t, int n, int i, affect_t *affectation_a, int *indice_affectation_p) {
  /* On est à la fin à la fin du tableau, on a donc une permutation valide */
  if (i == n) {
    /* Copie de la permutation dans un nouveau tableau (lié à affect) */
    int *pelican_a = malloc(n * sizeof (int));
    for (int k = 0 ; k < n ; ++k)
      pelican_a[k] = t[k];
      
    /* On stocke cette nouvelle affectation */
    affectation_a[*(indice_affectation_p)] = affect_create(n, pelican_a);
    *indice_affectation_p += 1;
  } 

  /* Sinon on explore les permutations de i à n-1 */
  else {
    for (int j = i ; j < n ; ++j) {
      /* On inverse i et j puis on regarde recursivement le reste des permutations */
      swap(t, i, j);
      generate_permutation_bis(t, n, i + 1, affectation_a, indice_affectation_p);
  
      /* On remet i et j en place */
      swap(t, i, j);
    }
  }
}


/* L'ensemble des affectations correspond à l'ensemble des arrangements
 * des entiers de 0 à board_size - 1, càd une taille de board_size! */ 
/**
 * \fn static affect_t *generate_permutation(int board_size)
 * \brief Generate all the possible affectations using generate_permutation_bis
 * \brief Complexity: O(n!)
 * \param board_size The board size
 * \return an affectation
 */
static affect_t *generate_permutation(int board_size) {
  int n_arrangements = factorielle(board_size);
  affect_t *affectation_a = malloc(n_arrangements * sizeof (affect_t));

  /* On créé le tableau des entiers de 0 à board_size - 1 */
  int *t = malloc(board_size * sizeof(int));
  for (int i = 0 ; i < board_size ; ++i)
    t[i] = i;
  
  /* On génère les permutations et on les stocke dans affectation_a */
  /* On se sert d'un pointeur qui stocke l'indice courant de affectation_a */
  int *indice_affectation_p = malloc(sizeof (int));
  *indice_affectation_p = 0;
  generate_permutation_bis(t, board_size, 0, affectation_a, indice_affectation_p);

  /* Free */
  free(indice_affectation_p);
  free(t);

  return affectation_a;
}

/**
 * \fn static void destroy_permutation(affect_t *affectation_a, int size)
 * \brief Destroy all the affectations previously generated
 * \brief Complexity: O(n!)
 * \param affectation_a The affectation array
 * \param size The array size
 */
static void destroy_permutation(affect_t *affectation_a, int size) {
  for (int i = 0 ; i < size ; ++i)
    affect_destroy(affectation_a[i]);

  free(affectation_a);
}           

 

/********************
 * PUBLIC FUNCTIONS *
 ********************/


/**
 * \fn int compute_score(const board_t b, const affect_t a, const constraint_t *constraint_a, int *pos_relations[])
 * \brief Test a generated affectation
 * \brief Complexity: O(1) if no dependance, O(n) if there are n dependences
 * \param b The board
 * \param a The affectation
 * \param constraint_a The constraints
 * \param pos_relations The possible positions for each bi-penguin constraint
 * \return the number of respected constraints
 */
int compute_score(const board_t b, const affect_t a, const constraint_t *constraint_a, custom_type_t *pos_relations[]) {
  /* Il y a autant de contraintes que de pelicans et de positions dans le tableau */
  int n_constraints = board_get_size(b);
  int score = 0;
 
  for (int i = 0 ; i < n_constraints ; i++)
    score += apply_constraint(b, a, constraint_a[i], constraint_a, pos_relations);

  return score;
}


/* BRUTEFORCE, raisonnable pour un nombre de pelicans < 10 */
/**
 * \fn list_t run_solver(const board_t b, const constraint_t *constraint_a)
 * \brief Test all the possible affectation and store the valid affectations (Brute forcing)
 * \brief ?
 * \param b The board
 * \param constraint_a The constraints
 * \param pos_relations The possible positions for each bi-penguin constraint
 * \return the valid affectations
 */
list_t run_solver(const board_t b, const constraint_t *constraint_a) {
  /* Il y a autant de contraintes que de pelicans et de positions dans le tableau */
  int n_constraints = board_get_size(b);
  int n_arrangements = factorielle(n_constraints);

  custom_type_t *pos_tab = compute_position_a(b);
  custom_type_t *pos_relations[3];  
  compute_relation_a(b, pos_relations); 
 
  /* On génère tous les arrangements possibles d'affectations de pelicans */
  affect_t *affectation_a = generate_permutation(board_get_size(b));

  /* On liste les affectations qui realisent un score maximal */
  int best_score = 0;
  list_t l = list_create();

  for (int i = 0 ; i < n_arrangements ; i++) {
    compute_available_positions((constraint_t *) constraint_a, board_get_size(b), pos_tab, pos_relations, affectation_a[i]);

    int current_score = compute_score(b, affectation_a[i], constraint_a, pos_relations);
 
    /* On nettoie la liste et on ajoute la nouvelle meilleur affectation */
    if (current_score > best_score) {
      best_score = current_score;
      list_hard_clean(l, affect_destroy_cast);
      affect_t affect_valide = affect_copy(affectation_a[i]);
      list_add(l, (void *) affect_valide);
    }

    /* On ajoute la nouvelle affectation à la liste qui est aussi bien que celle déjà présente */
    if (current_score == best_score) {
      affect_t affect_valide = affect_copy(affectation_a[i]);
      list_add(l, (void *) affect_valide);
    }
  } 
    
  free(pos_tab);
  destroy_permutation(affectation_a, n_arrangements);

  return l;
}
  
