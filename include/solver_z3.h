/**
 * \file constraint.h
 * \brief Contains the z3 solver
 * \author PARPAITE Thibault <br>
 * MENANTEAU Yoann
 * \date 02/01/2017
 */

#ifndef _SOLVER_Z3_H
#define _SOLVER_Z3_H

#include "board.h" 
#include "affect.h"
#include "list.h"
#include "generate.h"

extern affect_t solver_z3(constraint_t *constraint_a, enum constraint_type constraint_type_a[], const board_t b, int indice, custom_type_t *bi_penguin_relation_a[], custom_type_t mono_pinguin_relation_a[]);

#endif
