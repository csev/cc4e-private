#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct dnode {
    char *key;
    char *value;
};

struct gudict {
   int alloc;
   int length;
   struct dnode *items;
};

/* Constructor - dct = dict() */
struct gudict * gudict_new() {
    int i;
    struct gudict *p = malloc(sizeof(*p));
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
void gudict_del(struct gudict* self) {
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
void gudict_print(struct gudict* self)
{
    int first = 1;
    int i;
    printf("{");
    for(i=0; i < self->length; i++) {
         if ( ! first ) printf(", ");
         printf("'%s': ", self->items[i].key);
         printf("'%s'", self->items[i].value);
         first = 0;
    }
    printf("}\n");
}

int gudict_len(const struct gudict* self)
{
    return self->length;
}

/* find a node - used in get and put */
struct dnode *gudict_find(struct gudict* self, char *key)
{
    int i;
    if ( key == NULL ) return NULL;
    for(i=0; i < self->alloc; i++) {
        if ( self->items[i].key == NULL ) continue;
        if(strcmp(key, self->items[i].key) == 0 ) return &(self->items[i]);
    }
    return NULL;
}

/* x.get(key) - Returns NULL if not found */
char* gudict_get(struct gudict* self, char *key)
{
    struct dnode *retval = gudict_find(self, key);
    if ( retval == NULL ) return NULL;
    return retval->value;
}

/* x[key] = value; Insert or replace the value associated with a key */
void gudict_put(struct gudict* self, char *key, char *value) {

    int i;
    struct dnode *old, *new;
    char *new_key, *new_value;

    if ( key == NULL || value == NULL ) return;

    /* First look up */
    old = gudict_find(self, key);
    if ( old != NULL ) {
        free(old->value);
        new_value = malloc(strlen(value)+1);
        strcpy(new_value, value);
        old->value = new_value;
        return;
    }

    /* Not found - time to insert */
    new_value = malloc(strlen(value)+1);
    strcpy(new_value, value);

    new_key = malloc(strlen(key)+1);
    strcpy(new_key, key);

    self->items[self->length].key = new_key;
    self->items[self->length].value = new_value;

    self->length++;
}

int main(void)
{
    struct dnode * cur;
    struct gudict * dct = gudict_new();
    gudict_print(dct);
    gudict_put(dct, "z", "Catch phrase");
    gudict_print(dct);
    gudict_put(dct, "z", "W");
    gudict_print(dct);
    gudict_put(dct, "y", "B");
    gudict_put(dct, "c", "C");
    gudict_put(dct, "a", "D");
    gudict_print(dct);
    printf("Length =%d\n",gudict_len(dct));

    printf("z=%s\n", gudict_get(dct, "z"));
    printf("x=%s\n", gudict_get(dct, "x"));

    /*
    printf("\nDump\n");
    for(cur = dct->head; cur != NULL ; cur = cur->next ) {
        printf("%s=%s\n", cur.key, cur.value);
    }
    */

    gudict_del(dct);
}

// rm -f a.out ; gcc gudict.c; a.out ; rm -f a.out


