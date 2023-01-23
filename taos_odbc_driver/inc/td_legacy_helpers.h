/* Copyright (C) 2000 MySQL AB & MySQL Finland AB & TCX DataKonsult AB
                 2016 MariaDB Corporation AB
   
   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
   
   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.
   
   You should have received a copy of the GNU Library General Public
   License along with this library; if not see <http://www.gnu.org/licenses>
   or write to the Free Software Foundation, Inc.,
   51 Franklin St., Fifth Floor, Boston, MA 02110, USA
*/
#ifndef _TD_LEGACY_HELPERS_H
#define _TD_LEGACY_HELPERS_H

/********
 *
 * Function definitions, types, macros for TDDB_List, TDDB_DynArray, and TDDB_DynString copied from C/C
 *
 ********/

/*
  *************** Dubble-linked lists *********
*/
typedef struct st_td_odbc_list {
  struct st_td_odbc_list *prev, *next;
  void *data;
} TDDB_List;

typedef int(*TDDB_ListWalkAction)(void *, void *);

	/* Add a element to start of list */
TDDB_List *TDDB_ListAdd(TDDB_List *root, TDDB_List *element);
TDDB_List *TDDB_ListDelete(TDDB_List *root, TDDB_List *element);
void TDDB_ListFree(TDDB_List *root, unsigned int free_data);
TDDB_List *TDDB_ListCons(void *data, TDDB_List *list);
TDDB_List *TDDB_ListReverse(TDDB_List *root);
unsigned int TDDB_ListLength(TDDB_List *list);
int TDDB_ListWalk(TDDB_List *list, TDDB_ListWalkAction action, char *argument);

#define TDDB_LIST_REST(a) ((a)->next)
#define TDDB_LIST_PUSH(a,b) (a)=TDDB_ListCons((b),(a))
#define TDDB_LIST_POP(A) {TDDB_List *old=(A); (A)=TDDB_ListDelete(old,old) ; free((char *) old); }

/************************** TDDB_DynString ************************/
typedef struct st_td_odbc_dynstr {
  char *str;
  size_t length,max_length,alloc_increment;
} TDDB_DynString;

my_bool TDDB_InitDynamicString(TDDB_DynString *str, const char *init_str,
			    size_t init_alloc, size_t alloc_increment);
my_bool TDDB_DynstrSet(TDDB_DynString *str, const char *init_str);
my_bool TDDB_DynstrRealloc(TDDB_DynString *str, size_t additional_size);
my_bool TDDB_DynstrAppend(TDDB_DynString *str, const char *append);
my_bool TDDB_DynstrAppendMem(TDDB_DynString *str, const char *append,
			  size_t length);
void TDDB_DynstrFree(TDDB_DynString *str);
char *TDDB_DynstrMake(register char *dst, register const char *src, size_t length);

/* Helper-macro to slightly optimize appending of constant strings */
#define TDDB_DYNAPPENDCONST(DYNSTR, LITERALCONST) TDDB_DynstrAppendMem(DYNSTR, LITERALCONST, sizeof(LITERALCONST) - 1)

/************* TDDB_DynArray - Arrays that can grow dynamicly. **************/
typedef struct ma_odbc_st_dynarr {
  char *buffer;
  unsigned int elements,max_element;
  unsigned int alloc_increment;
  unsigned int size_of_element;
} TDDB_DynArray;

my_bool TDDB_InitDynamicArray(TDDB_DynArray *array, unsigned int element_size,
                              unsigned int init_alloc, unsigned int alloc_increment);
my_bool TDDB_InsertDynamic(TDDB_DynArray *array, void *element);
	/* Alloc room for one element */
unsigned char *TDDB_AllocDynamic(TDDB_DynArray *array);
	/* remove last element from array and return it */
unsigned char *TDDB_PopDynamic(TDDB_DynArray *array);
my_bool TDDB_SetDynamic(TDDB_DynArray *array, void * element, unsigned int idx);
void TDDB_GetDynamic(TDDB_DynArray *array, void * element, unsigned int idx);
void TDDB_DeleteDynamic(TDDB_DynArray *array);
void TDDB_DeleteDynamicElement(TDDB_DynArray *array, unsigned int idx);
void TDDB_FreezeSizeDynamic(TDDB_DynArray *array);

#endif /* #ifndef _TD_LEGACY_HELPERS_H */
