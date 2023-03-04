#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * This is our map entry for Map<String,Integer>
 *
 * The key is a string / character array which is allocated using malloc()
 * when a new entry is created.
 */
struct MapEntry {
    char *key;
    int value;
    struct MapEntry *prev;
    struct MapEntry *next;
};

/*
 * A MapIter contains the current item and whether this is a forward or
 * reverse iterator.
 */
struct MapIter {
   struct MapEntry *current;
   int reverse;

   struct MapEntry* (*next)(struct MapIter* self);
   void (*del)(struct MapIter* self);
};

/*
 * This is our Map class
 */
struct Map {
   /* Attributes */
   struct MapEntry *head;
   struct MapEntry *tail;
   int count;

   /* Methods */
   void (*put)(struct Map* self, char *key, int value);
   int (*get)(struct Map* self, char *key, int def);
   int (*size)(struct Map* self);
   void (*dump)(struct Map* self);
   struct MapIter* (*first)(struct Map* self);
   struct MapIter* (*last)(struct Map* self);
   void (*asort)(struct Map* self);
   void (*ksort)(struct Map* self);
   struct MapEntry* (*index)(struct Map* self, int position);
   void (*del)(struct Map* self);
};

/**
 * Destructor for the Map Class
 *
 * Loops through and frees all the keys and entries in the map.
 * The values are integers and so there is no need to free them.
 */
void Map_del(struct Map* self) {
    struct MapEntry *cur, *next;
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

/**
 * Destructor for the MapIter Class
 */
void MapIter_del(struct MapIter* self) {
    free((void *)self);
}

/**
 * Map_dump - In effect a toString() except we print the contents of the Map to stdout
 *
 * self - The pointer to the instance of this class.
 */

void Map_dump(struct Map* self)
{
    struct MapEntry *cur;
    printf("Object Map@%p count=%d\n", self, self->count);
    for(cur = self->head; cur != NULL ; cur = cur->next ) {
         printf("  %s=%d\n", cur->key, cur->value);
    }
}

/**
 * Map_find - Locate and return the entry with the matching key or NULL if there is no entry
 *
 * self - The pointer to the instance of this class.
 * key - A character pointer to the key value
 *
 * Returns a MapEntry or NULL.
 */
struct MapEntry* Map_find(struct Map* self, char *key)
{
    struct MapEntry *cur;
    if ( self == NULL || key == NULL ) return NULL;
    for(cur = self->head; cur != NULL ; cur = cur->next ) {
        if(strcmp(key, cur->key) == 0 ) return cur;
    }
    return NULL;
}

/**
 * Map_index - Locate and return the entry at the specified in the list
 *
 * self - The pointer to the instance of this class.
 * position - A zero-based position in the list
 *
 * Returns a MapEntry or NULL.
 */
struct MapEntry* Map_index(struct Map* self, int position)
{
    int i;
    struct MapEntry *cur;
    if ( self == NULL ) return NULL;
    for(cur = self->head, i=0; cur != NULL ; cur = cur->next, i++ ) {
        if ( i >= position ) return cur;
    }
    return NULL;
}

/**
 * Map_put - Add or update an entry in the Map
 *
 * self - The pointer to the instance of this class.
 * key - A character pointer to the key value
 * value - The value to be stored with the associated key
 *
 * If the key is not in the Map, an entry is added.  If there
 * is already an entry in the Map for the key, the value
 * is updated.
 *
 * Sample call:
 *
 *    map->put(map, "x", 42);
 *
 * This method takes inspiration from the Python code:
 *
 *   map["key"] = value
 */
void Map_put(struct Map* self, char *key, int value) {

    struct MapEntry *old, *new;
    char *new_key, *new_value;

    if ( key == NULL ) return;

    /* First look up */
    old = Map_find(self, key);
    if ( old != NULL ) {
        old->value = value;
        return;
    }

    /* Not found - time to insert */
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
}

/**
 * Map_get - Locate and return the value for the corresponding key or a default value
 *
 * self - The pointer to the instance of this class.
 * key - A character pointer to the key value
 * def - A default value to return if the key is not in the Map
 *
 * Returns an integer
 *
 * Sample call:
 * 
 * int ret = map->get(map, "z", 42);
 *
 * This method takes inspiration from the Python code:
 *
 *   value = map.get("key", 42)
 */
int Map_get(struct Map* self, char *key, int def)
{
    struct MapEntry *retval = Map_find(self, key);
    if ( retval == NULL ) return def;
    return retval->value;
}

/**
 * Map_size - Return the number of entries in the Map as an integer
 *
 * self - The pointer to the instance of this class.
 *
 * This medhod is like the Python len() function, but we name it
 * size() to pay homage to Java.
 */
int Map_size(struct Map* self)
{
    return self->count;
}

/**
 * MapIter_next - Advance the iterator forwards
 * or backwards and return the next item
 *
 * self - The pointer to the instance of this class.
 *
 * returns NULL when there are no more entries
 *
 * This is inspired by the following Python code:
 *
 *   item = next(iterator, False)
 */
struct MapEntry* MapIter_next(struct MapIter* self)
{
    if ( self->current == NULL) return NULL;
    if ( self->reverse == 0 ) {
        self->current = self->current->next;
    } else {
        self->current = self->current->prev;
    }

    return self->current;
}

/**
 * Map_first - Create an iterator from the head of
 * the Map and return the first item
 *
 * self - The pointer to the instance of this class.
 *
 * returns NULL when there are no entries in the Map
 *
 * This is inspired by the following Python code
 * that creates an iterator from a dictionary:
 *
 *     x = {'a': 1, 'b': 2, 'c': 3}
 *     it = iter(x)
 */
struct MapIter* Map_first(struct Map* self)
{
    struct MapIter *iter = malloc(sizeof(*iter));
    iter->current = self->head;
    iter->reverse = 0;
    iter->next = &MapIter_next;
    iter->del = &MapIter_del;
    return iter;
}

/**
 * Map_last - Start an iterator at the tail of the
 * Map and mark the iterator as "going backwards"
 *
 * self - The pointer to the instance of this class.
 *
 * returns NULL when there are no entries in the Map
 *
 * This is inspired by the following Python code:
 *
 *     x = {'a': 1, 'b': 2, 'c': 3}
 *     it = iter(reversed(x))
 */
struct MapIter* Map_last(struct Map* self)
{
    struct MapIter *iter = malloc(sizeof(*iter));
    iter->current = self->tail;
    iter->reverse = 1;
    iter->next = &MapIter_next;
    iter->del = &MapIter_del;
    return iter;
}

/**
 * Map_swap - Swap the current MapEntry with the its successor in the Map
 *
 * self - The pointer to the instance of this class.
 * cur - A MapEntry in the Map
 *
 * This code must deal with cur being the first item in the Map
 * is the last item in the Map.
 */
void Map_swap(struct Map* self, struct MapEntry* cur)
{

    struct MapEntry *prev, *next, *rest;

    /* Guardian pattern */
    if ( cur == NULL || cur->next == NULL ) return;

    /* Grab these before we start changing things */
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

/**
 * Map_ksort - Sort the list so that the keys are low to high
 *
 * self - The pointer to the instance of this class.
 *
 * This code uses a lame, N-squared rock sort for simplicity.
 * The outer loop is a conunted loop that runs size() times.
 * The inner loop goes through the map comparing successive
 * elements and when a pair of elements is in the wrong order they
 * are swapped.  The inner loop in effect insures that the largest
 * value tumbles down to the bottom of the Map.  And if this inner
 * loop is done size() times we are assured that the Map is sorted.
 * Is one tiny optimization, if we get through the inner loop with
 * no swaps, we can exit the outer loop.
 *
 * The inspiration for the name of this routine comes from the PHP
 * ksort() function.
 */
void Map_ksort(struct Map* self) {

    struct MapEntry *prev, *cur, *next, *rest;
    int i, swapped;

    if ( self->head == NULL ) return;

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
        if ( swapped == 0 ) return;
    }
}

/**
 * Map_asort - Sort the list so that the values are low to high
 *
 * self - The pointer to the instance of this class.
 *
 * This code uses a lame, N-squared rock sort for simplicity.
 * The outer loop is a conunted loop that runs size() times.
 * The inner loop goes through the map comparing successive
 * elements and when a pair of elements is in the wrong order they
 * are swapped.  The inner loop in effect insures that the largest
 * value tumbles down to the bottom of the Map.  And if this inner
 * loop is done size() times we are assured that the Map is sorted.
 a Is one tiny optimization, if we get through the inner loop with
 * no swaps, we can exit the outer loop.
 *
 * The inspiration for the name of this routine comes from the (poorly
 * named) PHP * asort() function.
 */
void Map_asort(struct Map* self) {

    struct MapEntry *cur;
    int i;

    if ( self->head == NULL ) return;

    for (i=0; i<=self->count; i++) {
        for(cur = self->head; cur != NULL ; cur = cur->next ) {
            if ( cur->next == NULL ) continue;  // Last item in the list
            // In order already
            if ( cur->value <= cur->next->value ) continue;

            // printf("Flipping %d %d\n", cur->value, cur->next->value);
            Map_swap(self, cur);
        }
    }
}

/**
 * Constructor for the Map Class
 *
 * Initialized both the attributes and methods
 */
struct Map * Map_new() {
    struct Map *p = malloc(sizeof(*p));

    p->head = NULL;
    p->tail = NULL;
    p->count = 0;

    p->put = &Map_put;
    p->get = &Map_get;
    p->size = &Map_size;
    p->dump = &Map_dump;
    p->first = &Map_first;
    p->last = &Map_last;
    p->asort = &Map_asort;
    p->ksort = &Map_ksort;
    p->index = &Map_index;
    p->del = &Map_del;
    return p;
}


/**
 * The main program to test and exercise the Map 
 * and MapEntry classes.
 */
int main(void)
{
    struct Map * map = Map_new();
    struct MapEntry *cur;
    struct MapIter *iter;

    printf("Testing Map class\n");
    map->put(map, "z", 8);
    map->put(map, "z", 1);
    map->put(map, "y", 2);
    map->put(map, "b", 3);
    map->put(map, "a", 4);
    map->dump(map);

    printf("z=%d\n", map->get(map, "z", 42));
    printf("x=%d\n", map->get(map, "x", 42));

    printf("\nIterate forwards\n");
    iter = map->first(map);
    while(1) {
        cur = iter->next(iter);
        if ( cur == NULL ) break;
        printf(" %s=%d\n", cur->key, cur->value);
    }
    iter->del(iter);

    printf("\nIterate backwards\n");
    iter = map->last(map);
    while(1) {
        cur = iter->next(iter);
        if ( cur == NULL ) break;
        printf(" %s=%d\n", cur->key, cur->value);
    }
    iter->del(iter);

    map->ksort(map);
    printf("\nSorted by key\n");
    map->dump(map);

    printf("\nSorted by value\n");
    map->asort(map);
    map->dump(map);

    cur = map->index(map, 0);
    printf("The smallest value is %s=%d\n", cur->key, cur->value);

    int pos = map->size(map) - 1;
    cur = map->index(map, pos);
    printf("The largest value is %s=%d\n", cur->key, cur->value);

    map->del(map);
}

// rm -f a.out ; gcc map.c; a.out ; rm -f a.out

