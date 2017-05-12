/**
 * \file generate.h
 * \brief Contains the declaration of the functions for the constraint and affectation generations
 * \author PARPAITE Thibault <br>
 * MENANTEAU Yoann
 * \date 02/01/2017
 */

#ifndef _GENERATE_H
#define _GENERATE_H

#include "board.h"
#include "affect.h"
#include "constraint.h"
#include "custom_type.h"

typedef struct constraint_s *constraint_t;

// Generate a random constraint
extern constraint_t generate_constraint(int board_size);
// Generate a random constraint arrangement
extern constraint_t *generate_constraint_array(int board_size);
// Free the random constraint array
extern void destroy_constraint_array(constraint_t *constraint_a, int board_size);
// Add for each constraint, its possible positions 
extern affect_t generate_affectation(int board_size);
// Generate all possible position for each position constraint
extern custom_type_t *compute_position_a(board_t b);
// Generate all possible couple for each bi-pelican constraint
extern void compute_relation_a(const board_t b, custom_type_t *pos_relation_a[]);
// Add for each constraint, its possible positions 
extern void compute_available_positions(constraint_t *constraint_a, int board_size, custom_type_t pos_tab[], custom_type_t *pos_relations[], affect_t a);

#endif /* _GENERATE_H */
