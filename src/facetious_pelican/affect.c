/**
 * \file affect.c
 * \brief Contains the definitions of the functions used to manage the affectation
 * \author PARPAITE Thibault <br>
 * MENANTEAU Yoann
 * \date 02/01/2017
 */
 
#include <stdio.h>
#include <stdlib.h> 
#include "position.h"
#include "affect.h"

/*********************************
 * PRIVATE STRUCTURE & FUNCTIONS *
 *********************************/

/**
 * \struct affect_s
 * \brief affectations for each bird
 * 
 * affect contains all the positions affected for each bird
 */
struct affect_s {
  int size;
  int *pelican_a;
};


/********************
 * PUBLIC FUNCTIONS *
 ********************/

/* CONSTRUCTEURS et ACCESSEURS */

/**
 * \fn affect_t affect_create(int board_size, int *pelican_a)
 * \brief Create a new affectation
 * \brief Complexity: O(1)
 * \param board_size The board size
 * \param pelican_a An array containing all pelican positions
 * \return An affectation
 */
affect_t affect_create(int board_size, int *pelican_a) {
  affect_t a = malloc(sizeof (struct affect_s));
  a->size = board_size;
  a->pelican_a = pelican_a;

  return a;
}


/**
 * \fn void affect_destroy(affect_t a)
 * \brief Destroy an affectation
 * \brief Complexity: O(1)
 * \param a The affectation to destroy
 */
void affect_destroy(affect_t a) {
  free(a->pelican_a);
  free(a);
}


/**
 * \fn int affect_get_size(const affect_t a)
 * \brief Return the affectation size
 * \brief Complexity: O(1)
 * \param a The affectation
 * \return The affectation size
 */
int affect_get_size(const affect_t a) {
  return a->size;
}


/**
 * \fn int affect_get_size(const affect_t a)
 * \brief Return all the pelican positions
 * \brief Complexity: O(1)
 * \param a The affectation
 * \return An array with all the position ids
 */
int *affect_get_pelican_a(const affect_t a) {
  return a->pelican_a;
}


/**
 * \fn affect_t affect_copy(const affect_t a)
 * \brief Return a copy of an affectation
 * \brief Complexity: O(n) where n = the affectation size
 * \param a An affectation
 * \return An affectation copy
 */
affect_t affect_copy(const affect_t a) {
  int size = affect_get_size(a);
  int *old_pelican_a = affect_get_pelican_a(a);

  /* On recopie tout d'abord le tableau */
  int *pelican_a = malloc(size * sizeof (int));

  for (int i = 0 ; i < size ; ++i)
    pelican_a[i] = old_pelican_a[i];
   
  /* Puis on créé une nouvelle structure */
  return affect_create(size, pelican_a);
} 


/**
 * \fn void affect_display(const affect_t a)
 * \brief Display an affectation
 * \brief Complexity: O(n) where n = the affectation size
 * \param a An affectation
 */
void affect_display(board_t b, const affect_t a) {
  printf("\n-------------------\n");
  printf("AFFECTATION\n");
  printf("-------------------\n");
  printf("Pelican\t| Position\n");

  for(int i = 0 ; i < a->size ; ++i) {
    printf("%d\t| %d (tag : ", i + 1, a->pelican_a[i]);  
    position_t p = board_get_position_a(b)[a->pelican_a[i]];
    position_display_tags(p);
    printf(")\n");
  }

  printf("-------------------\n\n");
}


/**
 * \fn void display_graph(affect_t a, int board_size)
 * \brief Display the affectation in the graph (only for the 16-sized graph)
 * \brief Complexity: O(n) where n = the board size
 * \param a An affectation
 * \param board_size The board size
 */
void display_graph_16(affect_t a, int board_size){
  char pos[board_size];
  for (int i = 0; i < board_size; ++i){
    if (i < 9) 
      pos[affect_get_pelican_a(a)[i]] = '1'+i;
    else 
      pos[affect_get_pelican_a(a)[i]] = 'A'+i-9;
  }

  printf("%c-----------------------%c\n", pos[0], pos[1]);
  printf("|\\                     /|\n");
  printf("| ++--------%c--------++ |\n", pos[4]);
  printf("|  |      / | \\      |  |\n");
  printf("|  +-----%c  |  %c-----+  |\n", pos[8], pos[9]);
  printf("|  |    /|\\ | /|\\    |  |\n");
  printf("|  |   / |  %c  | \\   |  |\n", pos[12]);
  printf("|  |  /  | / \\ |  \\  |  |\n");
  printf("|  +-%c---%c     %c----%c+  |\n", pos[7], pos[15], pos[13], pos[5]);
  printf("|  |  \\  | \\ / |  /  |  |\n");
  printf("|  |   \\ |  %c  | /   |  |\n", pos[14]);
  printf("|  |    \\| /|\\ |/    |  |\n");
  printf("|  +------%c | %c-----++  |\n", pos[11], pos[10]);
  printf("|  |      \\ | /      |  |\n");
  printf("| ++--------%c--------++ |\n", pos[6]);
  printf("|/                     \\|\n");
  printf("%c-----------------------%c\n", pos[3], pos[2]);
}

void display_graph_8(affect_t a, int board_size){
  char pos[board_size];
  for (int i = 0; i < board_size; ++i){
      pos[affect_get_pelican_a(a)[i]] = '1'+i;
  }   
  printf("  +----%c-----------%c---+\n", pos[0], pos[1]);   
  printf("  |                    |\n");   
  printf("  %c                    %c\n", pos[7], pos[2]);   
  printf("  |                    |\n");   
  printf("  |                    %c\n", pos[3]);      
  printf("  |                    |\n");      
  printf("  %c                    %c\n", pos[6], pos[4]);      
  printf("  |                    |\n");      
  printf("  +----------%c---------+\n", pos[5]);   
}                         
