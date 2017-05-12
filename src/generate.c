/**
 * \file generate.c
 * \brief Contains the definition of the functions for the constraints and affectations generation
 * \author PARPAITE Thibault <br>
 * MENANTEAU Yoann
 * \date 02/01/2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <math.h>
#include "generate.h"

#define BI_PELICAN_CONSTRAINT_SIZE 3
#define POSITION_TAG_SIZE 8
#define NORTH_SOUTH 2
#define FAILURE -1


/*********************
 * PRIVATE FUNCTIONS *
 *********************/

/**
 * \fn int get_position(int n, bool t[])
 * \brief Get the n th available position
 * \brief Complexity: O(n) where n = board size
 * \param pos_occupied an element containing whether or not a position is available
 * \param board_size the size of the board
 * \param random_value a random_value
 * \return the number of the n th available position or -1 if error
 */
static int get_position(custom_type_t pos_occupied, int board_size, int random_value) {
  int position;
  for (position = 0 ; (random_value+1) != 0 ; position++) {
    if (custom_type_get_bit(pos_occupied, position))
      random_value--;
  }
  return position-1;
}


/**
 * \fn static int *generate_position(int affect_size)
 * \brief Generate a random position array
 * \brief Complexity: O(n) where n = affect size
 * \param affect_size the affectation size
 * \return a random position array
 */
static int *generate_position(int affect_size) {
  int *res_a = malloc(affect_size * sizeof(int));
  int random_value, pos;
  
  //unsigned int pos_occupied = -1; // tout à 1
  custom_type_t pos_occupied = custom_type_create(affect_size);
  memset(custom_type_get_addr(pos_occupied), -1,custom_type_get_size(pos_occupied)/2+1);

  // For each pelican
  for (int i = 0 ; i < affect_size ; ++i) {
    random_value = rand()%(affect_size - i);
    // We get a random position
    pos = get_position(pos_occupied, affect_size, random_value);
    custom_type_set_bit(pos_occupied, pos, false);
    res_a[i] = pos;
  }
  // We return a random position array
  return res_a;
}


/********************
 * PUBLIC FUNCTIONS *
 ********************/

/**
 * \fn int * generate_position_a(int board_size, board_t b)
 * \brief Generate all possible position for each position constraint
 * \brief Complexity: polynomial to verify
 * \param board the board
 * \return an array containing each corresponding position for each position tag
 */
custom_type_t *compute_position_a(board_t board){
  int board_size = board_get_size(board);
  // 0=N 1=S 2=S|N 3=C 4=E 5=W 6=F 7=B
  custom_type_t *pos_tab = malloc(sizeof(custom_type_t)*POSITION_TAG_SIZE);
  // For each position tag
  for (int i = 0; i < POSITION_TAG_SIZE; ++i){
    pos_tab[i] = custom_type_create(board_size);
    // We test each position
    for (int j = 0; i!= NORTH_SOUTH && j < board_size; ++j){
      // And store each corresponding position
      if (has_tag(board, j, i))
	custom_type_set_bit(pos_tab[i], j, true);
    }
  }
  // (The index 2 is used for NORTH or SOUTH)
  custom_type_or(pos_tab[NORTH_SOUTH], pos_tab[TAG_NORTH]);
  custom_type_or(pos_tab[NORTH_SOUTH], pos_tab[TAG_SOUTH]);
  // We return an array containing each corresponding position for each position tag
  return pos_tab;
}


/**
 * \fn void generate_relation_a(int board_size, int *pos_relations[], board_t b)
 * \brief Generate all possible couple for each bi-pelican constraint
 * \brief Complexity: polynomial
 * \param board the board
 * \param pos_relation_a the array to store the couples
 */
void compute_relation_a(const board_t board, custom_type_t *pos_relation_a[]){
  int board_size = board_get_size(board);

  // For each constraint type ...
  for (int i = 0; i < BI_PELICAN_CONSTRAINT_SIZE; ++i){
    pos_relation_a[i] = malloc(sizeof(custom_type_t)*board_size);
    // And for each position ...
    for (int j = 0; j < board_size; ++j){
      pos_relation_a[i][j] = custom_type_create(board_size);
      // We test each position couple to determine a relation array
      for (int k = 0; k < board_size; ++k){
	switch(i) {
	case FACE:
	  if (constraint_face(board, j, k))
	    custom_type_set_bit(pos_relation_a[i][j],k, true);
	  break;
	case SAME_SIDE:
	  if (constraint_same_side(board, j, k))
	    custom_type_set_bit(pos_relation_a[i][j],k, true);
	  break;
	case CORNER:
	  if (constraint_corner(board, j, k))
	    custom_type_set_bit(pos_relation_a[i][j],k, true);
	  break;
	}
      }
    }
  }
}


/**
 * \fn void generate_available_positions(constraint_t *constraint_a, int board_size, int pos_tab[], int *pos_relations[], affect_t a)
 * \brief Add for each constraint, its possible positions 
 * \brief Complexity: O(n) where n = board size
 * \param constraint_a the random constraint array
 * \param constraint_size the constraint quantity
 * \param pos_a contains each possible position for each constraint of type POSITION
 * \param pos_relation_a contains each possible position for each constraint of types FACE, SAME_SIDE and CORNER
 * \param affectation contains the pelican positions
 */
void compute_available_positions(constraint_t *constraint_a, int constraint_size, custom_type_t pos_a[], custom_type_t *pos_relation_a[], affect_t affectation){
  int tag_size, p2;
  custom_type_t positions;
  enum tag *tag_a;
  enum constraint_type tag_type;
  int *pelican_a = affect_get_pelican_a(affectation);
  
  // For each constraint
  for (int i = 0; i < constraint_size; ++i){    
    tag_type = get_constraint_type(constraint_a[i]);    
    p2 = get_constraint_pelican2(constraint_a[i]);
    positions = custom_type_create(constraint_size); //(each bit of position represent a possible position)
    // Depending of the type
    switch(tag_type) {
    case POSITION:
      tag_size = get_constraint_tag_size(constraint_a[i]);
      tag_a = get_constraint_location_tag_a(constraint_a[i]);
      for (int j = 0; j < tag_size; ++j){
	custom_type_or(positions, pos_a[tag_a[j]]);
      }
      break;
    default:
      if (tag_type <= CORNER)
	custom_type_copy(positions, pos_relation_a[tag_type][pelican_a[p2-1]]);
      break;
    }
    // We generate an array of available position
    constraint_set_positions(constraint_a[i], positions);
  }
}


/**
 * \fn constraint_t *generate_constraint(int p, struct constraint *c1)
 * \brief Generate a random constraint for each pelican
 * \brief Complexity: O(n) where n = board size
 * \param board_size the board size
 * \return a random constraint array (each index is related to a pelican color)
 */
constraint_t *generate_constraint_array(int board_size) {
  int random_value, tag_size, p2;
  constraint_t *constraint_a = malloc(board_size * sizeof (constraint_t));
  enum constraint_type type = NO_CONSTRAINT;
  enum tag *tag_a;
  bool opposite = false;
  int rand_type;

  /* There is one constraint per pelican and each pelican has one position
   * So, we generate n constraints where n = board size */
  for (int p1 = 0 ; p1 < board_size ; ++p1) {
    tag_size = 1;
    
    rand_type = rand()%(2);
    if (rand_type) { 
      /* The constraint type is POSITION and we determin which position */	   
      if (board_size == 4) {
        random_value = rand()%5;
	if (random_value == 4)
	  random_value = 7;
      }
      else if (board_size == 16)
	random_value = rand()%7;
      else if (board_size == 8)
	random_value = rand()%6;

      switch(random_value) {
      case 2:
	tag_size = 2;
	tag_a = malloc(2 * sizeof(enum tag));
	tag_a[0] = TAG_NORTH;
	tag_a[1] = TAG_SOUTH;
	break;
	
      default:
	tag_a = malloc(sizeof(enum tag));
	tag_a[0] = random_value;
	break;
      }
      type = POSITION;
      p2 = NO_COLOR;    
    } 
    else { 
      p2 = rand()%(board_size - 2)+1;
      if (p2 == p1)
        p2++;
      /* If not, we determine an other constraint type */
      tag_a = NULL;
      random_value = rand()%6;
      switch(random_value){
      case 3:
	type = SAME_CONSTRAINT;
	break;
      case 4:
	type = OPPOSITE_CONSTRAINT;
	opposite = true;
	break;
      default:
	type = random_value;	
	break;
      }
    }   

    /* Finally the constraint is created */
    constraint_a[p1] = constraint_create(type, tag_a, tag_size, p1+1, p2, opposite);
  }
  return constraint_a;
}


/**
 * \fn void destroy_constraint_array(constraint_t *constraint_a, int board_size)
 * \brief Destroy each constraint in the constraint array generated before
 * \brief Complexity: O(n) where n = the array size
 * \param constraint_a, the constraint array
 * \param board_size
 * \return void
 */
void destroy_constraint_array(constraint_t *constraint_a, int board_size) {
  for (int i = 0 ; i < board_size ; ++i)
    constraint_destroy(constraint_a[i]);

  free(constraint_a);
}


/**
 * \fn affect_t generate_affectation(int affectation_size)
 * \brief Generate a random affectation
 * \brief Complexity: O(n) where n is the affectation size
 * \param affectation_size the affectation size
 * \return a random affectation
 */
affect_t generate_affectation(int affectation_size) {
  int *position_a = generate_position(affectation_size);
  return affect_create(affectation_size, position_a);
}
