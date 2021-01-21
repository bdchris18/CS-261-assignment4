/*
 * In this file, you will write the structures and functions needed to
 * implement a priority queue.  Feel free to implement any helper functions
 * you need in this file to implement a priority queue.  Make sure to add your
 * name and @oregonstate.edu email address below:
 *
 * Name:
 * Email:
 */

#include <stdlib.h>
#include <stdio.h>

#include "pq.h"
#include "dynarray.h"

/*
 * This is the structure that represents a priority queue.  You must define
 * this struct to contain the data needed to implement a priority queue.
 */
struct pq{
  struct dynarray* array;
};

struct pq_node{
  void* value;
  int priority;
};


/*
 * This function should allocate and initialize an empty priority queue and
 * return a pointer to it.
 */
struct pq* pq_create() {
  struct pq* pq_array = (struct pq*)malloc(sizeof(struct pq));
  pq_array->array = dynarray_create();
  return pq_array;
}


/*
 * This function should free the memory allocated to a given priority queue.
 * Note that this function SHOULD NOT free the individual elements stored in
 * the priority queue.  That is the responsibility of the caller.
 *
 * Params:
 *   pq - the priority queue to be destroyed.  May not be NULL.
 */
void pq_free(struct pq* pq) {
  free(pq->array);
  free(pq);
  return;
}


/*
 * This function should return 1 if the specified priority queue is empty and
 * 0 otherwise.
 *
 * Params:
 *   pq - the priority queue whose emptiness is to be checked.  May not be
 *     NULL.
 *
 * Return:
 *   Should return 1 if pq is empty and 0 otherwise.
 */
int pq_isempty(struct pq* pq) {
  if(dynarray_size(pq->array) == 0){
    return 1;
  }
  return 0;
}


/*
 * This function should insert a given element into a priority queue with a
 * specified priority value.  Note that in this implementation, LOWER priority
 * values are assigned to elements with HIGHER priority.  In other words, the
 * element in the priority queue with the LOWEST priority value should be the
 * FIRST one returned.
 *
 * Params:
 *   pq - the priority queue into which to insert an element.  May not be
 *     NULL.
 *   value - the value to be inserted into pq.
 *   priority - the priority value to be assigned to the newly-inserted
 *     element.  Note that in this implementation, LOWER priority values
 *     should correspond to elements with HIGHER priority.  In other words,
 *     the element in the priority queue with the LOWEST priority value should
 *     be the FIRST one returned.
 */
void pq_insert(struct pq* pq, void* value, int priority) {
  struct pq_node* node = (struct pq_node*)malloc(sizeof(struct pq_node));
  node->priority = priority;
  node->value = value;
  dynarray_insert(pq->array, node);
  if(dynarray_size(pq->array) > 1){
    int parent_index = (dynarray_size(pq->array)-1)/2;
    struct pq_node* parent = dynarray_get(pq->array, parent_index);
    if(node->priority < parent->priority){
      int curr_index = dynarray_size(pq->array)-1;
      while(node->priority < parent->priority){
        if(parent->priority < node->priority) break;
        dynarray_set(pq->array, curr_index, parent);
        dynarray_set(pq->array, parent_index, node);
        curr_index = parent_index;
        parent_index = (parent_index-1)/2;
        parent = dynarray_get(pq->array, parent_index);
      }
    }
  }
  return;
}


/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with LOWEST priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   LOWEST priority value.
 */
void* pq_first(struct pq* pq) {
  struct pq_node* temp = dynarray_get(pq->array, 0);
  return temp->value;
}

void print_array(struct pq* pq){
  printf("Printing list\n");
  for(int i = 0; i < dynarray_size(pq->array); ++i){
    struct pq_node* temp = dynarray_get(pq->array, i);
    printf("Priority: %d\n", temp->priority);
  }
}


/*
 * This function should return the priority value of the first item in a
 * priority queue, i.e. the item with LOWEST priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a priority value.  May not be
 *     NULL or empty.
 *
 * Return:
 *   Should return the priority value of the first item in pq, i.e. the item
 *   with LOWEST priority value.
 */
int pq_first_priority(struct pq* pq) {
  struct pq_node* temp = dynarray_get(pq->array, 0);
  return temp->priority;
}


/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with LOWEST priority value, and then remove that item
 * from the queue.
 *
 * Params:
 *   pq - the priority queue from which to remove a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   LOWEST priority value.
 */
void* pq_remove_first(struct pq* pq) {
  if(dynarray_size(pq->array) == 0){
    free(pq->array);
    return NULL;
  }
  struct pq_node* root_node = dynarray_get(pq->array, 0);
  if(dynarray_size(pq->array) == 1){
    dynarray_remove(pq->array, 0);
    free(root_node);
    return root_node->value;
  }
  struct pq_node* replacement = dynarray_get(pq->array, dynarray_size(pq->array)-1);
  dynarray_remove(pq->array, dynarray_size(pq->array)-1);
  dynarray_set(pq->array, 0, replacement);
  int node_index = 0;
  int child1index = 0;
  int child2index = 0;
  struct pq_node* child1prio;
  struct pq_node* child2prio;
  while(replacement->priority > child1prio->priority || replacement->priority > child2prio->priority){
    if((child1index * 2) + 1 > dynarray_size(pq->array) || (child2index * 2) + 2 > dynarray_size(pq->array)){
      break;
    }
    child1index = (node_index * 2) + 1;
    child2index = (node_index * 2) + 2;
    if(child1index < dynarray_size(pq->array)){
      child1prio = dynarray_get(pq->array, child1index);
    }
    if(child2index < dynarray_size(pq->array)){
      child2prio = dynarray_get(pq->array, child2index);
    }
    if(child1prio->priority > child2prio->priority){
      struct pq_node* child = dynarray_get(pq->array, child2index);
      dynarray_set(pq->array, child2index, replacement);
      dynarray_set(pq->array, node_index, child);
      node_index = child2index;
    }
    else{
      struct pq_node* child = dynarray_get(pq->array, child1index);
      dynarray_set(pq->array, child1index, replacement);
      dynarray_set(pq->array, node_index, child);
      node_index = child1index;
    }
  }
  free(root_node);
  return root_node->value;
}
