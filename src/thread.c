#define _BSD_SOURCE
#include "thread.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <ucontext.h>
#include <assert.h>
#include "valgrind/valgrind.h"

#define THREAD_STACK_SIZE 64*1024

list_t *ready_queue;
thread_t cur_thread;
thread_t main_thread;

void _startup_thread_(void) __attribute__ ((constructor));

void _startup_thread_(void){
  ready_queue = new_list();

  main_thread = malloc(sizeof (struct thread_s));
  main_thread->retval = NULL;
  main_thread->waiting_thread = NULL;

  if (getcontext(&(main_thread->context)) == -1) {
    perror("getcontext");
    return;
  }

  main_thread->status = RUNNING;
  cur_thread = main_thread;
}

void _free_thread_(thread_t t){
  if (t != main_thread) {
    VALGRIND_STACK_DEREGISTER(t->vg_sid);
    free(t->context.uc_stack.ss_sp);
  }
  free(t);
}

void _cleanup_thread_(void) __attribute__ ((destructor));

void _cleanup_thread_(void){
  free_list(ready_queue, _free_thread_);
  //freeing current stack if it was allocated manually gives memory errors
  if (cur_thread == main_thread) {
    _free_thread_(cur_thread);
  }
}

void _thread_wrap_function_(void*(*func)(void*), void *funcarg){
  void *retval = func(funcarg);
  thread_exit(retval);
}

thread_t thread_self(void){
  thread_yield();
  return cur_thread;
}

int thread_create(thread_t *newthread, void *(*func)(void *), void *funcarg){
  (*newthread) = malloc(sizeof (struct thread_s));
  (*newthread)->waiting_thread = NULL;

  if (getcontext(&((*newthread)->context)) == -1) {
    perror("getcontext");
    return -1;
  };

  (*newthread)->context.uc_stack.ss_size = THREAD_STACK_SIZE;
  (*newthread)->context.uc_stack.ss_sp = malloc((*newthread)->context.uc_stack.ss_size);
  (*newthread)->vg_sid = VALGRIND_STACK_REGISTER((*newthread)->context.uc_stack.ss_sp, (*newthread)->context.uc_stack.ss_sp + (*newthread)->context.uc_stack.ss_size);

  makecontext(&((*newthread)->context), (void (*)(void))_thread_wrap_function_, 2, func, funcarg);

  (*newthread)->status = READY;

#ifdef ORD_FIFO
  insert_in_list_tail(ready_queue, *newthread);
  thread_yield();
#elif ORD_FILO
  insert_in_list_head(ready_queue, *newthread);
  thread_yield();
#elif ORD_FIBONACCI
  // insertion en tête du nouveau thread pour le rendre prioritaire mais on reste dans le thread parent
  // on crée les deux fils à chaque fois et on passe la main à un des deux fils qui refait la même chose
  insert_in_list_head(ready_queue, *newthread);
#else
  insert_in_list_tail(ready_queue, *newthread);
  thread_yield();
#endif

  return 0;
}

int thread_yield(void){
  thread_t self = cur_thread;
  
  if (self->status == RUNNING) {
    self->status = READY;
    insert_in_list_tail(ready_queue, self);
  }

  thread_t next = get_head(ready_queue);
  next->status = RUNNING;
  remove_from_list(ready_queue, next, NULL);



  
  cur_thread = next;

  if(swapcontext(&(self->context), &(next->context)) == -1) {
    perror("swapcontext");
    return 1;
  }

  return 0;
}

int thread_join(thread_t thread, void **retval){
  thread_t self = cur_thread;

  if(thread->waiting_thread != NULL) {
    return -1;
  }

  if (thread->status != ENDED) {
    thread->waiting_thread = self;
    self->status = WAITING;
        
    thread_yield();
  }

  if(retval != NULL) {
    *retval = thread->retval;
  }

  _free_thread_(thread);

  return 0;
}

void thread_exit(void *retval){

  thread_t self = cur_thread;
  self->retval = retval;

  self->status = ENDED;

  if (self->waiting_thread != NULL) {
    self->waiting_thread->status = READY;

#ifdef ORD_FIFO
    insert_in_list_tail(ready_queue, self->waiting_thread);
#elif ORD_FILO
    insert_in_list_head(ready_queue, self->waiting_thread);
#elif ORD_FIBONACCI
    //insertion en tête pour remonter dans l'arbre
    insert_in_list_head(ready_queue, self->waiting_thread);
#else
    insert_in_list_tail(ready_queue, self->waiting_thread);
#endif

  } else if (is_empty(ready_queue)) {
    exit(0);
  }
  
  thread_yield();
}

int thread_mutex_init(thread_mutex_t *mutex){
  mutex->value = 1;
  mutex->waiting_queue = new_list();
  return 0;
}

int thread_mutex_destroy(thread_mutex_t *mutex){
  if (!is_empty(mutex->waiting_queue) || mutex->value <= 0) {
    return -1;
  }
  free_list(mutex->waiting_queue, NULL);
  return 0;
}

void thread_mutex_lock(thread_mutex_t *mutex){
  if (mutex->value <= 0) {
    thread_t self = cur_thread;
    self->status = WAITING;
    insert_in_list_tail(mutex->waiting_queue, self);
    thread_yield();
  }

  assert(mutex->value > 0);

  mutex->value -= 1;
}

void thread_mutex_unlock(thread_mutex_t *mutex){
  assert(mutex->value <= 0);

  mutex->value += 1;

  if (!is_empty(mutex->waiting_queue)) {
    thread_t next = get_head(mutex->waiting_queue);
    next->status = READY;
    remove_from_list(mutex->waiting_queue, next, NULL);
    insert_in_list_head(ready_queue, next);
  }

  thread_yield();
}
