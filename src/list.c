#include <stdlib.h>
#include <string.h>

#include "list.h"

list_t *new_list() {
  list_t *list = malloc(sizeof (list_t));
  CIRCLEQ_INIT(list);
  return list;
}

int is_empty(list_t *l) {
  return CIRCLEQ_EMPTY(l);
}

int is_in_list(list_t *l __attribute__ ((unused)), element_t e) {
  /*element_t el;
  FOR_EACH(el, l) {
    if (el == e) {
      return 1;
    }
  }
  return 0;*/
  return e->pointers.cqe_next != NULL && e->pointers.cqe_prev != NULL;
}

element_t get_head(list_t *l) {
  return l->cqh_first;
}

void insert_in_list_head(list_t *l, element_t e) {
  CIRCLEQ_INSERT_HEAD(l, e, pointers);
}

void insert_in_list_tail(list_t *l, element_t e) {
  CIRCLEQ_INSERT_TAIL(l, e, pointers);
}

void remove_from_list(list_t *l, element_t e, void(free_method)(element_t)) {
  /*if (!is_in_list(l, e)) {
    return;
    }*/
  CIRCLEQ_REMOVE(l, e, pointers);
  if (free_method != NULL) {
    free_method(e);
  } else {
    e->pointers.cqe_next = NULL;
    e->pointers.cqe_prev = NULL;
  }
}

void free_list(list_t *l, void(free_method)(element_t)) {
  element_t e;
  FOR_EACH(e, l) {
    remove_from_list(l, e, free_method);
  }
  free(l);
}
