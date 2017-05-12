/**
 * \file queue.h
 * \brief Contains the declaration of the functions used to create queues
 * \author PARPAITE Thibault <br>
 * MENANTEAU Yoann
 * \date 02/01/2017
 */

#ifndef _QUEUE_H
#define _QUEUE_H

#include <stdbool.h>

typedef struct queue_s *queue_t;

extern queue_t queue_create(void);
extern void queue_destroy(queue_t q);
extern bool queue_isempty(queue_t q);
extern void queue_push(queue_t q, void *e);
extern void *queue_front(queue_t s);
extern void queue_pop(queue_t s);

#endif /* _QUEUE_H */
