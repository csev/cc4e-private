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
    char *key;  /* public */
    int value;  /* public */
    struct MapEntry *__prev;
    struct MapEntry *__next;
};

/*
 * A MapIter contains the current item and whether this is a forward or
 * reverse iterator.
 */
struct MapIter {
    int __bucket;
    struct Map *__map;
    struct MapEntry *__current;

    struct MapEntry* (*next)(struct MapIter* self);
    void (*del)(struct MapIter* self);
};

/*
 * This is our Map class
 */
struct Map {
    /* Attributes */
    int __buckets;
    struct MapEntry *__heads[8];
    struct MapEntry *__tails[8];
    int __count;

    /* Methods */
    void (*put)(struct Map* self, char *key, int value);
    int (*get)(struct Map* self, char *key, int def);
    int (*size)(struct Map* self);
    void (*dump)(struct Map* self);
    struct MapIter* (*first)(struct Map* self);
    void (*del)(struct Map* self);
};

/**
 * Destructor for the Map Class
 *
 * Loops through and frees all the keys and entries in the map.
 * The values are integers and so there is no need to free them.
 */
void __Map_del(struct Map* self)
{
    int i;
    struct MapEntry *cur, *next;

    for(i=0; i<self->__buckets; i++) {
        cur = self->__heads[i];
        while(cur) {
            free(cur->key);
            /* value is just part of the struct */
            next = cur->__next;
            free(cur);
            cur = next;
        }
    }
    free((void *)self);
}

/**
 * Destructor for the MapIter Class
 */
void __MapIter_del(struct MapIter* self) {
    free((void *)self);
}

/** getHash - Compute a hash from a string */
int getHash(char *str)
{
    int hash = 42;
    if ( str == NULL ) return 0;
    for( ; *str ; str++) {
        hash = ( hash << 3 ) ^ *str;
    }
    return hash;
}

/**
 * map->dump - In effect a toString() except we print the contents of the Map to stdout
 *
 * self - The pointer to the instance of this class.
 */

void __Map_dump(struct Map* self)
{
    int i;
    struct MapEntry *cur;
    printf("Object HashMap@%p count=%d buckets=%d\n", self, self->__count, self->__buckets);
    for(i = 0; i < self->__buckets; i++ ) {
        for(cur = self->__heads[i]; cur != NULL ; cur = cur->__next ) {
            printf(" %s=%d [%d]\n", cur->key, cur->value, i);
        }
    }
}

/**
 * map->find - Locate and return the entry with the matching key or NULL if there is no entry
 *
 * self - The pointer to the instance of this class.
 * key - A character pointer to the key value
 * bucket - The hash bucket
 *
 * Returns a MapEntry or NULL.
 */
struct MapEntry* __Map_find(struct Map* self, char *key, int bucket)
{
    struct MapEntry *cur;
    if ( self == NULL || key == NULL ) return NULL;
    for(cur = self->__heads[bucket]; cur != NULL ; cur = cur->__next ) {
        if(strcmp(key, cur->key) == 0 ) return cur;
    }
    return NULL;
}

/**
 * map->put - Add or update an entry in the Map
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
void __Map_put(struct Map* self, char *key, int value) {

    int bucket;
    struct MapEntry *old, *new;
    char *new_key, *new_value;

    bucket = getHash(key) % self->__buckets;

    if ( key == NULL ) return;

    /* First look up */
    old = __Map_find(self, key, bucket);
    if ( old != NULL ) {
        old->value = value;
        return;
    }

    /* Not found - time to insert */
    new = malloc(sizeof(*new));
    new->__next = NULL;

    if ( self->__heads[bucket] == NULL ) self->__heads[bucket] = new;
    if ( self->__tails[bucket] != NULL ) self->__tails[bucket]->__next = new;
    new->__prev = self->__tails[bucket];
    self->__tails[bucket] = new;

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
int __Map_get(struct Map* self, char *key, int def)
{
    int bucket = getHash(key) % self->__buckets;
    struct MapEntry *retval = __Map_find(self, key, bucket);
    if ( retval == NULL ) return def;
    return retval->value;
}

/**
 * map->size - Return the number of entries in the Map as an integer
 *
 * self - The pointer to the instance of this class.
 *
 * This medhod is like the Python len() function, but we name it
 * size() to pay homage to Java.
 */
int __Map_size(struct Map* self)
{
    return self->__count;
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
struct MapEntry* __MapIter_next(struct MapIter* self)
{
    if ( self->__current != NULL ) {
        self->__current = self->__current->__next;
        if ( self->__current != NULL ) return self->__current;
    }

    // We might be at the end of a chain so advance the bucket until 
    // we find a non-empty bucket
    while ( self->__current == NULL) {
        if ( self->__bucket >= self->__map->__buckets ) return NULL;
        self->__bucket++;
        self->__current = self->__map->__heads[self->__bucket];
    }
    return self->__current;
}

/**
 * map->first - Create an iterator from the head of
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
struct MapIter* __Map_first(struct Map* map)
{
    struct MapIter *iter = malloc(sizeof(*iter));
    iter->__map = map;
    iter->__bucket = 0;
    iter->__current = map->__heads[iter->__bucket];
    iter->next = &__MapIter_next;
    iter->del = &__MapIter_del;
    return iter;
}

/**
 * Constructor for the Map Class
 *
 * Initialized both the attributes and methods
 */
struct Map * Map_new() {
    struct Map *p = malloc(sizeof(*p));

    p->__buckets = 8;
    for(int i=0; i < p->__buckets; i++ ) {
        p->__heads[i] = NULL;
        p->__tails[i] = NULL;
    }

    p->__count = 0;

    p->put = &__Map_put;
    p->get = &__Map_get;
    p->size = &__Map_size;
    p->dump = &__Map_dump;
    p->first = &__Map_first;
    p->del = &__Map_del;
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

    map->del(map);
}

// rm -f a.out ; gcc map_hash.c; a.out ; rm -f a.out

