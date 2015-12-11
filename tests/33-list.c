#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "thread.h"
#include "list.h"

int main() {
  list_t *l = new_list();

  assert(l != NULL);
  assert(is_empty(l));

  thread_t t1, t2;
  t1 = malloc(sizeof(struct thread_s));
  t2 = malloc(sizeof(struct thread_s));

  insert_in_list_tail(l, t1);

  assert(!is_empty(l));
  assert(is_in_list(l, t1));

  insert_in_list_head(l, t2);

  assert(get_head(l) == t2);

  assert(is_in_list(l, t2));
  
  int i = 0;
  element_t e;
  FOR_EACH(e, l) {
    i++;
  }
  assert(i == 2);

  remove_from_list(l, t1, NULL);
  remove_from_list(l, t2, NULL);

  assert(!is_in_list(l, t1));
  assert(!is_in_list(l, t2));

  assert(is_empty(l));

  free(t1);
  free(t2);
  free_list(l, NULL);
  
  return 0;
}
