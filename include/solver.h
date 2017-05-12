/**
 * \file solver.h
 * \brief Contains the declaration of the function used to run the solver
 * \author PARPAITE Thibault <br>
 * MENANTEAU Yoann
 * \date 02/01/2017
 */

#ifndef _SOLVER_H
#define _SOLVER_H

#include "board.h"
#include "affect.h"
#include "constraint.h"
#include "generate.h"
#include "list.h"

// Test all the possible affectation and store the valid affectations (Brute forcing)
extern list_t run_solver(const board_t b, const constraint_t *constraint_a);
extern int compute_score(const board_t b, const affect_t a, const constraint_t *constraint_a, custom_type_t *pos_relations[]);


#endif /* _SOLVER_H */
