#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct dnode {
    char *key;
    char *value;
};

struct guhash {
   int alloc;
   int length;
   struct dnode *items;
};

int getBucket(char *str, int buckets)
{
    unsigned int hash = 123456;
    if ( str == NULL ) return 0;
    for( ; *str ; str++) {
        hash = ( hash << 3 ) ^ *str;
    }
    return hash % buckets;
}

/* Constructor - dct = dict() */
struct guhash * guhash_new() {
    int i;
    struct guhash *p = malloc(sizeof(*p));
    p->length = 0;
    p->alloc = 8;
    p->items = malloc(p->alloc * sizeof(struct dnode));
    for(i=0; i < p->alloc; i++) {
        p->items[i].key = NULL;
        p->items[i].value = NULL;
    }
    return p;
}

/* Destructor - del(dct) */
void guhash_del(struct guhash* self) {
    int i;
    for(i=0; i < self->alloc; i++) {
        if ( self->items[i].key != NULL ) free(self->items[i].key);
        if ( self->items[i].value != NULL ) free(self->items[i].value);
    }
    free((void *) self->items);
    free((void *) self);
}

/* print(dct) */
/* {'z': 'W', 'y': 'B', 'c': 'C', 'a': 'D'} */
void guhash_print(struct guhash* self)
{
    int first = 1;
    int i;
    printf("{");
    for(i=0; i < self->alloc; i++) {
        if ( self->items[i].key == NULL ) continue;
        if ( ! first ) printf(", ");
        printf("'%s': ", self->items[i].key);
        printf("'%s'", self->items[i].value);
        first = 0;
    }
    printf("}\n");
}

int guhash_len(const struct guhash* self)
{
    return self->length;
}

/* find a node - used in get and put */
struct dnode *guhash_find(struct guhash* self, char *key)
{
    int i, bucket, offset;
    if ( key == NULL ) return NULL;
    bucket = getBucket(key, self->alloc);
    for(offset=0; offset < self->alloc; offset++) {
        i = (offset + bucket) % self->alloc;
        if ( self->items[i].key == NULL ) {
            return &(self->items[i]);
        }
        if(strcmp(key, self->items[i].key) == 0 ) {
            return &(self->items[i]);
        }
    }

    printf("Could not find empty or matching key - failure!!");
    return NULL;
}

/* x.get(key) - Returns NULL if not found */
char* guhash_get(struct guhash* self, char *key)
{
    struct dnode *retval = guhash_find(self, key);
    if ( retval == NULL || retval->key == NULL ) return NULL;
    return retval->value;
}

/* x[key] = value; Insert or replace the value associated with a key */
void guhash_put(struct guhash* self, char *key, char *value) {

    int i;
    struct dnode *old;

    if ( key == NULL || value == NULL ) return;

    /* First look up */
    old = guhash_find(self, key);
    if ( old != NULL && old->key != NULL ) {
        free(old->value);
        old->value = malloc(strlen(value)+1);
        strcpy(old->value, value);
        return;
    }

    /* Not found - time to insert */
    old->value = malloc(strlen(value)+1);
    strcpy(old->value, value);
    old->key =  malloc(strlen(key)+1);
    strcpy(old->key, key);

    self->length++;
}

int main(void)
{
    struct dnode * cur;
    struct guhash * dct = guhash_new();
    guhash_print(dct);
    guhash_put(dct, "z", "Catch phrase");
    guhash_print(dct);
    guhash_put(dct, "z", "W");
    guhash_print(dct);
    guhash_put(dct, "y", "B");
    guhash_put(dct, "c", "C");
    guhash_put(dct, "a", "D");
    guhash_print(dct);
    printf("Length =%d\n",guhash_len(dct));

    printf("z=%s\n", guhash_get(dct, "z"));
    printf("x=%s\n", guhash_get(dct, "x"));

    /*
    printf("\nDump\n");
    for(cur = dct->head; cur != NULL ; cur = cur->next ) {
        printf("%s=%s\n", cur.key, cur.value);
    }
    */

    guhash_del(dct);
}

// rm -f a.out ; gcc guhash.c; a.out ; rm -f a.out


