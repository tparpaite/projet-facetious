/**
 * \file position.c
 * \brief Contains the definitions of the functions used to manage the position structure
 * \author PARPAITE Thibault <br>
 * MENANTEAU Yoann
 * \date 02/01/2017
 */

#include <stdio.h>
#include <stdlib.h> 
#include "position.h"


/*********************
 * PRIVATE STRUCTURE *
 *********************/

/**
 * \struct position_s
 * \brief Definition of a position
 *
 * On choisit d'utiliser des listes car le nombre de tags 
 * et de voisins est différent pour chaque position
 * Si on avait utilisé des tableaux, on aurait de la 
 * mémoire allouée inutilement
 */
struct position_s {
  list_t list_tags;
  list_t list_neighbors_id;
};


static enum tag list_getelement_tag(list_t l) {
  enum tag *t = (enum tag *) list_getelement(l);
  return *t;
}


static char *string_from_tag(enum tag t) {
  static char *strings[] = {  "TAG_NORTH", "TAG_SOUTH", "TAG_NORTH_SOUTH", "TAG_CORNER", "TAG_EAST", "TAG_WEST", "TAG_FAR", "TAG_BAGPIPE", "NONE"};
  return strings[t];
}


/********************
 * PUBLIC FUNCTIONS *
 ********************/

/* CONSTRUCTEURS et ACCESSEURS */

/**
 * \fn position_t position_create(void)
 * \brief Create a new position 
 * \brief Complexity: O(1)
 * \return A new position
 */
position_t position_create(void) {
  position_t p = malloc(sizeof (struct position_s));

  p->list_tags = list_create();
  p->list_neighbors_id = list_create();

  return p;
}


/**
 * \fn void position_destroy(position_t p)
 * \brief Destroy a position 
 * \brief Complexity: O(n) where n is the size of the neighbours list
 * \param p The position
 */
void position_destroy(position_t p) {
  list_destroy(p->list_tags);
  list_destroy(p->list_neighbors_id);
  free(p);
} 


/**
 * \fn void position_add_tag(position_t p, enum tag t)
 * \brief Add a tag t to a position p
 * \brief Complexity: O(1) 
 * \param p The position
 * \param t The tag
 */
void position_add_tag(position_t p, enum tag t) {
  /* On alloue un espace mémoire propre à l'élément */
  enum tag *element = malloc(sizeof (enum tag));
  *element = t;
  /* Puis on l'ajoute à la liste */
  list_add(p->list_tags, (void *) element);
}


/**
 * \fn void position_add_neighbor(position_t p, int neighbor_id)
 * \brief Add a neighbour with the ID neighbor_id to a position p
 * \brief Complexity: O(1) 
 * \param p The position
 * \param neighbor_id The neighbour ID
 */
void position_add_neighbor(position_t p, int neighbor_id) {
  /* On alloue un espace mémoire propre à l'élément */
  /* Nécessaire car liste générique */
  int *element = malloc(sizeof (int));
  *element = neighbor_id;

  /* Puis on l'ajoute à la liste */
  list_add(p->list_neighbors_id, (void *) element);
}


/**
 * \fn list_t position_get_list_tags(position_t p)
 * \brief Return the tags for the position p
 * \brief Complexity: O(1) 
 * \param p The position
 * \return A tag list
 */
list_t position_get_list_tags(position_t p) {
  return p->list_tags;
}


/**
 * \fn list_t position_get_neighbors_id(position_t p)
 * \brief Return the neighbours for the position p
 * \brief Complexity: O(1) 
 * \param p The position
 * \return A neighbours list
 */
list_t position_get_neighbors_id(position_t p) {
  return p->list_neighbors_id;
}


void position_display_tags(position_t p) {
  list_t l = p->list_tags;
  list_begin(l);

  while (!list_isend(l)) {
    enum tag t = list_getelement_tag(l);
    printf("%s ", string_from_tag(t));
    list_next(l);
  }
}

      
  
