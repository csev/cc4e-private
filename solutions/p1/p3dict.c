#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct dnode {
    char *key;
    char *value;
};

struct p3dict {
   int alloc;
   int length;
   struct dnode *items;
};

/* Constructor - dct = dict() */
struct p3dict * p3dict_new() {
    int i;
    struct p3dict *p = malloc(sizeof(*p));
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
void p3dict_del(struct p3dict* self) {
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
void p3dict_print(struct p3dict* self)
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

int p3dict_len(const struct p3dict* self)
{
    return self->length;
}

/* find a node - used in get and put */
struct dnode *p3dict_find(struct p3dict* self, char *key)
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
char* p3dict_get(struct p3dict* self, char *key)
{
    struct dnode *retval = p3dict_find(self, key);
    if ( retval == NULL ) return NULL;
    return retval->value;
}

/* x[key] = value; Insert or replace the value associated with a key */
void p3dict_put(struct p3dict* self, char *key, char *value) {

    int i;
    struct dnode *old, *new;
    char *new_key, *new_value;

    if ( key == NULL || value == NULL ) return;

    /* First look up */
    old = p3dict_find(self, key);
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
    struct p3dict * dct = p3dict_new();
    p3dict_print(dct);
    p3dict_put(dct, "z", "Catch phrase");
    p3dict_print(dct);
    p3dict_put(dct, "z", "W");
    p3dict_print(dct);
    p3dict_put(dct, "y", "B");
    p3dict_put(dct, "c", "C");
    p3dict_put(dct, "a", "D");
    p3dict_print(dct);
    printf("Length =%d\n", p3dict_len(dct));

    printf("z=%s\n", p3dict_get(dct, "z"));
    printf("x=%s\n", p3dict_get(dct, "x"));

    /*
    printf("\nDump\n");
    for(cur = dct->head; cur != NULL ; cur = cur->next ) {
        printf("%s=%s\n", cur.key, cur.value);
    }
    */

    p3dict_del(dct);
}

// rm -f a.out ; gcc p3dict.c; a.out ; rm -f a.out


