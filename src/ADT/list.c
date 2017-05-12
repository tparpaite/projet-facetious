/**
 * \file list.c
 * \brief Contains the definitions of the functions used to create lists
 * \author PARPAITE Thibault <br>
 * MENANTEAU Yoann
 * \date 02/01/2017
 */

#include <stdlib.h>
#include <stdio.h>
#include "list.h"

/* PRIVATE STRUCTURES */

typedef struct node_s *node_t;

/**
 * \struct node_s
 * \brief a node used by the list
 *
 * Contains an element and a pointer to the next node
 */
struct node_s {
  void *element;
  node_t next;
};

/**
 * \struct list_s
 * \brief a list
 *
 * That list can contain any type of element
 */
struct list_s {
  node_t head;
  node_t key;
};

/**
 * \fn list_t list_create()
 * \brief Create a new list
 * \brief Complexity: O(1)
 * \return board_size the board size
 */
list_t list_create() {
  list_t l = malloc(sizeof (struct node_s));
  l->head = NULL;
  l->key = NULL;
  return l;
}

/**
 * \fn void list_add(list_t l, void *e)
 * \brief Add an element on head of the list
 * \brief Complexity: O(1)
 * \param l the list
 * \param e the element
 */
void list_add(list_t l, void *e) {
  node_t n = malloc(sizeof (struct node_s));
  n->element = e;
  n->next = l->head;
  l->head = n;
}


/**
 * \fn void list_next(list_t l)
 * \brief Make the cursor go the next item
 * \brief Complexity: O(1)
 * \param l the list
 */
void list_next(list_t l) {
  if (l->key != NULL)
    l->key = l->key->next;
}

/**
 * \fn void list_begin(list_t l) 
 * \brief Make the cursor on the first element
 * \brief Complexity: O(1)
 * \param l the list
 */
void list_begin(list_t l) {
  l->key = l->head;
}

/**
 * \fn void *list_getelement(list_t l)
 * \brief Return the current element
 * \brief Complexity: O(1)
 * \param l the list
 * \return an element
 */
void *list_getelement(list_t l) {
  if (l->key != NULL)
    return l->key->element;
  return NULL;
}

/**
 * \fn void list_delete(list_t l)
 * \brief Delete a list
 * \brief Complexity: O(n) if key != head if not => O(1)
 * \param l the list
 */
void list_delete(list_t l) {
  if (l->key == NULL)
    return;

  if (l->key == l->head) {
    /* Supprimer en tête */
    l->head = l->key->next;
    free(l->key->element); // choix
    free(l->key);
    l->key = l->head;
  }
  else {
    /* On cherche le noeud qui precede celui pointé par la key */
    node_t previous_node = l->head;

    while (previous_node->next != l->key)
      previous_node = previous_node->next; 

    previous_node->next = l->key->next;
    free(l->key->element); // choix
    free(l->key);
    l->key = previous_node->next;
  }
}

/**
 * \fn bool list_isempty(list_t l)
 * \brief Return whether or not the list is empty
 * \brief Complexity: O(1)
 * \param l the list
 * \return boolean
 */
bool list_isempty(list_t l) {
  return l->head == NULL;
}

/**
 * \fn bool list_isempty(list_t l)
 * \brief Return whether or not the cursor is at the end
 * \brief Complexity: O(1)
 * \param l the list
 * \return boolean
 */
bool list_isend(list_t l) {
  return list_getelement(l) == NULL;
}

/**
 * \fn void list_destroy(list_t l)
 * \brief Destroy a given list
 * \brief Complexity: O(n) where n = the list size
 * \param l the list
 * \return boolean
 */
void list_destroy(list_t l) {
  list_begin(l);

  while (!list_isempty(l))
    list_delete(l);

  free(l);
}

/**
 * \fn void list_destroy(list_t l)
 * \brief Destroy the node where the cursor is and the element on the node
 * \brief Complexity: O(1)
 * \param l the list
 */
static void list_hard_delete(list_t l, void (*delete_element)(void *e)) {
  if (l->key == NULL)
    return;

  if (l->key == l->head) {
    /* Supprimer en tête */
    l->head = l->key->next;
    delete_element(l->key->element);
    free(l->key);
    l->key = l->head;
  }
  else {
    /* On cherche le noeud qui precede celui pointé par la key */
    node_t previous_node = l->head;

    while (previous_node->next != l->key)
      previous_node = previous_node->next; 

    previous_node->next = l->key->next;
    delete_element(l->key->element);
    free(l->key);
    l->key = previous_node->next;
  }
}


/**
 * \fn void list_hard_clean(list_t l, void (*delete_element)(void *e))
 * \brief Destroy each element on the list one by one
 * \brief Complexity: O(n) where n = the list size
 * \param l the list
 */
void list_hard_clean(list_t l, void (*delete_element)(void *e)) {
  list_begin(l);

  while (!list_isempty(l))
    list_hard_delete(l, delete_element);
}


/**
 * \fn void list_hard_destroy(list_t l, void (*delete_element)(void *e))
 * \brief Destroy each element on the list one by one and the list itself
 * \brief Complexity: O(n) where n = the list size
 * \param l the list
 */
void list_hard_destroy(list_t l, void (*delete_element)(void *e)) {
  list_begin(l);

  while (!list_isempty(l))
    list_hard_delete(l, delete_element);

  free(l);
}





