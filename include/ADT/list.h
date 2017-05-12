/**
 * \file list.h
 * \brief Contains the declaration of the functions used to create lists
 * \author PARPAITE Thibault <br>
 * MENANTEAU Yoann
 * \date 02/01/2017
 */

#ifndef _LIST_H
#define _LIST_H

#include <stdbool.h>

typedef struct list_s *list_t;

extern list_t list_create();
extern void list_destroy(list_t l);
extern void list_add(list_t l, void *e);
extern void list_next(list_t l);
extern void list_begin(list_t l);
extern void *list_getelement(list_t l);
extern void list_delete(list_t l);
extern bool list_isempty(list_t l);
extern bool list_isend(list_t l);

/* Cette fonction est utile lorsqu'on veut liberer les elements de la liste
 * et que ceux qui sont des structures, on fournit alors la fonction necessaire
 * à la libération de cette structure */
 
// Destroy each element on the list one by one and the list itself
extern void list_hard_destroy(list_t l, void (*delete_element)(void *e));
// Destroy each element on the list one by one but not the list
extern void list_hard_clean(list_t l, void (*delete_element)(void *e));

extern list_t G_p_list;

#endif /* _LIST_H */
