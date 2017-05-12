/**
 * \file z3.c
 * \brief Contains the definitions of the functions used to generate a z3 script
 * \author PARPAITE Thibault <br>
 * MENANTEAU Yoann
 * \date 02/01/2017
 */
 
/* popen library */
#define _XOPEN_SOURCE 

#include <stdio.h>
#include <stdlib.h>
#include "z3.h"

/********************
 * PUBLIC FUNCTIONS *
 ********************/

/**
 * \fn void init_z3_formula(int board_size, FILE *script_file)
 * \brief Initialize the z3 formula with the conditions "each pelican has one place and has to be placed somewhere"
 * \brief Complexity: polynomial
 * \param board_size the board size
 * \param script_file the file to be written
 */
void init_z3_formula(int board_size, FILE *script_file) {
  fprintf (script_file, "\n;Initialisation\n");
  for (int i = 0; i < board_size; i++){
    // First, we declare each boolean used
    for (int j = 0; j < board_size; j++){
      fprintf(script_file, "(declare-const p%d_%d Bool)\n", i+1, j);
    }
  }
  // We assert that each pelican has to be placed
  fprintf(script_file,"(assert (and");
  for (int i = 0; i < board_size; ++i) {
    fprintf(script_file," (or");
    for (int j = 0; j < board_size; ++j) {
      fprintf(script_file," p%d_%d", i+1, j);
    }
    fprintf(script_file,")");
  }
  fprintf(script_file,"))");
  // We assert that each pelican has only one place
  fprintf(script_file,"(assert (and");
  for(int j = 0; j < board_size; ++j) {
    fprintf(script_file," (and");
    for(int i = 0; i < board_size; ++i) {
      fprintf(script_file," (implies p%d_%d (and", i+1, j);
      for( int k = 0; k < board_size; ++k) {
	if (k != i) {
	  fprintf(script_file," (not p%d_%d)", k+1, j);
	}
      }
      fprintf(script_file,"))");
    }
    fprintf(script_file,")");
  }
  fprintf(script_file,"))\n");
}


/**
 * \fn void generate_z3_fcs_constraints(int i, int j, int *bi_pel_relation_a, int board_size, bool is_opposite, FILE *script_file)
 * \brief Generate the statements xFACEy,xNEXTy,xCORNERy 
 * \brief Complexity: O(n²)
 * \param i the colour of the first bird
 * \param j the colour of the second bird
 * \param bi_pel_relation_a a bi penguin relation array, contains all the possible position couples for each bi-penguin constraint
 * \param board_size the board size
 * \param is_opposite whether or not the bird want the relation true or false
 * \param script_file the script file
 */
void generate_z3_fcs_constraints(int i, int j, custom_type_t *bi_pel_relation_a, int board_size, bool is_opposite, FILE *script_file) {
  fprintf(script_file,"\n;R_FCS\n");
  bool est_vide;
  fprintf(script_file,"(assert (and ");
  for (int k = 0; k < board_size; ++k) {
    // If the pelican 2 is positioned on the place k
    fprintf(script_file,"(implies p%d_%d (or", j, k);
    est_vide = true;
    for (int l = 0; l < board_size; ++l) {
      // Then at least one of the following places have to be free
      if ((!is_opposite && (custom_type_get_bit(bi_pel_relation_a[k],l))) || (is_opposite && !(custom_type_get_bit(bi_pel_relation_a[k], l)))) {
	fprintf(script_file," p%d_%d", i, l);			
	est_vide = false;
      }
    }
    if (est_vide)
      fprintf(script_file," false");

    fprintf(script_file,"))");
  }
  fprintf(script_file,"))\n");
}


/**
 * \fn generate_z3_position_constraints(int board_size, constraint_t constraint, FILE *script_file, int pos_tab[])
 * \brief Generate the script part for the position constraints
 * \brief Complexity: O(n) where n = board size
 * \param board_size the board size
 * \param constraint the constraint
 * \param script_file the script file
 * \param pos_tab an array of each possible positions for each position constraint
 */
void generate_z3_position_constraints(int board_size, constraint_t constraint, FILE *script_file, custom_type_t pos_tab[]) {
  bool est_vide = true;
  // Each "1" bit is a possible position for that constraint
  int c_p1 = get_constraint_pelican1(constraint);
  fprintf(script_file,"\n;Position\n");
  // We assert all possible position for that constraint
  fprintf(script_file,"(assert (or");
  enum tag *tag_a = get_constraint_location_tag_a(constraint);
  int tag_size = get_constraint_tag_size(constraint);
  for(int i = 0; i < tag_size; i++){
    for (int j = 0; j < board_size; j++){
      custom_type_t temp = pos_tab[tag_a[i]];
      if (custom_type_get_bit(temp, j)){
	fprintf(script_file," p%d_%d", c_p1, j);
	est_vide = false;
      }
    }
  }

  if (est_vide)
    fprintf(script_file," false");
	
  fprintf(script_file,"))\n");
}


/**
 * \fn void z3_place_affectation(affect_t affectation, int affectation_size, FILE *script_file)
 * \brief Add each bird position into the z3 script
 * \brief Complexity: O(n) where n = affectation size
 * \param affectation the affectation
 * \param affectation_size the affectation size
 * \param script_file the script file
 */
void z3_place_affectation(affect_t affectation, int affectation_size, FILE *script_file){
  int * pelican_a = affect_get_pelican_a(affectation);
  fprintf(script_file, "(assert(and ");
  for (int i = 0; i < affectation_size; ++i){
    fprintf(script_file, "p%d_%d ", i+1, pelican_a[i]);
  }
  fprintf(script_file, "))\n");
}

/**
 * \fn void z3_contradiction(FILE *script_file)
 * \brief Simply add false if there is an infinite cycle 
 * \brief Complexity: O(1)
 * (e.g.: the bird 1 wants what the bird 2 wants and the bird 2 wants same or the opposite of the bird 1). 
 * We consider that combination illogic and incorrect.
 * \param script_file the script file
 */
void z3_contradiction(FILE *script_file){
  fprintf(script_file, "(assert false)\n");
}


/**
 * \fn void display_z3_model(char model[])
 * \brief display a formatted z3 model
 * \brief Complexity: O(n) where n = the length of the string model
 * \param model the model from the z3 output
 */
affect_t get_z3_affect(char model[], int board_size){
  int affect_a[board_size];
  bool pion = true;
  bool sur_nombre = false;
  printf("Model : \n");
  int pelican_id = 0;
  for (int i = 0; model[i] != '\0'; i++){
    if (!sur_nombre && model[i] >= '0' && model[i] <= '9'){
      sur_nombre = true;
      if (pion) {
	int val = atoi(&model[i]);
	printf("p%d", val);
	pelican_id = val;
      }
      else {
	printf("_%d ", atoi(&model[i]));
	affect_a[pelican_id-1] = atoi(&model[i]);
	}
    } else {
      if (sur_nombre){
	if (pion){
	  pion = false;	    
	} else {
	  pion = true;
	}
      }
      sur_nombre = false;
    }
  }
  printf("\n");
  return affect_create(board_size, affect_a);
}
  
/**
 * \fn static void generate_z3_script(int affectation_size, constraint_t *constraint_a, bool placement, affect_t a, int *bi_penguin_relation_a[], int mono_penguin_relation_a[])
 * \brief Generate the z3 script to test an affectation
 * \brief Complexity: polynomial
 * \param affectation_size the affectation size
 * \param constraint_a the constraint array to treat the dependences
 * \param placement whether or not we want the pelican positions considered
 * \param a The affectation 
 * \param bi_penguin_relation_a All possible positions for each bi-penguin constraint
 * \param mono_penguin_relation_a an array containing all possible positions for the position constraints
 */
void generate_z3_script(int affectation_size, constraint_t *constraint_a, bool placement, affect_t a, custom_type_t *bi_penguin_relation_a[], custom_type_t mono_penguin_relation_a[]){
  // We use the file res
  FILE *res = fopen("res", "w+");
  bool treated_pelican[affectation_size];
  // We initialise the conditions one pelican on one case and one case for each pelican  	
  init_z3_formula(affectation_size, res);	
	
  // Generation of each constraint into the script file
  for (int i = 0; i < affectation_size; ++i) {
    switch(get_constraint_type(constraint_a[i])) {
    case POSITION:
      generate_z3_position_constraints(affectation_size, constraint_a[i], res, mono_penguin_relation_a);
      break;
    case NO_CONSTRAINT:
      break;
    case OPPOSITE_CONSTRAINT:
    case SAME_CONSTRAINT:
      memset(treated_pelican, 0, affectation_size * sizeof (bool));
      if (!treat_dependence(constraint_a[i], constraint_a, affectation_size, treated_pelican, bi_penguin_relation_a, a)){
	z3_contradiction(res);
      } else {
	i--;
	}
      break;
    default:
      generate_z3_fcs_constraints(get_constraint_pelican1(constraint_a[i]), get_constraint_pelican2(constraint_a[i]), bi_penguin_relation_a[get_constraint_type(constraint_a[i])], affectation_size, get_constraint_opposite(constraint_a[i]), res);
      break;
    }
  }		
  // Placement of the pelican
  if (placement)
    z3_place_affectation(a,affectation_size, res);
		
  fprintf(res, "(check-sat)\n(get-model)\n");
  fclose(res);
}


/**
 * \fn static void get_z3_output(char content[])
 * \brief Launch z3 and store the output into a string
 * \brief Complexity: O(1)
 * \param content the string
 */
void get_z3_output(char content[]){
  FILE *t = popen("/net/ens/herbrete/public/z3/bin/z3 res | grep -B 1 \"true\"", "r");
  fread(content, sizeof(char), OUTPUT_SIZE-1, t);
  pclose(t);
}
