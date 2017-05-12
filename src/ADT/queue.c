/**
 * \file queue.c
 * \brief Contains the definitions of the functions used to create queues
 * \author PARPAITE Thibault <br>
 * MENANTEAU Yoann
 * \date 02/01/2017
 */

#include <stdio.h>
#include <stdlib.h>
#include "queue.h"


typedef struct node_s *node_t;

/**
 * \struct node_s
 * \brief a node used by the queue
 *
 * Contains an element and a pointer to the next node
 */
struct node_s {
  void *element;
  node_t next;
};

/**
 * \struct queue_s
 * \brief a queue
 *
 * Can contain any type of element
 */
struct queue_s {
  node_t head;
  node_t tail;
};

/**
 * \fn queue_t queue_create(void)
 * \brief Create a new queue
 * \brief Complexity: O(1)
 * \return A new queue
 */
queue_t queue_create(void) {
  queue_t q = malloc(sizeof (struct queue_s));
  q->head = NULL;
  q->tail = NULL;
  return q;
}

/**
 * \fn void queue_destroy(queue_t q)
 * \brief Destroy a queue
 * \brief Complexity: O(n) where n = the queue size
 * \param q The queue to destroy
 */
void queue_destroy(queue_t q) {
  while (!queue_isempty(q))
    queue_pop(q);
  
  free(q);
}

/**
 * \fn bool queue_isempty(queue_t q)
 * \brief Determine whether or not the queue is empty
 * \brief Complexity: O(1)
 * \param q The queue
 * \return a boolean
 */
bool queue_isempty(queue_t q) {
  return q->head == NULL;
}

/**
 * \fn void queue_push(queue_t q, void *e)
 * \brief Push a new element e in the queue q
 * \brief Complexity: O(1)
 * \param q The queue
 * \param e The element
 */
void queue_push(queue_t q, void *e) {
  node_t n = malloc(sizeof (struct node_s));
  n->element = e;
  n->next = NULL;

  if (queue_isempty(q))
    q->head = n;
  else
    q->tail->next = n;

  q->tail = n;
}

/**
 * \fn void *queue_front(queue_t q) 
 * \brief Return the last element pushed from the queue q
 * \brief Complexity: O(1)
 * \param q The queue
 * \return The element
 */
void *queue_front(queue_t q) {
  if (queue_isempty(q))
    return NULL;

  return q->head->element;
}

/**
 * \fn void queue_pop(queue_t q)
 * \brief Pop the last element pushed from the queue q 
 * \brief Complexity: O(1)
 * \param q The queue
 * \return The element
 */
void queue_pop(queue_t q) {
  if (queue_isempty(q))
    return;

  node_t old_head = q->head;
  q->head = q->head->next;
  free(old_head->element); // choix
  free(old_head);
}
