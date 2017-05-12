/**
 * \file constraint.h
 * \brief Contains the declaration of the functions used to manage the constraints
 * \author PARPAITE Thibault <br>
 * MENANTEAU Yoann
 * \date 02/01/2017
 */

#ifndef _CONSTRAINT_H
#define _CONSTRAINT_H

#include "board.h"
#include "affect.h"
#include "list.h"
#include "generate.h"

/**
 * \enum constraint_type
 * \brief constraints type constants
 * Set of constants used to define the type of a constraint
 */
enum constraint_type { FACE, SAME_SIDE, CORNER, POSITION, SAME_CONSTRAINT, OPPOSITE_CONSTRAINT, NO_CONSTRAINT };
typedef struct custom_type_s *custom_type_t;
typedef struct constraint_s *constraint_t;

#define OUTPUT_SIZE 10000

/* CONSTRUCTEURS et ACCESSEURS */

extern constraint_t constraint_create(enum constraint_type type, enum tag *location_tag_a,  int size, int p1, int p2, bool negation);
extern void constraint_destroy(constraint_t c);
extern void board_add_tag(board_t b, unsigned int position, enum tag t);
extern bool get_constraint_opposite(constraint_t c);
extern enum tag * get_constraint_location_tag_a(constraint_t c);
extern int get_constraint_tag_size(constraint_t c);
extern int get_constraint_pelican1(constraint_t c);
extern int get_constraint_pelican2(constraint_t c);
extern custom_type_t get_constraint_positions(constraint_t c);
extern enum constraint_type get_constraint_type(constraint_t c);
extern void set_constraint_type(constraint_t c, enum constraint_type t);
extern void constraint_set_positions(constraint_t c, custom_type_t positions);

/* FUNCTIONS */

extern void display_constraint(const constraint_t c);
extern bool apply_constraint(const board_t b, const affect_t a, const constraint_t c, const constraint_t *constraint_a, custom_type_t *bi_penguin_relation_a[]);
extern bool apply_constraint_rec(const board_t b, const affect_t a, int indice, constraint_t constraint_a[], custom_type_t *bi_penguin_relation_a[]);
extern affect_t apply_constraint_z3(const board_t b, constraint_t *constraint_a, affect_t a, custom_type_t *bi_penguin_relation_a[], custom_type_t mono_pinguin_relation_a[], bool placement);
extern bool constraint_face(const board_t b, int position_p1, int position_p2);
extern bool constraint_same_side(const board_t b, int position_p1, int position_p2);
extern bool constraint_position(const board_t b, int position, enum tag *location_tag_a, int size);
extern bool constraint_corner(const board_t b, int position_p1, int position_p2);
extern bool treat_dependence(constraint_t c1, const constraint_t constraint_a[], int affectation_size, bool treated_pelican[], custom_type_t *pos_relations[], affect_t a);

#endif /* _CONSTRAINT_H */
