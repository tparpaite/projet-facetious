/**
 * \file test_solver_cmp.c
 * \brief Tests fonctionnels du solveur
 * \author PARPAITE Thibault
 * \date 06 décembre 2016
 */

#include <stdio.h>
#include <stdlib.h>
#include "board.h"
#include "affect.h"
#include "generate.h"
#include "z3.h"
#include <time.h>
#include "solver_z3.h"
#include "solver.h"
#include "constraint.h"
#include "position.h"

constraint_t * get_jeu_de_test_board_8(int test_number);


bool test_run_solver_cmp(constraint_t *constraint_a) {  
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

  /* Constraints generation */
  if (constraint_a == NULL)
    constraint_a = generate_constraint_array(board_size);

  custom_type_t *pos_tab = compute_position_a(board);;
  custom_type_t *pos_relations[3];  
  compute_relation_a(board, pos_relations);
  enum constraint_type constraint_type_a[board_size];	 
	
  /* Perform the solvers */
  printf("Contraintes :\n");
  for (int i = 0 ; i < board_size ; i++)
    display_constraint(constraint_a[i]);

   printf("\n[TEST SOLVER Z3]\n");
   affect_t valid_affect = solver_z3(constraint_a, constraint_type_a, board, 0, pos_relations, pos_tab);  
  if (valid_affect == NULL){
    printf("No solution\n");
    return false; // Il y a toujours une solution c'est donc une erreur
  }
  else {
    int *affect_a = affect_get_pelican_a(valid_affect);
    printf("\n\nAffectation issue de Z3 : \n");
    for (int i = 0; i < board_size; i++){
      printf("Pelican %d position %d\n", i+1, affect_a[i]);
    }
    printf("\n[TEST SOLVEUR VERSION 1]\n");
    list_t l = run_solver(board, constraint_a);
    printf("Comparaison des resultats\n");
    list_begin(l);
    while(!list_isend(l)){      
      affect_t a = (affect_t) list_getelement(l);
      int *affect_b = affect_get_pelican_a(a);
      bool same = true;
      for (int i = 0; i < board_size; i++){
	if (affect_a[i] != affect_b[i])
	  same = false;
      }
      if (same) {
	printf("[MATCHED] Concordance trouvee, les resultats du SOLVEUR V1 et SOLVEUR Z3 sont identiques\n\n");
	display_graph_8(valid_affect, board_size);
	return true;
      }
      list_next(l);
    }
  }
  printf("Pas de concordance\n");
	
  /* Destroy used elements */
  destroy_constraint_array(constraint_a, board_size);	
  board_destroy(board);
  return false;
}

int main(int argc, char *argv[]){
  srand(time(NULL));
  for (int i = 0; i < 7 ; i++)
    printf("%s", (test_run_solver_cmp(get_jeu_de_test_board_8(i)))?"PASS\n\n":"FAILED\n");
  
  return EXIT_SUCCESS;
}


constraint_t * get_jeu_de_test_board_8(int test_number){
  constraint_t *constraint_a = malloc(sizeof(constraint_t)*8);
  test_number = test_number%7;
  enum tag *tag_a = NULL;
  switch(test_number){
  case 0:
    //TEST 1 valid
    constraint_a[0] = constraint_create(SAME_SIDE, NULL, 0, 1, 2, false);
    constraint_a[1] = constraint_create(CORNER, NULL, 0, 2, 3, false);
    constraint_a[2] = constraint_create(SAME_SIDE, NULL, 0, 3, 4, false);
    constraint_a[3] = constraint_create(SAME_SIDE, NULL, 0, 4, 5, false);
    constraint_a[4] = constraint_create(CORNER, NULL, 0, 5, 6, false);
    constraint_a[5] = constraint_create(CORNER, NULL, 0, 6, 7, false);
    constraint_a[6] = constraint_create(SAME_SIDE, NULL, 0, 7, 8, false);
    constraint_a[7] = constraint_create(CORNER, NULL, 0, 8, 1, false);
    break;
  case 1:
    //TEST 2 valid
    constraint_a[0] = constraint_create(FACE, NULL, 0, 1, 5, false);
    constraint_a[1] = constraint_create(FACE, NULL, 0, 2, 5, false);
    constraint_a[2] = constraint_create(FACE, NULL, 0, 3, 8, false);
    constraint_a[3] = constraint_create(FACE, NULL, 0, 4, 8, false);
    constraint_a[4] = constraint_create(FACE, NULL, 0, 5, 7, false);
    constraint_a[5] = constraint_create(FACE, NULL, 0, 6, 1, false);
    constraint_a[6] = constraint_create(FACE, NULL, 0, 7, 5, false);
    constraint_a[7] = constraint_create(FACE, NULL, 0, 8, 3, false);
    break;
  case 2:
    //TEST 3 valid    
    tag_a = malloc(sizeof(enum tag));
    *tag_a = TAG_NORTH;
    constraint_a[0] = constraint_create(POSITION, tag_a, 1, 1, 0, false);
    tag_a = malloc(sizeof(enum tag));
    *tag_a = TAG_NORTH;
    constraint_a[1] = constraint_create(POSITION, tag_a, 1, 2, 0, false);
    tag_a = malloc(sizeof(enum tag));
    *tag_a = TAG_EAST;
    constraint_a[2] = constraint_create(POSITION, tag_a, 1, 3, 0, false);
    tag_a = malloc(sizeof(enum tag));
    *tag_a = TAG_EAST;
    constraint_a[3] = constraint_create(POSITION, tag_a, 1, 4, 0, false);
    tag_a = malloc(sizeof(enum tag));
    *tag_a = TAG_EAST;
    constraint_a[4] = constraint_create(POSITION, tag_a, 1, 5, 0, false);
    tag_a = malloc(sizeof(enum tag));
    *tag_a = TAG_SOUTH;
    constraint_a[5] = constraint_create(POSITION, tag_a, 1, 6, 0, false);
    tag_a = malloc(sizeof(enum tag));
    *tag_a = TAG_WEST;
    constraint_a[6] = constraint_create(POSITION, tag_a, 1, 7, 0, false);
    tag_a = malloc(sizeof(enum tag));
    *tag_a = TAG_WEST;
    constraint_a[7] = constraint_create(POSITION, tag_a, 1, 8, 0, false);
    break;
  case 3:
    //TEST 4 invalid 3 pelicans want to be on NORTH, at least one constraint removed
    tag_a = malloc(sizeof(enum tag));
    *tag_a = TAG_NORTH;
    constraint_a[0] = constraint_create(POSITION, tag_a, 1, 1, 0, false);
    tag_a = malloc(sizeof(enum tag));
    *tag_a = TAG_NORTH;
    constraint_a[1] = constraint_create(POSITION, tag_a, 1, 2, 0, false);
    tag_a = malloc(sizeof(enum tag));
    *tag_a = TAG_NORTH;
    constraint_a[2] = constraint_create(POSITION, tag_a, 1, 3, 0, false);
    constraint_a[3] = constraint_create(SAME_SIDE, NULL, 0, 4, 2, false);
    constraint_a[4] = constraint_create(SAME_SIDE, NULL, 0, 5, 2, false);
    constraint_a[5] = constraint_create(SAME_SIDE, NULL, 0, 6, 2, false);
    constraint_a[6] = constraint_create(SAME_SIDE, NULL, 0, 7, 2, false);
    constraint_a[7] = constraint_create(FACE, NULL, 0, 8, 2, false);
    break;
  case 4:
    //TEST 5 invalid 4 pelicans want face the same pelican, at least one constraint removed
    constraint_a[0] = constraint_create(FACE, NULL, 0, 1, 5, false);
    constraint_a[1] = constraint_create(FACE, NULL, 0, 2, 5, false);
    constraint_a[2] = constraint_create(FACE, NULL, 0, 3, 5, false);
    constraint_a[3] = constraint_create(FACE, NULL, 0, 4, 5, false);
    constraint_a[4] = constraint_create(SAME_SIDE, NULL, 0, 5, 2, false);
    constraint_a[5] = constraint_create(SAME_SIDE, NULL, 0, 6, 2, false);
    constraint_a[6] = constraint_create(SAME_SIDE, NULL, 0, 7, 2, false);
    constraint_a[7] = constraint_create(SAME_SIDE, NULL, 0, 8, 2, false);
    break;
  case 5:
    //TEST 6 invalid each pelicans want share a corner, at least one constraint removed
    constraint_a[0] = constraint_create(CORNER, NULL, 0, 1, 2, false);
    constraint_a[1] = constraint_create(CORNER, NULL, 0, 2, 3, false);
    constraint_a[2] = constraint_create(CORNER, NULL, 0, 3, 4, false);
    constraint_a[3] = constraint_create(CORNER, NULL, 0, 4, 5, false);
    constraint_a[4] = constraint_create(CORNER, NULL, 0, 5, 6, false);
    constraint_a[5] = constraint_create(CORNER, NULL, 0, 6, 7, false);
    constraint_a[6] = constraint_create(CORNER, NULL, 0, 7, 8, false);
    constraint_a[7] = constraint_create(CORNER, NULL, 0, 8, 1, false);
    break;
  case 6:
    // TEST 7 valid good case
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
  }
  return constraint_a;
}
