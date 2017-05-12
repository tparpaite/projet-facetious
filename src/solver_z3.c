#include "solver_z3.h"

#define NO_SOLUTION 0

/**
 * \fn bool solver_z3(constraint_t *constraint_a, enum constraint_type constraint_type_a[], int board_size, int indice, int *bi_penguin_relation_a[], int mono_pinguin_relation_a[])
 * \brief The z3 solver
 * \brief Complexity: exponential
 * \param constraint_a The constraint array
 * \param constraint_type_a The constraint types
 * \param a The affectation 
 * \param board_size The board size
 * \param indice The current index
 * \param bi_penguin_relation_a All possible positions for each bi-penguin constraint
 * \param mono_pinguin_relation_a an array containing all possible positions for the position constraints
 * \return a valid affectation
 */
affect_t solver_z3(constraint_t *constraint_a, enum constraint_type constraint_type_a[], const board_t b, int indice, custom_type_t *bi_penguin_relation_a[], custom_type_t mono_pinguin_relation_a[]){  
  int board_size = board_get_size(b);
  affect_t valid_affect; 
  // If the affectation is satisfied
  valid_affect = apply_constraint_z3(b, constraint_a, NULL, bi_penguin_relation_a, mono_pinguin_relation_a, false);
  if (valid_affect){
    return valid_affect;
    }
  // If overflow then we return ERROR
  if (indice == board_size){
    return NO_SOLUTION; 
  }

  // After a first test, we save the constraint types and begin to remove constraints
  else if (indice == 0) {	
    for (int i = 0; i < board_size; i++){
      constraint_type_a[i] = get_constraint_type(constraint_a[i]);	  
    }
    
    set_constraint_type(constraint_a[0], NO_CONSTRAINT);
    printf("Avec retrait\n");
    // We test again with thre removed constraints
    return solver_z3(constraint_a, constraint_type_a, b, indice+1, bi_penguin_relation_a, mono_pinguin_relation_a);
  }

  
  /* **************** The next part will create an arborescence. That will test all possibilities. *************** */


  // We set and try an other combinaison
  set_constraint_type(constraint_a[indice], constraint_type_a[indice]);
  if (indice+1 < board_size)
    set_constraint_type(constraint_a[indice+1], NO_CONSTRAINT);

  valid_affect = solver_z3(constraint_a, constraint_type_a, b, indice+1, bi_penguin_relation_a, mono_pinguin_relation_a);
  if (valid_affect)
    return valid_affect;
 
  // Set and test an other combinaison
  set_constraint_type(constraint_a[indice], NO_CONSTRAINT);
  if (indice+1 < board_size)
    set_constraint_type(constraint_a[indice+1], NO_CONSTRAINT);
	
  valid_affect = solver_z3(constraint_a, constraint_type_a, b, indice+1, bi_penguin_relation_a, mono_pinguin_relation_a);
  if (valid_affect)
    return valid_affect;
  
  // We re-affect the original constraint types	
  set_constraint_type(constraint_a[indice], constraint_type_a[indice]);

  if (indice+1 < board_size)
    set_constraint_type(constraint_a[indice+1], constraint_type_a[indice+1]);

  // Finally if no solution found, we return NO_SOLUTION
  return NO_SOLUTION;
}
