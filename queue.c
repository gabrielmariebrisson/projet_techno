#include "queue.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

/* *********************************************************** */

struct queue_s {
  struct element_s* head;
  struct element_s* tail;
  unsigned int length;
};

/* *********************************************************** */

struct element_s {
  void* data;
  struct element_s* next;
  struct element_s* prev;
};

/* *********************************************************** */

typedef struct queue_s queue;
typedef struct element_s element_t;

/* *********************************************************** */

queue* queue_new()
{
  queue* q = malloc(sizeof(queue));
  assert(q);
  q->length = 0;
  q->tail = q->head = NULL;
  return q;
}

/* *********************************************************** */

void queue_push_head(queue* q, void* data)
{
  assert(q);
  element_t* e = malloc(sizeof(element_t));
  assert(e);
  e->data = data;
  e->prev = NULL;
  e->next = q->head;
  if (q->head) q->head->prev = e;
  q->head = e;
  if (!q->tail) q->tail = e;
  q->length++;
}

/* *********************************************************** */

void queue_push_tail(queue* q, void* data)
{
  assert(q);
  element_t* e = malloc(sizeof(element_t));
  assert(e);
  e->data = data;
  e->prev = q->tail;
  e->next = NULL;
  if (q->tail) q->tail->next = e;
  q->tail = e;
  if (!q->head) q->head = e;
  q->length++;
}

/* *********************************************************** */

void* queue_pop_head(queue* q)
{
  assert(q);
  assert(q->length > 0);
  if (!q->head) return NULL;
  void* data = q->head->data;
  element_t* next = q->head->next;
  if (next) next->prev = NULL;
  free(q->head);
  q->head = next;
  q->length--;
  if (!q->head) q->tail = NULL;  // empty list
  return data;
}

/* *********************************************************** */

void* queue_pop_tail(queue* q)
{
  assert(q);
  assert(q->length > 0);
  if (!q->tail) return NULL;
  void* data = q->tail->data;
  element_t* prev = q->tail->prev;
  if (prev) prev->next = NULL;
  free(q->tail);
  q->tail = prev;
  q->length--;
  if (!q->tail) q->head = NULL;  // empty list
  return data;
}

/* *********************************************************** */

int queue_length(const queue* q)
{
  assert(q);
  return q->length;
}

/* *********************************************************** */

bool queue_is_empty(const queue* q)
{
  assert(q);
  return (q->length == 0);
}

/* *********************************************************** */

void* queue_peek_head(queue* q)
{
  assert(q);
  assert(q->head);
  return q->head->data;
}

/* *********************************************************** */

void* queue_peek_tail(queue* q)
{
  assert(q);
  assert(q->tail);
  return q->tail->data;
}

/* *********************************************************** */

void queue_clear(queue* q)
{
  assert(q);
  element_t* e = q->head;
  while (e) {
    element_t* tmp = e;
    e = e->next;
    free(tmp);
  }
  q->head = q->tail = NULL;
  q->length = 0;
}

/* *********************************************************** */

void queue_clear_full(queue* q, void (*destroy)(void*))
{
  assert(q);
  element_t* e = q->head;
  while (e) {
    element_t* tmp = e;
    if (destroy) destroy(e->data);
    e = e->next;
    free(tmp);
  }
  q->head = q->tail = NULL;
  q->length = 0;
}

/* *********************************************************** */

void queue_free(queue* q)
{
  queue_clear(q);
  free(q);
}

/* *********************************************************** */

void queue_free_full(queue* q, void (*destroy)(void*))
{
  queue_clear_full(q, destroy);
  free(q);
}

/* *********************************************************** */
