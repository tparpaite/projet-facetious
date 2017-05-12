/**
 * \file custom_type.c
 * \brief Contains the definitions of the functions used for the structure custom_type
 * \author PARPAITE Thibault <br>
 * MENANTEAU Yoann
 * \date 02/01/2017
 */

#include "custom_type.h"

/**
 * \struct custom_type_s
 * \brief A type without size limit
 * \brief We are using it to store possible positions for a constraint
 *
 * Contains the value addr and a size
 */
struct custom_type_s {
  void *addr;
  int size;
};


/**
 * \fn custom_type_t custom_type_create(int size)
 * \brief Initialize the type
 * \brief Complexity: O(size) because of memset (linear)
 * \param size the size in bytes
 * \return an element custom_type
 */
custom_type_t custom_type_create(int size){
  custom_type_t t =  malloc(sizeof (struct custom_type_s));
  t->addr = malloc(1 + size/8); // malloc prend une taille en octets
  t->size = size;
  memset(t->addr, 0, 1 + size/8);
  return t;
}


/**
 * \fn custom_type_get_addr(custom_type_t t)
 * \brief Get the address value
 * \brief Complexity: O(1)
 * \param t an element
 * \return the address
 */
void * custom_type_get_addr(custom_type_t t){
  return t->addr;
}


/**
 * \fn custom_type_get_size(custom_type_t t)
 * \brief Get the size value
 * \brief Complexity: O(1)
 * \param t an element
 * \return the size
 */
int custom_type_get_size(custom_type_t t){
  return t->size;
}


/**
 * \fn custom_type_or(custom_type_t t, custom_type_t q)
 * \brief Apply a logical or between two elements
 * \brief Complexity: O(n)
 * \param t an element (input|output)
 * \param q an element (input)
 */
void custom_type_or(custom_type_t t, custom_type_t q){
  int min_size = (t->size < q->size) ? t->size : q->size; 
  for (int i = 0; i < min_size; ++i)
    ((char *) t->addr)[i] |= ((char *) q->addr)[i];
}


/**
 * \fn custom_type_set_bit(custom_type_t t, int index, bool val)
 * \brief Set a bit to a value
 * \brief Complexity: O(1)
 * \param t an element (input|output)
 * \param index the index
 * \val the value
 */
void custom_type_set_bit(custom_type_t t, int index, bool val){
  int byte = index / 8;
  int offset = index % 8;
  unsigned char *c = t->addr;

  if (val)
    c[byte] |= (1 << offset);
  else
    c[byte] &= ~(1 << offset);
}


/**
 * \fn custom_type_get_bit(custom_type_t t, int index)
 * \brief Get a bit value
 * \brief Complexity: O(1)
 * \param t an element
 * \param index the index
 * \return the value
 */
bool custom_type_get_bit(custom_type_t t, int index){
  int byte = index / 8;
  int offset = index % 8;
  unsigned char *c = t->addr;
  return (c[byte] & (1 << offset));
}


/**
 * \fn custom_type_destroy(custom_type_t t)
 * \brief Destructor
 * \brief Complexity: O(1)
 * \param t an element
 */
void custom_type_destroy(custom_type_t t) {
  free(t->addr);
  free(t);
}


/**
 * \fn custom_type_affect(custom_type_t t, custom_type_t q)
 * \brief Affect a value
 * \brief Complexity: O(1)
 * \param t an element(INPUT|OUTPUT)
 * \param q the element to affect
 */
void custom_type_copy(custom_type_t t, custom_type_t q){
  int min_size = (t->size < q->size) ? t->size : q->size; 
  for (int i = 0; i < min_size; ++i)
    ((char *)t->addr)[i] = ((char *)q->addr)[i];
		
  t->size = q->size;
}
