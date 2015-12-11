#ifndef LIST_H
#define LIST_H

#include "thread.h"

#include <sys/queue.h>

#define FOR_EACH(E, L) for (E = L->cqh_first; E != (void*)L; E = E->pointers.cqe_next)

struct thread_s;

typedef struct list_s list_t;
typedef struct thread_s *element_t;

CIRCLEQ_HEAD(list_s, thread_s);

/*
 * Alloue et initialise une liste
 */
list_t *new_list();

/*
 * Indique si la liste est vide
 */
int is_empty(list_t *l);

/*
 * Retourne le premier élément dont l'adresse est égale à data ou NULL
 */
int is_in_list(list_t *l, element_t el);

/*
 * Retourne le premier élément de la liste
 */
element_t get_head(list_t *l);

/*
 * Insère un élément en début de liste
 */
void insert_in_list_head(list_t *l, element_t e);

/*
 * Copie et insère un nouvel élément en fin de liste
 */
void insert_in_list_tail(list_t *l, element_t e);

/*
 * Retire un élément de la liste et, si free_method est non nulle, libère sa mémoire
 */
void remove_from_list(list_t *l, element_t e, void(free_method)(element_t));

/*
 * Libère une liste et, si free_method est non nulle, ses éléments
 */
void free_list(list_t *l, void(free_method)(element_t));

#endif
