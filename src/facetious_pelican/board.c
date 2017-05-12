/**
 * \file board.c
 * \brief Contains the definitions of the functions used to manage the board
 * \author PARPAITE Thibault <br>
 * MENANTEAU Yoann
 * \date 02/01/2017
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include "board.h"

/*********************************
 * PRIVATE STRUCTURE & FUNCTIONS *
 *********************************/

/**
 * \struct board_s
 * \brief Definition of the board
 *
 * board contains the available positions on the board
 */
struct board_s {
  int size;
  position_t *position_a;
};


/**
 * \fn static void queue_push_int(queue_t q, int i)
 * \brief Push a new int in a queue
 * \brief Complexity: O(1)
 * \param q the queue
 * \param i the int
 */
static void queue_push_int(queue_t q, int i) {
  int *p = malloc(sizeof (int));
  *p = i;
  queue_push(q, (void *) p);
}


/**
 * \fn static int queue_front_int(queue_t q)
 * \brief Get the front element of type int in a queue
 * \brief Complexity: O(1)
 * \param q the queue
 * \return the int
 */
static int queue_front_int(queue_t q) {
  int *p = (int *) queue_front(q);
  return *p;
}


/**
 * \fn static int list_getelement_int(list_t l)
 * \brief Get an int from a list
 * \brief Complexity: O(1)
 * \param l the list
 * \return the int
 */
static int list_getelement_int(list_t l) {
  int *p = (int *) list_getelement(l);
  return *p;
}


/**
 * \fn static enum tag list_getelement_enumtag(list_t l)
 * \brief Get a tag from a list
 * \brief Complexity: O(1)
 * \param l the list
 * \return the tag
 */
static enum tag list_getelement_enumtag(list_t l) {
  enum tag *element = (enum tag *) list_getelement(l);
  return *element;
}


/********************
 * PUBLIC FUNCTIONS *
 ********************/

/* CONSTRUCTEURS et ACCESSEURS */

/**
 * \fn board_t board_create(int size)
 * \brief Create a new board
 * \brief Complexity: O(n) where n = the size
 * \param board_size the size
 * \return the board
 */
board_t board_create(int board_size) {
  board_t b = malloc(sizeof (struct board_s));
  b->size = board_size;

  // We create a position array
  b->position_a = malloc(board_size * sizeof (position_t));

  // We initialise each position
  for (int i = 0 ; i < board_size ; ++i)
    b->position_a[i] = position_create(i);

  return b;
}


/**
 * \fn void board_destroy(board_t b)
 * \brief Destroy a board
 * \brief Complexity: n(O)
 * \param b the board
 */
void board_destroy(board_t b) {
  for (int i = 0 ; i < b->size ; ++i)
    position_destroy(b->position_a[i]);
  
  free(b->position_a);
  free(b);
}


/**
 * \fn unsigned int board_get_size(const board_t b)
 * \brief return the board size
 * \brief Complexity: O(1)
 * \param b the board
 * \return the size
 */
unsigned int board_get_size(const board_t b) {
  return b->size;
}


/**
 * \fn position_t *board_get_position_a(const board_t b)
 * \brief return a position array
 * \brief Complexity: O(1)
 * \param b the board
 * \return a position array
 */
position_t *board_get_position_a(const board_t b) {
  return b->position_a;
}


/* FUNCTIONS */

bool is_neighboor(const board_t b, unsigned int x, unsigned int y) {
  list_t l = position_get_neighbors_id(b->position_a[x]);
  list_begin(l);
  while (!list_isend(l)) {
    if (y == list_getelement_int(l))
      return true;
    list_next(l);
  }
  return false;
}
 

/*
  Uses BFS (Bread First Search, Parcours en Profondeur) algorithm O(n + m) 
  where n = vertex quantity and m = edge quantity,
  malloc O(l), memset O(l) where l = board size
  free() O(1), queue_create() O(1), queue_push_int() O(1), 
  queue_isempty() O(1), queue_pop() O(1), list_begin() O(1),
  list_getelement_int() O(1), list_next() O(1)
  Complexity O(n + m + l)
*/
 
/* We use the BFS algorithm to return the minimal distance between two edges */
/**
 * \fn unsigned int distance(const board_t b, unsigned int x, unsigned int y)
 * \brief Compute the distance between two positions, return UINT_MAX if problem
 * \param b the board
 * \param x coordinate x
 * \param y coordinate y
 * \return The distance between the two positions.
 */
unsigned int distance(const board_t b, unsigned int x, unsigned int y) {
  /* The distance of an edge with itself is 0 */
  if (x == y)
    return 0;

  /* We use a boolean array to mark the position */
  bool *marquage_a = malloc(b->size * sizeof (bool));
  memset(marquage_a, 0, b->size * sizeof (bool));

  /* We push the first edge, then we mark */
  queue_t q = queue_create();
  queue_push_int(q, x);
  marquage_a[x] = 1;

  int d = 0;
  int pos_id, voisin_id;
  list_t l;

  while (!queue_isempty(q)) {
    pos_id = queue_front_int(q);
    queue_pop(q);
    d++;

    /* We browse the neighbours */
    l = position_get_neighbors_id(b->position_a[pos_id]);
    list_begin(l);

    while (!list_isend(l)) {
      voisin_id = list_getelement_int(l);
     
      /* When arrived at destination we stop so */
      if (voisin_id == y)
        return d;

      /* We don't mark the same edge twice */
      if (!marquage_a[voisin_id]) {
        queue_push_int(q, voisin_id);
        marquage_a[voisin_id] = 1;
      }
      list_next(l);
    }
  }
  free(marquage_a);

  /* In case of error, we return an error code, any graph problem ? */
  return UINT_MAX;
}


/**
 * \fn bool has_tag(const board_t b, unsigned int position_id, enum tag tag)
 * \brief Checks if a position possesses the corresponding tag
 * \brief Complexity: O(n) where n is the list size
 * \param b the board
 * \param position_id a position id
 * \param tag a tag
 * \return a boolean
 */
bool has_tag(const board_t b, unsigned int position_id, enum tag tag) {
  position_t p = b->position_a[position_id];
  list_t l = position_get_list_tags(p);
 
  /* We browse the list for the given tag */
  enum tag current_tag;
  list_begin(l);

  while (!list_isend(l)) {
    current_tag = list_getelement_enumtag(l);
    if (current_tag == tag) {
      return true;
    }
    list_next(l);
  }    

  return false;
}
