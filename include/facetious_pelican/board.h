/**
 * \file board.h
 * \brief Contains the declaration of the functions used to manage the board
 * \author PARPAITE Thibault <br>
 * MENANTEAU Yoann
 * \date 02/01/2017
 */
 
#ifndef _BOARD_H
#define _BOARD_H

#include <stdbool.h>
#include "position.h"
#include "queue.h"

#define NO_COLOR 0 // A very particular color that no bird could ever possess

typedef struct board_s *board_t;

/* CONSTRUCTEURS et ACCESSEURS */

extern board_t board_create(int board_size);
extern board_t board_from_file(char filepath[]);
extern void board_destroy(board_t b);
extern unsigned int board_get_size(const board_t b);
extern position_t *board_get_position_a(const board_t b);


/* FUNCTIONS */

// Compute the distance between two positions, return UINT_MAX if problem
extern unsigned int distance(const board_t b, unsigned int x, unsigned int y);
extern bool has_tag(const board_t b, unsigned int position, enum tag tag);
extern bool is_neighboor(const board_t b, unsigned int x, unsigned int y);

#endif /* _BOARD_H */
