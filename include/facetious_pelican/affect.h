/**
 * \file affect.h
 * \brief Contains the declaration of the functions used to manage the affectation
 * \author PARPAITE Thibault <br>
 * MENANTEAU Yoann
 * \date 02/01/2017
 */

#ifndef _AFFECT_H
#define _AFFECT_H

#include "board.h"

typedef struct affect_s *affect_t;

/* CONSTRUCTEURS et ACCESSEURS */

extern affect_t affect_create(int board_size, int *position_a);
extern void affect_destroy(affect_t a);
extern int affect_get_size(const affect_t a);
extern int *affect_get_pelican_a(const affect_t a);
extern affect_t affect_copy(const affect_t a);
extern void affect_display(board_t b, const affect_t a);
extern void display_graph_16(affect_t a, int board_size);
extern void display_graph_8(affect_t a, int board_size);

#endif /* _AFFECT_H */
