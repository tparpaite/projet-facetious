/**
 * \file test_solver.c
 * \brief Tests fonctionnels du solveur
 * \author PARPAITE Thibault
 * \date 06 décembre 2016
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "generate.h"
#include "solver.h"


/* Nécessaire sinon warning à la compilation */
static void affect_destroy_cast(void *p) {
  affect_t a = (affect_t) p;
  return affect_destroy(a);
}


void test_run_solver() {
  srand(time(NULL));

  /* Board creation */ 
  int board_size = 8;
  board_t board = board_create(board_size);
  position_t *pos_board = board_get_position_a(board);

  printf("[CREATION DE LA BOARD]\n");
  position_add_tag(pos_board[0], TAG_NORTH);
  position_add_tag(pos_board[0], TAG_CORNER);
  position_add_tag(pos_board[1], TAG_NORTH);
  position_add_tag(pos_board[1], TAG_CORNER);
  position_add_tag(pos_board[2], TAG_EAST);
  position_add_tag(pos_board[2], TAG_CORNER);
  position_add_tag(pos_board[3], TAG_EAST);
  position_add_tag(pos_board[4], TAG_EAST);
  position_add_tag(pos_board[4], TAG_CORNER);
  position_add_tag(pos_board[5], TAG_SOUTH);
  position_add_tag(pos_board[5], TAG_CORNER);
  position_add_tag(pos_board[6], TAG_CORNER);
  position_add_tag(pos_board[6], TAG_WEST);
  position_add_tag(pos_board[7], TAG_CORNER);
  position_add_tag(pos_board[7], TAG_WEST);
  position_add_neighbor(pos_board[0], 1);
  position_add_neighbor(pos_board[0], 7);
  position_add_neighbor(pos_board[1], 0);
  position_add_neighbor(pos_board[1], 2);
  position_add_neighbor(pos_board[2], 1);
  position_add_neighbor(pos_board[2], 3);
  position_add_neighbor(pos_board[3], 2);
  position_add_neighbor(pos_board[3], 4);
  position_add_neighbor(pos_board[4], 3);
  position_add_neighbor(pos_board[4], 5);
  position_add_neighbor(pos_board[5], 4);
  position_add_neighbor(pos_board[5], 6);
  position_add_neighbor(pos_board[6], 5);
  position_add_neighbor(pos_board[6], 7);
  position_add_neighbor(pos_board[7], 6);
  position_add_neighbor(pos_board[7], 0);

  
  /* Création des contraintes aléatoires  */
  constraint_t *constraint_a = generate_constraint_array(board_size);

  /* Initialisation et précalcul */
  custom_type_t *pos_tab = compute_position_a(board);
  custom_type_t *pos_relations[3];  
  compute_relation_a(board, pos_relations); 

  /* Affichage des contraintes */
  printf("\nContraintes :\n");
  for (int i = 0 ; i < board_size ; ++i)
    display_constraint(constraint_a[i]);
  
  /* On lance le solveur puis on parcourt la liste des solutions */
  printf("\n\n[LANCEMENT DU SOLVEUR]\n\n");
  list_t l = run_solver(board, constraint_a);
  list_begin(l);


  /* On affiche le nombre de contraintes respectées */
  affect_t valid_affect = (affect_t) list_getelement(l);
  compute_available_positions((constraint_t *) constraint_a, board_get_size(board), pos_tab, pos_relations, valid_affect);
  int score = compute_score(board, valid_affect, constraint_a, pos_relations);
    
  printf("\nUne des meilleures affectations respectant %d contraintes :\n", score);

  affect_display(board, valid_affect);
  display_graph_8(valid_affect, board_size);  
  
  /* LIBERATION DE LA MEMOIRE */

  /* On détruit notre liste d'affectations */
  list_hard_destroy(l, affect_destroy_cast);

  /* On détruit le tableau de contraintes généré */
  destroy_constraint_array(constraint_a, board_size);

  free(pos_tab);

  /* On détruit la board */
  board_destroy(board);
}


int main(void) {
  test_run_solver();
  printf("\nPASS\n");
  return EXIT_SUCCESS;}
