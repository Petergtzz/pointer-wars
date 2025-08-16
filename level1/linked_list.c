#include "linked_list.h"

#define UNUSED()

// Function pointers to (potentially) custom malloc() and
// free() functions.
//
static void * (*malloc_fptr)(size_t size) = malloc;
static void   (*free_fptr)(void* addr)    = free; 

bool linked_list_register_malloc(void *(*malloc)(size_t))
{
  if(!malloc) return false;
  malloc_fptr = malloc;
  return true;
}

bool linked_list_register_free(void (*free)(void*))
{
  if(!free) return false;
  free_fptr = free;
  return true;
}

struct linked_list *linked_list_create(void)
{
  struct linked_list *ll = malloc_fptr(sizeof(struct linked_list));
  if(!ll) return NULL;

  ll->head = NULL;
  return ll;
}

bool linked_list_delete(struct linked_list *ll)
{
  if(!ll) return false;

  struct node *temp = ll->head;
  while (temp != NULL)
  {
    struct node *next = temp->next;
    free_fptr(temp);
    temp = next;
  }

  free_fptr(ll);
  return true;
}

size_t linked_list_size(struct linked_list *ll)
{
  if(!ll) return SIZE_MAX;

  size_t size = 0;
  struct node *temp = ll->head;
  // NOTE: increment size until we encounter a NULL 
  while(temp != NULL)
  {
    size++;
    temp = temp->next;
  }

  return size;
}

bool linked_list_insert_end(struct linked_list *ll, unsigned int data)
{
  if(!ll) return false;

  struct node *new = malloc_fptr(sizeof(struct node));
  new->data = data;
  new->next = NULL;
 
  // NOTE: quick check for null, if true then we are at the end
  if(ll->head == NULL) ll->head = new;
  else 
  {
    struct node *temp = ll->head;
    while(temp->next != NULL)
    {
      temp = temp->next;
    }
    temp->next = new;
  }

  return true;
}

bool linked_list_insert_front(struct linked_list *ll, unsigned int data)
{
  if(!ll) return false;

  struct node *new = malloc_fptr(sizeof(struct node));
  if(!new) return false;

  new->data = data;
  new->next = ll->head;
  ll->head = new; 
  return true;
}

bool linked_list_insert(struct linked_list *ll, size_t index, unsigned int data)
{
  if(!ll) return false;

  struct node *new = malloc_fptr(sizeof(struct node));
  if(!new) return false;
  new->data = data;
  new->next = NULL;

  if(index == 0)
  {
    new->next = ll->head;
    ll->head = new;
    return true;
  }

  struct node *temp = ll->head;
  size_t pos = 0;
  while(temp != NULL && pos < index - 1)
  {
    temp = temp->next;
    pos++;
  }

  // If temp is NULL, index is out of range
  if (temp == NULL) {
    free_fptr(new);
    return false;
  }

  new->next = temp->next;
  temp->next = new;
  return true;
}

size_t linked_list_find(struct linked_list *ll, unsigned int data)
{
  if(!ll) return SIZE_MAX;

  struct node *temp = ll->head;
  size_t index = 0;
  while(temp != NULL)
  {
    if(temp->data == data) return index;
    temp = temp->next;
    index++;
  }
  return SIZE_MAX;
}

bool linked_list_remove(struct linked_list *ll, size_t index)
{
  if(!ll) return false;

  if(index == 0)
  {
    struct node *to_delete = ll->head;
    ll->head = ll->head->next;
    free_fptr(to_delete);
    return true;
  }

  struct node *temp = ll->head;
  for(size_t pos = 0; temp != NULL && pos < index - 1; pos++)
  {
    temp = temp->next;
  }

  if(!temp) return false;

  struct node *to_delete = temp->next;
  temp->next = temp->next->next;
  free_fptr(to_delete);
  return true;
}

// Creates an iterator struct at a particular index.
// \param linked_list : Pointer to linked_list.
// \param index       : Index of the linked list to start at.
// Returns pointer to an iterator on success, NULL otherwise.
//
struct iterator * linked_list_create_iterator(struct linked_list * ll,
                                              size_t index)
{
  if(!ll) return NULL;

  struct iterator *result = malloc_fptr(sizeof(struct iterator));
  if(!result) return NULL;

  result->ll = ll;
  result->current_node = ll->head;
  result->current_index = 0;

  while(result->current_node && result->current_index < index)
  {
    result->current_node = result->current_node->next;
    result->current_index++;
  }

  if(!result->current_node)
  {
    free_fptr(result);
    return NULL;
  }

  result->data = result->current_node->data;

  return result;
}

// Deletes an iterator struct.
// \param iterator : Iterator to delete.
// Returns TRUE on success, FALSE otherwise.
//
bool linked_list_delete_iterator(struct iterator * iter)
{
  if(!iter) return false;

  free_fptr(iter);
  return true;
}

// Iterates to the next node in the linked_list.
// \param iterator: Iterator to iterate on.
// Returns TRUE when next node is present, FALSE once end of list is reached.
//
bool linked_list_iterate(struct iterator * iter)
{
  if(!iter || !iter->current_node) return false;
  if(!iter->current_node->next) return false;

  iter->current_node = iter->current_node->next;
  iter->data = iter->current_node->data;
  iter->current_index++;

  return true;
}

int main(int argc, char **argv)
{
  (void)argc;
  (void)argv;

  linked_list_register_malloc(malloc);
  linked_list_register_free(free);
  
  return 0;
}



// NOTE: Why are we using a linked list struct instead of just using the node? 
// NOTE: Why use malloc that way? why not only call malloc whenever we want to allocate a new node? 
// why use that complicated function pointers? 
//  Why  would the iterator have a pointer to the current node plus a data field?


// (void)(malloc_fptr);
// (void)(free_fptr);
