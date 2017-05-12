/**
 * \file z3.h
 * \brief Contains the declaration of the functions used to generate a z3 script
 * \author PARPAITE Thibault <br>
 * MENANTEAU Yoann
 * \date 02/01/2017
 */
 
#ifndef _Z3_H
#define _Z3_H

#include "board.h"
#include "constraint.h"

typedef struct constraint_s *constraint_t;

/* FUNCTIONS */

// Initialize the z3 formula with the conditions "each pelican has one place and has to be placed somewhere"
extern void init_z3_formula(int board_size, FILE *script_file);

// Generate the statements xFACEy,xNEXTy,xCORNERy 
extern void generate_z3_fcs_constraints(int i, int j, custom_type_t * bi_pel_relation_a, int board_size, bool negation, FILE *script_file);

// Generate the script part for the position constraints
extern void generate_z3_position_constraints(int board_size, constraint_t constraint, FILE *script_file, custom_type_t pos_tab[]);

// Add each bird position into the z3 script
extern void z3_place_affectation(affect_t affectation, int affectation_size, FILE *res);

// Simply add false if there is an infinite cycle 
extern void z3_contradiction(FILE *res);

// Display a z3 model from a string
extern affect_t get_z3_affect(char model[], int board_size);

// Generate the z3 script
extern void generate_z3_script(int affectation_size, constraint_t *constraint_a, bool placement, affect_t a, custom_type_t *bi_penguin_relation_a[], custom_type_t mono_penguin_relation_a[]);

// Launch z3 and store the output into a string
extern void get_z3_output(char content[]);

#endif /* _Z3_H */
