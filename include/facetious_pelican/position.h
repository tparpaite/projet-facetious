/**
 * \file position.h
 * \brief Contains the declaration of the functions used to manage the position structure
 * \author PARPAITE Thibault <br>
 * MENANTEAU Yoann
 * \date 02/01/2017
 */
 
#ifndef _POSITION_H
#define _POSITION_H

#include "list.h"

#define TAG_SIZE 20

typedef struct position_s *position_t;

/**
 * \enum tag
 * \brief tags constants
 * Tag is a set of constants used to define a position
 */
enum tag {
  TAG_NORTH = 0, 
  TAG_SOUTH = 1,
  TAG_CORNER = 3,     /* Situated in a part of the universe with a bend */
  TAG_EAST = 4, 
  TAG_WEST = 5,
  TAG_FAR = 6,         /* Situated in the external part of the graph */  
  TAG_BAGPIPE = 7,    /* Situated near Scotland */  
  NONE = 8
};

/* CONSTRUCTEURS et ACCESSEURS */

extern position_t position_create();
extern void position_destroy(position_t p);
extern void position_add_tag(position_t p, enum tag t);
extern void position_add_neighbor(position_t p, int neighbor_id);
extern list_t position_get_list_tags(position_t p);
extern list_t position_get_neighbors_id(position_t p);
extern void position_display_tags(position_t p);

#endif /* _POSITION_H */
