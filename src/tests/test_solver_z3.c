/**
 * \file test_solver_z3.c
 * \brief Tests fonctionnels du solveur z3
 * \author MENANTEAU Yoann
 * \date 06 décembre 2016
 */
 
#include <stdio.h>
#include <stdlib.h>
#include "board.h"
#include "affect.h"
#include "generate.h"
#include "z3.h"
#include "solver_z3.h"


void test_run_solver_z3() {
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
  
  /* Création des contraintes  */
  constraint_t *constraint_a = malloc(sizeof(constraint_t)*8);
  enum tag *tag_a = NULL;
  tag_a = malloc(sizeof(enum tag));
  *tag_a = TAG_NORTH;
  constraint_a[0] = constraint_create(POSITION, tag_a, 1, 1, 0, false);
  tag_a = malloc(sizeof(enum tag));
  *tag_a = TAG_WEST;
  constraint_a[1] = constraint_create(POSITION, tag_a, 1, 2, 0, false);
  tag_a = malloc(sizeof(enum tag));
  *tag_a = TAG_NORTH;
  constraint_a[2] = constraint_create(POSITION, tag_a, 1, 3, 0, false);
  constraint_a[3] = constraint_create(SAME_SIDE, NULL, 0, 4, 2, false);
  constraint_a[4] = constraint_create(FACE, NULL, 0, 5, 1, false);
  constraint_a[5] = constraint_create(CORNER, NULL, 0, 6, 5, false);
  constraint_a[6] = constraint_create(SAME_SIDE, NULL, 0, 7, 6, false);
  constraint_a[7] = constraint_create(FACE, NULL, 0, 8, 2, false);

  printf("\nContraintes :\n");
  for (int i = 0 ; i < board_size ; ++i)
    display_constraint(constraint_a[i]);
	
  /* Initialisation et précalcul */
  custom_type_t * pos_tab = compute_position_a(board);
  custom_type_t *pos_relations[3];
  compute_relation_a(board, pos_relations);

  printf("\n\n[LANCEMENT DU SOLVEUR]\n\n");
  enum constraint_type constraint_type_a[board_size];	 
	
  /* Perform the solver */
  affect_t valid_affect = solver_z3(constraint_a, constraint_type_a,  board, 0, pos_relations, pos_tab);
  if (valid_affect == NULL)
    printf("No solution\n");
  else {
    printf("Affectation trouvée\n");
    affect_display(board, valid_affect);
    display_graph_8(valid_affect, board_size);  
  }
	
  /* Destroy used elements */
  destroy_constraint_array(constraint_a, board_size);	
  board_destroy(board);  
}

int main(int argc, char *argv[]){
  test_run_solver_z3();
  printf("\nPASS\n");
  return EXIT_SUCCESS;
}
