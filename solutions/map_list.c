#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * This is our map entry for MapList<String,Integer>
 *
 * The key is a string / character array which is allocated using malloc()
 * when a new entry is created.
 */
struct MapListEntry {
    char *key;  /* public */
    int value;  /* public */
    struct MapListEntry *__prev;
    struct MapListEntry *__next;
};

/*
 * A MapListIter contains the current item and whether this is a forward or
 * reverse iterator.
 */
struct MapListIter {
   struct MapListEntry *__current;
   int __reverse;

   struct MapListEntry* (*next)(struct MapListIter* self);
   void (*del)(struct MapListIter* self);
};

/*
 * This is our MapList class
 */
struct MapList {
   /* Attributes */
   struct MapListEntry *__head;
   struct MapListEntry *__tail;
   int __count;

   /* Methods */
   void (*put)(struct MapList* self, char *key, int value);
   int (*get)(struct MapList* self, char *key, int def);
   int (*size)(struct MapList* self);
   void (*dump)(struct MapList* self);
   struct MapListIter* (*iter)(struct MapList* self);
   struct MapListIter* (*last)(struct MapList* self);
   void (*asort)(struct MapList* self);
   void (*ksort)(struct MapList* self);
   struct MapListEntry* (*index)(struct MapList* self, int position);
   void (*del)(struct MapList* self);
};

/**
 * Destructor for the MapList Class
 *
 * Loops through and frees all the keys and entries in the map.
 * The values are integers and so there is no need to free them.
 */
void __MapList_del(struct MapList* self) {
    struct MapListEntry *cur, *next;
    cur = self->__head;
    while(cur) {
        free(cur->key);
        /* value is just part of the struct */
        next = cur->__next;
        free(cur);
        cur = next;
    }
    free((void *)self);
}

/**
 * Destructor for the MapListIter Class
 */
void __MapListIter_del(struct MapListIter* self) {
    free((void *)self);
}

/**
 * map->dump - In effect a toString() except we print the contents of the MapList to stdout
 *
 * self - The pointer to the instance of this class.
 */

void __MapList_dump(struct MapList* self)
{
    struct MapListEntry *cur;
    printf("Object MapList@%p count=%d\n", self, self->__count);
    for(cur = self->__head; cur != NULL ; cur = cur->__next ) {
         printf("  %s=%d\n", cur->key, cur->value);
    }
}

/**
 * map->find - Locate and return the entry with the matching key or NULL if there is no entry
 *
 * self - The pointer to the instance of this class.
 * key - A character pointer to the key value
 *
 * Returns a MapListEntry or NULL.
 */
struct MapListEntry* __MapList_find(struct MapList* self, char *key)
{
    struct MapListEntry *cur;
    if ( self == NULL || key == NULL ) return NULL;
    for(cur = self->__head; cur != NULL ; cur = cur->__next ) {
        if(strcmp(key, cur->key) == 0 ) return cur;
    }
    return NULL;
}

/**
 * map->index - Locate and return the entry at the specified in the list
 *
 * self - The pointer to the instance of this class.
 * position - A zero-based position in the list
 *
 * Returns a MapListEntry or NULL.
 */
struct MapListEntry* __MapList_index(struct MapList* self, int position)
{
    int i;
    struct MapListEntry *cur;
    if ( self == NULL ) return NULL;
    for(cur = self->__head, i=0; cur != NULL ; cur = cur->__next, i++ ) {
        if ( i >= position ) return cur;
    }
    return NULL;
}

/**
 * map->put - Add or update an entry in the MapList
 *
 * self - The pointer to the instance of this class.
 * key - A character pointer to the key value
 * value - The value to be stored with the associated key
 *
 * If the key is not in the MapList, an entry is added.  If there
 * is already an entry in the MapList for the key, the value
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
void __MapList_put(struct MapList* self, char *key, int value) {

    struct MapListEntry *old, *new;
    char *new_key, *new_value;

    if ( key == NULL ) return;

    /* First look up */
    old = __MapList_find(self, key);
    if ( old != NULL ) {
        old->value = value;
        return;
    }

    /* Not found - time to insert */
    new = malloc(sizeof(*new));
    new->__next = NULL;
    if ( self->__head == NULL ) self->__head = new;
    if ( self->__tail != NULL ) self->__tail->__next = new;
    new->__prev = self->__tail;
    self->__tail = new;

    new_key = malloc(strlen(key)+1);
    strcpy(new_key, key);
    new->key = new_key;

    new->value = value;

    self->__count++;
}

/**
 * map->get - Locate and return the value for the corresponding key or a default value
 *
 * self - The pointer to the instance of this class.
 * key - A character pointer to the key value
 * def - A default value to return if the key is not in the MapList
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
int __MapList_get(struct MapList* self, char *key, int def)
{
    struct MapListEntry *retval = __MapList_find(self, key);
    if ( retval == NULL ) return def;
    return retval->value;
}

/**
 * map->size - Return the number of entries in the MapList as an integer
 *
 * self - The pointer to the instance of this class.
 *
 * This medhod is like the Python len() function, but we name it
 * size() to pay homage to Java.
 */
int __MapList_size(struct MapList* self)
{
    return self->__count;
}

/**
 * MapListIter_next - Advance the iterator forwards
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
struct MapListEntry* __MapListIter_next(struct MapListIter* self)
{
    struct MapListEntry * retval = self->__current;

    if ( retval == NULL) return NULL;
    if ( self->__reverse == 0 ) {
        self->__current = self->__current->__next;
    } else {
        self->__current = self->__current->__prev;
    }

    return retval;
}

/**
 * map->iter - Create an iterator from the head of the MapList
 *
 * self - The pointer to the instance of this class.
 *
 * returns NULL when there are no entries in the MapList
 *
 * This is inspired by the following Python code
 * that creates an iterator from a dictionary:
 *
 *     x = {'a': 1, 'b': 2, 'c': 3}
 *     it = iter(x)
 */
struct MapListIter* __MapList_iter(struct MapList* self)
{
    struct MapListIter *iter = malloc(sizeof(*iter));
    iter->__current = self->__head;
    iter->__reverse = 0;
    iter->next = &__MapListIter_next;
    iter->del = &__MapListIter_del;
    return iter;
}

/**
 * map->last - Start an iterator at the tail of the
 * MapList and mark the iterator as "going backwards"
 *
 * self - The pointer to the instance of this class.
 *
 * returns NULL when there are no entries in the MapList
 *
 * This is inspired by the following Python code:
 *
 *     x = {'a': 1, 'b': 2, 'c': 3}
 *     it = iter(reversed(x))
 */
struct MapListIter* __MapList_last(struct MapList* self)
{
    struct MapListIter *iter = malloc(sizeof(*iter));
    iter->__current = self->__tail;
    iter->__reverse = 1;
    iter->next = &__MapListIter_next;
    iter->del = &__MapListIter_del;
    return iter;
}

/**
 * __MapList_swap - Swap the current MapListEntry with the its successor in the MapList
 *
 * self - The pointer to the instance of this class.
 * cur - A MapListEntry in the MapList
 *
 * This code must deal with cur being the first item in the MapList
 * is the last item in the MapList.
 */
void __MapList_swap(struct MapList* self, struct MapListEntry* cur)
{

    struct MapListEntry *prev, *next, *rest;

    /* Guardian pattern */
    if ( cur == NULL || cur->__next == NULL ) return;

    /* Grab these before we start changing things */
    next = cur->__next;
    prev = cur->__prev;
    rest = cur->__next->__next;

    if ( prev != NULL ) {
        prev->__next = next;
    } else {
        self->__head = next;
    }

    cur->__next = rest;
    cur->__prev = next;

    next->__next = cur;
    next->__prev = prev;

    if ( rest != NULL ) {
        rest->__prev = cur;
    } else {
        self->__tail = cur;
    }
}

/**
 * map->ksort - Sort the list so that the keys are low to high
 *
 * self - The pointer to the instance of this class.
 *
 * This code uses a lame, N-squared rock sort for simplicity.
 * The outer loop is a conunted loop that runs size() times.
 * The inner loop goes through the map comparing successive
 * elements and when a pair of elements is in the wrong order they
 * are swapped.  The inner loop in effect insures that the largest
 * value tumbles down to the bottom of the MapList.  And if this inner
 * loop is done size() times we are assured that the MapList is sorted.
 * Is one tiny optimization, if we get through the inner loop with
 * no swaps, we can exit the outer loop.
 *
 * The inspiration for the name of this routine comes from the PHP
 * ksort() function.
 */
void __MapList_ksort(struct MapList* self) {

    struct MapListEntry *prev, *cur, *next, *rest;
    int i, swapped;

    if ( self->__head == NULL ) return;

    for (i=0; i<=self->__count; i++) {
        swapped = 0;
        for(cur = self->__head; cur != NULL ; cur = cur->__next ) {
            if ( cur->__next == NULL ) continue;  // Last item in the list
            // In order already
            if ( strcmp(cur->key, cur->__next->key) <= 0 ) continue;

            // printf("Flipping %s %s\n", cur->key, cur->next->key);
            __MapList_swap(self, cur);
            swapped = 1;
        }
        // Stop early if nothing was swapped
        if ( swapped == 0 ) return;
    }
}

/**
 * map->asort - Sort the list so that the values are low to high
 *
 * self - The pointer to the instance of this class.
 *
 * This code uses a lame, N-squared rock sort for simplicity.
 * The outer loop is a conunted loop that runs size() times.
 * The inner loop goes through the map comparing successive
 * elements and when a pair of elements is in the wrong order they
 * are swapped.  The inner loop in effect insures that the largest
 * value tumbles down to the bottom of the MapList.  And if this inner
 * loop is done size() times we are assured that the MapList is sorted.
 a Is one tiny optimization, if we get through the inner loop with
 * no swaps, we can exit the outer loop.
 *
 * The inspiration for the name of this routine comes from the (poorly
 * named) PHP * asort() function.
 */
void __MapList_asort(struct MapList* self) {

    struct MapListEntry *cur;
    int i;

    if ( self->__head == NULL ) return;

    for (i=0; i<=self->__count; i++) {
        for(cur = self->__head; cur != NULL ; cur = cur->__next ) {
            if ( cur->__next == NULL ) continue;  // Last item in the list
            // In order already
            if ( cur->value <= cur->__next->value ) continue;

            // printf("Flipping %d %d\n", cur->value, cur->__next->value);
            __MapList_swap(self, cur);
        }
    }
}

/**
 * Constructor for the MapList Class
 *
 * Initialized both the attributes and methods
 */
struct MapList * MapList_new() {
    struct MapList *p = malloc(sizeof(*p));

    p->__head = NULL;
    p->__tail = NULL;
    p->__count = 0;

    p->put = &__MapList_put;
    p->get = &__MapList_get;
    p->size = &__MapList_size;
    p->dump = &__MapList_dump;
    p->iter = &__MapList_iter;
    p->last = &__MapList_last;
    p->asort = &__MapList_asort;
    p->ksort = &__MapList_ksort;
    p->index = &__MapList_index;
    p->del = &__MapList_del;
    return p;
}

/**
 * The main program to test and exercise the MapList 
 * and MapListEntry classes.
 */
int main(void)
{
    struct MapList * map = MapList_new();
    struct MapListEntry *cur;
    struct MapListIter *iter;

    map->put(map, "z", 8);
    map->put(map, "z", 1);
    map->put(map, "y", 2);
    map->put(map, "b", 3);
    map->put(map, "a", 4);
    map->dump(map);

    printf("z=%d\n", map->get(map, "z", 42));
    printf("x=%d\n", map->get(map, "x", 42));

    printf("\nIterate forwards\n");
    iter = map->iter(map);
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

// rm -f a.out ; gcc map_list.c; a.out ; rm -f a.out

