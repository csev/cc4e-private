#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct mnode {
    char *key;
    int value;
    struct mnode *prev;
    struct mnode *next;
};

struct Map {
  // Methods
  struct mnode *head;
  struct mnode *tail;
  struct mnode *current;
  int count;
  int reverse;

  // Methods
  struct mnode* (*first)(struct Map* self);
  struct mnode* (*last)(struct Map* self);
  struct mnode* (*next)(struct Map* self);
  struct Map* (*dump)(struct Map* self);
  int (*get)(struct Map* self, char *key, int def);
  int (*size)(struct Map* self);
  struct Map* (*put)(struct Map* self, char *key, int value);
  struct Map* (*vsort)(struct Map* self);
  struct Map* (*ksort)(struct Map* self);
};

struct mnode* Map_first(struct Map* self)
{
    self->current = self->head;
    self->reverse = 0;
    return self->current;
}

struct mnode* Map_last(struct Map* self)
{
    self->current = self->tail;
    self->reverse = 1;
    return self->current;
}

struct mnode* Map_next(struct Map* self)
{
    if ( self->current == NULL) return NULL;
    if ( self->reverse == 0 ) {
        self->current = self->current->next;
    } else {
        self->current = self->current->prev;
    }

    return self->current;
}

struct Map* Map_dump(struct Map* self)
{
    struct mnode *cur;
    printf("Object Map@%p count=%d\n", self, self->count);
    for(cur = self->head; cur != NULL ; cur = cur->next ) {
         printf("  %s=%d\n", cur->key, cur->value);
    }
    return self;
}

struct mnode* Map_find(struct Map* self, char *key)
{
    struct mnode *cur;
    if ( key == NULL ) return NULL;
    for(cur = self->head; cur != NULL ; cur = cur->next ) {
        if(strcmp(key, cur->key) == 0 ) return cur;
    }
    return NULL;
}

// x.get(key) - Returns NULL if not found
int Map_get(struct Map* self, char *key, int def)
{
    struct mnode *retval = Map_find(self, key);
    if ( retval == NULL ) return def;
    return retval->value;
}

int Map_size(struct Map* self)
{
    return self->count;
}

// x[key] = value;
struct Map* Map_put(struct Map* self, char *key, int value) {
    
    struct mnode *old, *new;
    char *new_key, *new_value;

    if ( key == NULL ) return self;

    // First look up
    old = Map_find(self, key);
    if ( old != NULL ) {
        old->value = value;
        return self;
    }

    // Not found - time to insert
    new = malloc(sizeof(*new));
    new->next = NULL;
    if ( self->head == NULL ) self->head = new;
    if ( self->tail != NULL ) self->tail->next = new;
    new->prev = self->tail;
    self->tail = new;

    new_key = malloc(strlen(key)+1);
    strcpy(new_key, key);
    new->key = new_key;

    new->value = value;

    self->count++;

    return self; // To allow chaining
}

/* Swap the current node with current->next */
void Map_swap(struct Map* self, struct mnode* cur)
{

    struct mnode *prev, *next, *rest;

    if ( cur->next == NULL ) return;

    // Grab these before we start changing things
    next = cur->next;
    prev = cur->prev;
    rest = cur->next->next;
                              
    if ( prev != NULL ) {
        prev->next = next;
    } else {
        self->head = next;
    }

    cur->next = rest;
    cur->prev = next;

    next->next = cur;
    next->prev = prev;

    if ( rest != NULL ) {
        rest->prev = cur;
    } else {
        self->tail = cur;
    }
}

// Bubble sort by key - Order N**2
struct Map* Map_ksort(struct Map* self) {

    struct mnode *prev, *cur, *next, *rest;
    int i, swapped;

    if ( self->head == NULL ) return self;

    for (i=0; i<=self->count; i++) {
        swapped = 0;
        for(cur = self->head; cur != NULL ; cur = cur->next ) {
            if ( cur->next == NULL ) continue;  // Last item in the list
            // In order already
            if ( strcmp(cur->key, cur->next->key) <= 0 ) continue;

            // printf("Flipping %s %s\n", cur->key, cur->next->key);
            Map_swap(self, cur);
            swapped = 1;
        }
        // Stop early if nothing was swapped
        if ( swapped == 0 ) return self;
    }
    return self;
}

// Bubble sort by key - Order N**2
struct Map* Map_vsort(struct Map* self) {

    struct mnode *cur;
    int i;

    if ( self->head == NULL ) return self;

    for (i=0; i<=self->count; i++) {
        for(cur = self->head; cur != NULL ; cur = cur->next ) {
            if ( cur->next == NULL ) continue;  // Last item in the list
            // In order already
            if ( cur->value <= cur->next->value ) continue;
 
            // printf("Flipping %d %d\n", cur->value, cur->next->value);
            Map_swap(self, cur);
        }
    }
    return self;
}

/* Constructor */
struct Map * Map_new() {
    struct Map *p = malloc(sizeof(*p));

    p->head = NULL;
    p->tail = NULL;
    p->current = NULL;
    p->count = 0;
    p->reverse = 0;

    p->dump = &Map_dump;
    p->first = &Map_first;
    p->last = &Map_last;
    p->next = &Map_next;
    p->get = &Map_get;
    p->size = &Map_size;
    p->put = &Map_put;
    p->vsort = &Map_vsort;
    p->ksort = &Map_vsort;
    return p;
}

/* Destructor */
void Map_del(struct Map* self) {
    struct mnode *cur, *next;
    cur = self->head;
    while(cur) {
        free(cur->key);
        /* value is just part of the struct */
        next = cur->next;
        free(cur);
        cur = next;
    }
    free((void *)self);
}

int main(void)
{
    struct mnode *cur;
    struct Map * map = Map_new();

    printf("Testing Map class\n");
    map->put(map, "z", 8);
    map->put(map, "z", 1);
    map->put(map, "y", 2)->put(map, "b", 3);
    map->put(map, "a", 4);
    map->dump(map);

    printf("z=%d\n", map->get(map, "z", 42));
    printf("x=%d\n", map->get(map, "x", 42));

    printf("\nIterate forwards\n");
    for(cur = map->first(map); cur != NULL; cur = map->next(map) ) {
        printf(" %s=%d\n", cur->key, cur->value);
    }

    printf("\nIterate backwards\n");
    for(cur = map->last(map); cur != NULL; cur = map->next(map) ) {
        printf(" %s=%d\n", cur->key, cur->value);
    }

    map->ksort(map);
    printf("\nSorted by key\n");
    map->dump(map);

    printf("\nSorted by value\n");
    map->vsort(map)->dump(map);

    cur = map->first(map);
    printf("The smallest value is %s=%d\n", cur->key, cur->value);

    cur = map->last(map);
    printf("The largest value is %s=%d\n", cur->key, cur->value);

    Map_del(map);
}

// rm -f a.out ; gcc map.c; a.out ; rm -f a.out

