/**
 * \file test_list.c
 * \brief Tests fonctionnels de la liste implémentée
 * \author PARPAITE Thibault
 * \date 25 novembre 2016
 */

#include <stdlib.h>
#include <stdio.h>
#include "list.h"


static int list_getelement_int(list_t l) {
  int *p = (int *) list_getelement(l);
  return *p;
}


static void list_add_int(list_t l, int i) {
  int *p = malloc(sizeof (int));
  *p = i;
  list_add(l, (void *) p);
}


static void list_int_display(list_t l) {
  return; /* Pour contrer -Werror defined but not used */
  list_begin(l);
  printf("Liste : (");

  while (!list_isend(l)) {
    int x = list_getelement_int(l);
    printf("%d -> ", x);
    list_next(l);
  }

  printf("NULL)\n");
}


int test_liste_entiers() {
  int res = true;
  list_t l = list_create();

  /* La liste est vide */
  res = res && (list_isempty(l));

  /* Liste de la forme (0 -> 1 -> 2 -> 3 -> 4 -> 5) */
  for (int i = 5 ; i >= 0 ; i--) {
    list_add_int(l, i);
  }

  /* Affichage de la liste */
  list_int_display(l);

  /* On vérifie que les éléments ajoutés sont bien présents dans la liste */
  list_begin(l);

  for (int i = 0 ; i <= 5 ; i++) {
    res = res && (list_getelement_int(l) == i);
    list_next(l);
  }

  /* On supprime le premier élément c'est-à-dire 0 */
  list_begin(l);
  list_delete(l);
  res = res && (list_getelement_int(l) == 1);

  /* La liste n'est pas vide */
  res = res && (!list_isempty(l));

  /* On détruist la liste */
  list_destroy(l);

  return res;
}


int main(void) {
  printf("test_liste_entiers : %s\n", test_liste_entiers()?"PASS":"FAIL");
  return EXIT_SUCCESS;
}
  
    


  
  
  
