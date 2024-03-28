#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct dnode {
    char *key;
    int value;
    struct dnode *prev;
    struct dnode *next;
};

struct hash {
    int buckets;
    struct dnode *heads[8];
    struct dnode *tails[8];
    int count;
};

void hash_del(struct hash* self)
{
    int i;
    struct dnode *cur, *next;

    for(i=0; i<self->buckets; i++) {
        cur = self->heads[i];
        while(cur) {
            free(cur->key);
            /* value is just part of the struct */
            next = cur->next;
            free(cur);
            cur = next;
        }
    }
    free((void *)self);
}

/** getBucket - Compute a hash bucket from a string */
int getBucket(char *str, int buckets)
{
    int hash = 42;
    if ( str == NULL ) return 0;
    for( ; *str ; str++) {
        hash = ( hash << 3 ) ^ *str;
    }
    return hash % buckets;
}

void hash_dump(struct hash* self)
{
    int i;
    struct dnode *cur;
    printf("Object hash@%p count=%d buckets=%d\n", self, self->count, self->buckets);
    for(i = 0; i < self->buckets; i++ ) {
        for(cur = self->heads[i]; cur != NULL ; cur = cur->next ) {
            printf(" %s=%d [%d]\n", cur->key, cur->value, i);
        }
    }
}

struct dnode* hash_find(struct hash* self, char *key, int bucket)
{
    struct dnode *cur;
    if ( self == NULL || key == NULL ) return NULL;
    for(cur = self->heads[bucket]; cur != NULL ; cur = cur->next ) {
        if(strcmp(key, cur->key) == 0 ) return cur;
    }
    return NULL;
}

/**
 * hash_put - Add or update an entry in the hash
 *
 * self - The pointer to the instance of this class.
 * key - A character pointer to the key value
 * value - The value to be stored with the associated key
 *
 * If the key is not in the hash, an entry is added.  If there
 * is already an entry in the hash for the key, the value
 * is updated.
 *
 * Sample call:
 *
 *    hash_put(map, "x", 42);
 *
 * This method takes inspiration from the Python code:
 *
 *   map["key"] = value
 */
void hash_put(struct hash* self, char *key, int value) {

    int bucket;
    struct dnode *old, *new;
    char *new_key;

    if ( key == NULL ) return;

    /* First look up */
    bucket = getBucket(key, self->buckets);
    old = hash_find(self, key, bucket);
    if ( old != NULL ) {
        old->value = value;
        return;
    }

    /* Not found - time to insert */
    new = malloc(sizeof(*new));
    new->next = NULL;

    if ( self->heads[bucket] == NULL ) self->heads[bucket] = new;
    if ( self->tails[bucket] != NULL ) self->tails[bucket]->next = new;
    new->prev = self->tails[bucket];
    self->tails[bucket] = new;

    new_key = malloc(strlen(key)+1);
    strcpy(new_key, key);
    new->key = new_key;

    new->value = value;

    self->count++;
}

/**
 * hash_get - Locate and return the value for the corresponding key or a default value
 *
 * self - The pointer to the instance of this class.
 * key - A character pointer to the key value
 * def - A default value to return if the key is not in the hash
 *
 * Returns an integer
 *
 * Sample call:
 * 
 * int ret = hash_get(map, "z", 42);
 *
 * This method takes inspiration from the Python code:
 *
 *   value = map.get("key", 42)
 */
int hash_get(struct hash* self, char *key, int def)
{
    int bucket = getBucket(key, self->buckets);
    struct dnode *retval = hash_find(self, key, bucket);
    if ( retval == NULL ) return def;
    return retval->value;
}

/**
 * hash_size - Return the number of entries in the hash as an integer
 *
 * self - The pointer to the instance of this class.
 *
 * This medhod is like the Python len() function, but we name it
 * size() to pay homage to Java.
 */
int hash_size(struct hash* self)
{
    return self->count;
}

struct hash * hash_new() {
    struct hash *p = malloc(sizeof(*p));

    p->buckets = 8;
    for(int i=0; i < p->buckets; i++ ) {
        p->heads[i] = NULL;
        p->tails[i] = NULL;
    }

    p->count = 0;
    return p;
}

/**
 * The main program to test and exercise the hash 
 * and dnode classes.
 */
int main(void)
{
    struct hash * map = hash_new();
    struct dnode *cur;

    hash_put(map, "z", 8);
    hash_put(map, "z", 1);
    hash_put(map, "y", 2);
    hash_put(map, "b", 3);
    hash_put(map, "a", 4);
    hash_dump(map);

    printf("z=%d\n", hash_get(map, "z", 42));
    printf("x=%d\n", hash_get(map, "x", 42));

    hash_del(map);
}

// rm -f a.out ; gcc pyhash.c; a.out ; rm -f a.out

