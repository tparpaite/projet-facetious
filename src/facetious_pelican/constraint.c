/**
 * \file constraint.c
 * \brief Contains the definitions of the functions used to manage the constraints
 * \author PARPAITE Thibault <br>
 * MENANTEAU Yoann
 * \date 02/01/2017
 */

#include <stdlib.h>
#include <stdio.h>
#include "constraint.h"
#include "z3.h" 
#include "list.h"
#include <unistd.h>
#include <string.h>

#define NO_PELICAN_LEFT -1
#define TIMEOUT 3

/*********************************
 * PRIVATE STRUCTURE & FUNCTIONS *
 *********************************/

/**
 * \struct constraint_s
 * \brief constraint for a bird
 *
 * constraint represents a constraint associated to one or two birds
 */
struct constraint_s {
  enum constraint_type type;
  enum tag *location_tag_a;    /* NONE if not a POSITION constraint */
  custom_type_t positions;
  int tag_size;                /* 0 if not a POSITION constraint */
  int p1;
  int p2;                      /* NO_COLOR if mono-pingouin constraint */
  bool opposite;
};


/**
 * \fn static char *string_from_constraint_type(enum constraint_type type)
 * \brief Transform a constraint_type enum to a string
 * \brief Complexity = 0(1)
 * \param type the constraint type
 * \return the string version
 */
static char *string_from_constraint_type(enum constraint_type type) {
  static char *strings[] = { "FACE", "SAME_SIDE", "CORNER", "POSITION", "SAME_CONSTRAINT", "OPPOSITE_CONSTRAINT", "NO_CONSTRAINT" };
  return strings[type];
}

static char *string_from_tag(enum tag t) {
  static char *strings[] = {  "TAG_NORTH", "TAG_SOUTH", "TAG_NORTH_SOUTH", "TAG_CORNER", "TAG_EAST", "TAG_WEST", "TAG_FAR", "TAG_BAGPIPE", "NONE"};
  return strings[t];
}


/**
 * \fn static void affect_new_constraint(constraint_t c1, constraint_t c2, int *pos_relations[], int affectation_size, affect_t a)
 * \brief Generate a new constraint based on an other one
 * \brief Complexity = O(n) where n = tag quantity 
 * \param c1 the constraint to be affected
 * \param c2 the constraint to affect 
 * \param bi_penguin_relation_a All possible positions for each bi-penguin constraint
 * \param affectation_size the affectation size
 * \param affectation the affectation
 */
static void affect_new_constraint(constraint_t c1, constraint_t c2, custom_type_t *bi_penguin_relation_a[], int affectation_size, affect_t affectation){
  int *affect_a = NULL;
  
  if (affectation != NULL)
    affect_a = affect_get_pelican_a(affectation);

  c1->type = c2->type;
  
  // If the type is POSITION, we simply copy the second constraint to the first one
  if (c1->type == POSITION) {
    /* On fait une hard copy */
    c1->tag_size = c2->tag_size;
    c1->positions = c2->positions;
    c1->location_tag_a = malloc(c2->tag_size * sizeof (enum tag));
    for (int i = 0 ; i < c2->tag_size ; ++i)
      c1->location_tag_a[i] = c2->location_tag_a[i];
  }

  // If it is an other type (c1->p2 != c2->p2 != NO_COLOR) and if there is no cycles, we just copy the possible positions and the second pelican of the second constraint
  if (c1->p2 != c2->p2 && c1->p1 != c2->p2) {
    c1->p2 = c2->p2;
    c1->positions = c2->positions; 
  }
  else {
    // If there is a cycle, we have to generate a new pelican 2 for the constraint
    // For example, if the c2->p2 == c1->p1 of type face, we can't accept that p1 face p1, so we have to generate an other pelican
    int random_value = rand()%(affectation_size-2);
    if (random_value >= c1->p2-1)
      random_value++;
    if (random_value >= c1->p1-1)
      random_value++;

    c1->p2 = random_value+1;
	
    if (c1->type != NO_CONSTRAINT && affectation != NULL)
      custom_type_copy(c1->positions, bi_penguin_relation_a[c1->type][affect_a[c1->p2-1]]);
    else if (affectation != NULL) {
      for (int i = 0; i < affectation_size; ++i)
	custom_type_set_bit(c1->positions, i, true);
    }
  }
}


/********************
 * PUBLIC FUNCTIONS *
 ********************/

/* CONSTRUCTEURS et ACCESSEURS */

/**
 * \fn constraint_t constraint_create(enum constraint_type type, enum tag *location_tag_a,  int tag_size, int p1, int p2, bool opposite)
 * \brief Create a new constraint
 * \brief Complexity = 0(1)
 * \param type the constraint type
 * \param location_tag_a the location tags
 * \param tag_size the number of tags
 * \param p1 the pelican 1
 * \param p2 the pelican 2
 * \param opposite whether the pelican wants the opposite of that constraint or not
 * \return the constraint
 */
constraint_t constraint_create(enum constraint_type type, enum tag *location_tag_a, int tag_size, int p1, int p2, bool opposite) {
  constraint_t c = malloc(sizeof (struct constraint_s));

  c->type = type;
  c->location_tag_a = location_tag_a;
  c->positions = NULL;
  c->tag_size = tag_size;
  c->p1 = p1;
  c->p2 = p2;
  c->opposite = opposite;

  return c;
}


/**
 * \fn void constraint_destroy(constraint_t c)
 * \brief Destroy a constraint
 * \brief Complexity = 0(1)
 * \param c the constraint
 */
void constraint_destroy(constraint_t c) {
  if (c->p2 == NO_COLOR) /* mono-pinguin constraint */
    free(c->location_tag_a);

  if (c->positions != NULL)
    custom_type_destroy(c->positions);
  
  free(c);
}


/**
 * \fn bool get_constraint_opposite(constraint_t c)
 * \brief Return whether ot not the pelican wants the opposite of a constraint 
 * \brief Complexity = 0(1)
 * \param c the constraint
 * \return a boolean
 */
bool get_constraint_opposite(constraint_t c) {
  return c->opposite;
}


/**
 * \fn enum tag * get_constraint_location_tag_a(constraint_t c)
 * \brief Return an array of location tag for constraints of type position
 * \brief Complexity = 0(1)
 * \param c the constraint
 * \return an array of tags
 */
enum tag *get_constraint_location_tag_a(constraint_t c) {
  return c->location_tag_a;
}


/**
 * \fn int get_constraint_tag_size(constraint_t c)
 * \brief Return the number of tags for constraints of type position
 * \brief Complexity = 0(1)
 * \param c the constraint
 * \return a size
 */
int get_constraint_tag_size(constraint_t c) {
  return c->tag_size;
}


/**
 * \fn int get_constraint_pelican1(constraint_t c)
 * \brief Return the pelican 1
 * \brief Complexity = 0(1)
 * \param c the constraint
 * \return the pelican colour
 */
int get_constraint_pelican1(constraint_t c) {
  return c->p1;
}


/**
 * \fn int get_constraint_pelican2(constraint_t c)
 * \brief Return the pelican 2
 * \brief Complexity = 0(1)
 * \param c the constraint
 * \return the pelican colour
 */
int get_constraint_pelican2(constraint_t c) {
  return c->p2;
}


/**
 * \fn int get_constraint_positions(constraint_t c)
 * \brief Return the matching positions with this constraint
 * \brief Complexity = 0(1)
 * \param c the constraint
 * \return an int which each bit to 1 is a matching position
 */
custom_type_t get_constraint_positions(constraint_t c) {
  return c->positions;
}


/**
 * \fn enum constraint_type get_constraint_type(constraint_t c)
 * \brief Return the constraint type
 * \brief Complexity = 0(1)
 * \param c the constraint
 * \return the constraint type
 */
enum constraint_type get_constraint_type(constraint_t c){
  return c->type;
}

void set_constraint_type(constraint_t c, enum constraint_type t){
  c->type = t;
}

/**
 * \fn void constraint_set_positions(constraint_t c, int positions)
 * \brief Set the matching positions with the constraint
 * \brief Complexity = 0(1)
 * \param c the constraint
 * \param positions an int which each bit to 1 is a possible position 
 */
void constraint_set_positions(constraint_t c, custom_type_t positions) {
  c->positions = positions;
}


/* FUNCTIONS */

/**
 * \fn bool constraint_face(const board_t b, int position_p1, int position_p2)
 * \brief Test a constraint of type FACE
 * \brief Complexity = 0(n) where n is the board size
 * \param b the board 
 * \param position_p1 the position of the pelican 1
 * \param position_p2 the position of the pelican 2
 * \return a boolean 
 */
bool constraint_face(const board_t b, int position_p1, int position_p2) {
  return (has_tag(b, position_p1, TAG_SOUTH) && has_tag(b, position_p2, TAG_NORTH))
    || (has_tag(b, position_p1, TAG_NORTH) && has_tag(b, position_p2, TAG_SOUTH))
    || (has_tag(b, position_p1, TAG_EAST) && has_tag(b, position_p2, TAG_WEST))
    || (has_tag(b, position_p1, TAG_WEST) && has_tag(b, position_p2, TAG_EAST));
}


/**
 * \fn bool constraint_same_side(const board_t b, int position_p1, int position_p2)
 * \brief Test a constraint of type SAME_SIDE
 * \brief Complexity = 0(n) where n is the board size
 * \param b the board 
 * \param position_p1 the position of the pelican 1
 * \param position_p2 the position of the pelican 2
 * \return a boolean 
 */
bool constraint_same_side(const board_t b, int position_p1, int position_p2) {
  return (has_tag(b, position_p1, TAG_SOUTH) && has_tag(b, position_p2, TAG_SOUTH))
    || (has_tag(b, position_p1, TAG_NORTH) && has_tag(b, position_p2, TAG_NORTH))
    || (has_tag(b, position_p1, TAG_EAST) && has_tag(b, position_p2, TAG_EAST))
    || (has_tag(b, position_p1, TAG_WEST) && has_tag(b, position_p2, TAG_WEST));
}  


/**
 * \fn bool constraint_position(const board_t b, int position, enum tag *location_tag_a, int tag_size)
 * \brief Test a constraint of type POSITION
 * \brief Complexity = O(n*m) where n = tag quantity for the constraint and m = tag quantity for the position
 * \param b the board 
 * \param position the position
 * \param location_tag_a the tags in the constraint
 * \param tag_size the quantity of tags for that constraint
 * \return a boolean 
 */
bool constraint_position(const board_t b, int position, enum tag *location_tag_a, int tag_size) {
  for (int i = 0 ; i < tag_size ; ++i){
    if (has_tag(b, position, location_tag_a[i]))
      return true;
  }
  return false;
}


/**
 * \fn bool constraint_corner(const board_t b, int position_p1, int position_p2)
 * \brief Test a constraint of type CORNER
 * \brief Complexity = linear ?
 * \param b the board 
 * \param position_p1 the pelican 1 position
 * \param position_p2 the pelican 2 position 
 * \return a boolean 
 */
bool constraint_corner(const board_t b, int position_p1, int position_p2) {
  return (has_tag(b, position_p1, TAG_CORNER) && has_tag(b, position_p2, TAG_CORNER) 
	  && (is_neighboor(b, position_p1, position_p2)));
}


/**
 * \fn bool apply_constraint(const board_t b, const affect_t a, const constraint_t c, const constraint_t *constraint_a, int *bi_penguin_relation_a[])
 * \brief Apply constraint c on board b with affectation a
 * \brief Complexity: O(n²) (with n = the affectation size) if a lot of dependance but the next constraint won't have any dependence in this case and the complexity will be O(1)
 * \param b the board
 * \param a an affectation
 * \param c a constraint
 * \param constraint_a the constraint array to treat the dependences
 * \param bi_penguin_relation_a All possible positions for each bi-penguin constraint
 * \return A boolean telling if the constraint is verified.
 */
bool apply_constraint(const board_t b, const affect_t a, const constraint_t c, const constraint_t *constraint_a, custom_type_t *bi_penguin_relation_a[]) {
  int board_size = board_get_size(b);
  int *affect_a = affect_get_pelican_a(a);
  int pos_pelican = affect_a[c->p1-1];
  bool treated_pelican[board_size];
  bool opposite = get_constraint_opposite(c);
  custom_type_t positions = get_constraint_positions(c);

  switch(c->type){
  case NO_CONSTRAINT:
    return true;
    break;
    // In case of dependence, we treat it
  case OPPOSITE_CONSTRAINT:
  case SAME_CONSTRAINT:
    memset(treated_pelican, 0, board_size * sizeof (bool));
    if (treat_dependence(c, constraint_a, board_size, treated_pelican, bi_penguin_relation_a, a)){
      return apply_constraint(b, a, c, constraint_a, bi_penguin_relation_a);
    }
    else
      return false;
    break;
  default:
    // We verify that the position is correct with this constraint
    if ((!opposite && custom_type_get_bit(positions, pos_pelican)) || (opposite && !(custom_type_get_bit(positions, pos_pelican)))){
      return true;
    }
    else {
      return false;
    }
    break; 
  }
}


/**
 * \fn bool apply_constraint_rec(const board_t b, const affect_t a, const constraint_t c,bool p_left_a[], constraint_t constraint_a[], int *bi_penguin_relations[])
 * \brief Apply constraint c on board b with affectation a
 * \brief Complexity: polynomial
 * \param b the board
 * \param a an affectation
 * \param c a constraint
 * \param p_left_a a boolean array to inform which pelican's constraint had been treated
 * \param constraint_a all the constraints
 * \param bi_penguin_relation_a All possible positions for each bi-penguin constraint
 * \return A boolean telling if the constraint is verified.
 */
bool apply_constraint_rec(const board_t b, const affect_t a, int indice, constraint_t constraint_a[], custom_type_t *bi_penguin_relation_a[]) {
  constraint_t c = constraint_a[indice];
  int affectation_size = board_get_size(b);
  int *pelican_a = affect_get_pelican_a(a);
  bool treated_pelican[affectation_size];
  int position_p1 = pelican_a[c->p1-1];
  bool opposite = get_constraint_opposite(c);
  custom_type_t positions = get_constraint_positions(c);
 
  switch(c->type) {
  case NO_CONSTRAINT:
    break;
    // If there is a dependence, we treat it
  case SAME_CONSTRAINT:
  case OPPOSITE_CONSTRAINT:
    memset(treated_pelican, 0, affectation_size * sizeof (bool));
    if (treat_dependence(c, constraint_a, affectation_size, treated_pelican, bi_penguin_relation_a, a))
      return apply_constraint_rec(b, a, indice, constraint_a, bi_penguin_relation_a);
    else
      return false;
    break;
  default:
    // If not we verify that the pelican position satisfy the constraint
    if ((!opposite && !(custom_type_get_bit(positions, position_p1))) || (opposite && custom_type_get_bit(positions, position_p1)))
      return false;
    break;
  }
  indice++;
  if (indice == affectation_size)
    return true;

  // And we do the same until all the constraints are treated
  return apply_constraint_rec(b, a, indice, constraint_a, bi_penguin_relation_a);
}


/**
 * \fn bool apply_constraint_z3(const board_t b, constraint_t *constraint_a, affect_t a, int *bi_penguin_relation_a[], int mono_pinguin_relation_a[]) 
 * \brief Apply constraints on board b with affectation a
 * \brief Complexity: 
 * \param b The board
 * \param constraint_a The constraints to apply
 * \param a The affectation
 * \param bi_penguin_relation_a All possible positions for each bi-penguin constraint
 * \param mono_pinguin_relation_a an array containing all possible positions for the position constraints
 * \return the affectation if it is valid or null if not
 */
affect_t apply_constraint_z3(const board_t b, constraint_t *constraint_a, affect_t a, custom_type_t *bi_penguin_relation_a[], custom_type_t mono_pinguin_relation_a[], bool placement) {
  int board_size = board_get_size(b);	
  // Generate the z3 script file	
  generate_z3_script(board_size, constraint_a, placement, a, bi_penguin_relation_a, mono_pinguin_relation_a);	
  char content[OUTPUT_SIZE] = {0};
  // Test the affectation
  get_z3_output(content);
  // Display the model if satisfied
  if (content[0]){
    return get_z3_affect(content,board_size);
  } 
  return NULL;
}


/**
 * \fn static bool treat_dependance(constraint_t c1, const constraint_t constraint_a[], int affectation_size, bool treated_pelican[], int *pos_relations[], affect_t a)
 * \brief Treat each dependence
 * \brief Complexity = O(n²) where n = affectation size
 * \param c1 the constraint containing a dependence
 * \param constraint_a the array of all the constraints
 * \param affectation_size the affectation size
 * \param treated_pelican a boolean array to know whether or not a pelican has been treated
 * \param pos_relations All possible positions for each bi-penguin constraint
 * \param a the affectation
 * \return true if all the dependence are treated or false if not
 */
bool treat_dependence(constraint_t c1, const constraint_t constraint_a[], int affectation_size, bool treated_pelican[], custom_type_t *pos_relations[], affect_t a){
  // We get the constraint depending of the first one
  constraint_t c2 = constraint_a[c1->p2-1];
  // We mark the pelican
  treated_pelican[c1->p1-1] = true;
  // If the second constraint has itself a dependence ...
  if (c2->type == OPPOSITE_CONSTRAINT || c2->type == SAME_CONSTRAINT) {
    // We verify that we are not in a cycle
    if (!treated_pelican[c1->p2-1]){
      // We adapt the negation -- e.g. P1 wants negation of P2 who wants the negation of A so P1 wants A
      if (c1->opposite && c2->opposite)
	c1->opposite = false;
      // P1 want the same than P2 who doesn't want B so P1 doesn't want B
      else if (!c1->opposite && c2->opposite)
	c1->opposite = true;
      // If one dependence can't be treated the affectation is "unsat" so we return false
      if (!treat_dependence(c2, constraint_a, affectation_size, treated_pelican, pos_relations, a)){
	return false;
      }
    }
    else {
      return false;
    }
  }
  // We fix the dependence
  affect_new_constraint(c1, c2, pos_relations, affectation_size, a);
  return true;
}


/**
 * \fn void display_constraint(const constraint_t c)
 * \brief Simply display a constraint on the standard output
 * \brief Complexity: O(1)
 * \param c the constraint to display
 */
void display_constraint(const constraint_t c) {
  enum tag *t;
  printf("Pelican %d ", c->p1);
  if (!c->opposite)
    printf("wants ");
  else
    printf("doesn't want ");

  switch(c->type) {
  case POSITION:
    printf("a position ");
    t =  c->location_tag_a;
    printf("with tag(s):\t ");
    for (int i = 0; i < c->tag_size; i++)
      printf("%s\t", string_from_tag(t[i])); 
		
    printf("\n");
    break;
  case OPPOSITE_CONSTRAINT:
    printf("OPPOSITE_CONSTRAINT with the bird%d\n", c->p2);
    break;
  default:
    printf("%s with bird %d\n", string_from_constraint_type(c->type), c->p2);
    break;
  }
}
