/**
 * \file custom_type.h
 * \brief Contains the declaration of the functions used for the structure custom_type
 * \author PARPAITE Thibault <br>
 * MENANTEAU Yoann
 * \date 02/01/2017
 */
 
#ifndef _CUSTOM_TYPE_H
#define _CUSTOM_TYPE_H

#include<stdlib.h>
#include<stdio.h>
#include <stdbool.h>
#include <string.h>

typedef struct custom_type_s *custom_type_t;

/* FUNCTIONS */

extern custom_type_t custom_type_create(int size);
extern void custom_type_destroy(custom_type_t t);
extern void custom_type_or(custom_type_t t, custom_type_t q);
extern void custom_type_copy(custom_type_t t, custom_type_t q);
extern void custom_type_set_bit(custom_type_t t, int index, bool val);
extern bool custom_type_get_bit(custom_type_t t, int index);
extern void * custom_type_get_addr(custom_type_t t);
extern int custom_type_get_size(custom_type_t t);

#endif
