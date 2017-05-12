/**
 * \file test_list.c
 * \brief Tests fonctionnels de la file implémentée
 * \author PARPAITE Thibault
 * \date 29 novembre 2016
 */

#include <stdlib.h>
#include <stdio.h>
#include "queue.h"


static int queue_front_int(queue_t q) {
  int *p = (int *) queue_front(q);
  return *p;
}


static void queue_push_int(queue_t q, int i) {
  int *p = malloc(sizeof (int));
  *p = i;
  queue_push(q, (void *) p);
} 


int test_file_entiers() {
  int res = true;
  queue_t q = queue_create();

  /* La file est vide */
  res = res && (queue_isempty(q));

  /* Liste de la forme (0 -> 1 -> 2 -> 3 -> 4 -> 5) */
  for (int i = 0 ; i <= 5 ; i++) {
    queue_push_int(q, i);
  }

  /* La file n'est pas vide */
  res = res && (!queue_isempty(q));

  /* On vérifie que les éléments ajoutés sont bien présents et sont défiles dans le bon ordre */

  for (int i = 0 ; i <= 5 ; i++) {
    res = res && (queue_front_int(q) == i);
    queue_pop(q);
  }

  /* On détruist la liste */
  queue_destroy(q);

  return res;
}


int main(void) {
  printf("test_file_entiers : %s\n", test_file_entiers()?"PASS":"FAIL");
  return EXIT_SUCCESS;
}
