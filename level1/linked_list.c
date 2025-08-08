#include "linked_list.h"

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
  linked_list_register_free(free);
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

  

  return true;
}



int main(int argc, char **argv)
{
  if(argc > 0)
  {
    linked_list_register_malloc(malloc);

    struct linked_list *ll = linked_list_create();
    linked_list_insert_front(ll, 10);
    linked_list_insert_front(ll, 20);
    linked_list_insert_end(ll, 80);
    linked_list_insert_front(ll, 40);
    linked_list_insert_end(ll, 100);
    size_t size = linked_list_size(ll);
    printf("Size: %zu\n", size);
    struct node *temp = ll->head;
    while(temp != NULL)
    {
      printf("%d -> ", temp->data);
      temp = temp->next;
    }
    printf("NULL\n");
    

    linked_list_delete(ll);
  }

  return 0;
}



// NOTE: Why are we using a linked list struct instead of just using the node? 
// NOTE: Why use malloc that way? why not only call malloc whenever we want to allocate a new node? 
// why use that complicated function pointers? 
//


// (void)(malloc_fptr);
// (void)(free_fptr);
