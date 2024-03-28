#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct dnode {
    char *key;
    char *value;
    struct dnode *next;
};

struct listkv {
  struct dnode *head;
  struct dnode *tail;
  int count;
};

/* Constructor - dct = dict() */
struct listkv * listkv_new() {
    struct listkv *p = malloc(sizeof(*p));
    p->head = NULL;
    p->tail = NULL;
    p->count = 0;
    return p;
}

/* Destructor - del(dct) */
void listkv_del(struct listkv* self) {
    struct dnode *cur, *next;
    cur = self->head;
    while(cur) {
        free(cur->key);
        free(cur->value);
        next = cur->next;
        free(cur);
        cur = next;
    }
    free((void *)self);
}

/* print(dct) */
/* {'z': 'W', 'y': 'B', 'c': 'C', 'a': 'D'} */
void listkv_print(struct listkv* self)
{
    int first = 1;
    struct dnode *cur;
    printf("{");
    for(cur = self->head; cur != NULL ; cur = cur->next ) {
         if ( ! first ) printf(", ");
         printf("'%s': ", cur->key);
         printf("'%s'", cur->value);
         first = 0;
    }
    printf("}\n");
}

int listkv_len(const struct listkv* self)
{
    return self->count;
}

/* find a node - used in get and put */
struct dnode* listkv_find(struct listkv* self, char *key)
{
    struct dnode *cur;
    if ( key == NULL ) return NULL;
    for(cur = self->head; cur != NULL ; cur = cur->next ) {
        if(strcmp(key, cur->key) == 0 ) return cur;
    }
    return NULL;
}

/* x.get(key) - Returns NULL if not found */
char* listkv_get(struct listkv* self, char *key)
{
    struct dnode *retval = listkv_find(self, key);
    if ( retval == NULL ) return NULL;
    return retval->value;
}

/* x[key] = value; Insert or replace the value associated with a key */
void listkv_put(struct listkv* self, char *key, char *value) {

    struct dnode *old, *new;
    char *new_key, *new_value;

    if ( key == NULL || value == NULL ) return;

    /* First look up */
    old = listkv_find(self, key);
    if ( old != NULL ) {
        free(old->value);
        new_value = malloc(strlen(value)+1);
        strcpy(new_value, value);
        old->value = new_value;
        return;
    }

    /* Not found - time to insert */
    new = malloc(sizeof(*new));
    new->next = NULL;
    if ( self->head == NULL ) self->head = new;
    if ( self->tail != NULL ) self->tail->next = new;
    self->tail = new;

    new_value = malloc(strlen(value)+1);
    strcpy(new_value, value);
    new->value = new_value;

    new_key = malloc(strlen(key)+1);
    strcpy(new_key, key);
    new->key = new_key;

    self->count++;
}

int main(void)
{
    struct dnode * cur;
    struct listkv * dct = listkv_new();
    listkv_put(dct, "z", "Catch phrase");
    listkv_print(dct);
    listkv_put(dct, "z", "W");
    listkv_print(dct);
    listkv_put(dct, "y", "B");
    listkv_put(dct, "c", "C");
    listkv_put(dct, "a", "D");
    listkv_print(dct);
    printf("Length =%d\n",listkv_len(dct));

    printf("z=%s\n", listkv_get(dct, "z"));
    printf("x=%s\n", listkv_get(dct, "x"));

    printf("\nDump\n");
    for(cur = dct->head; cur != NULL ; cur = cur->next ) {
        printf("%s=%s\n", cur->key, cur->value);
    }

    listkv_del(dct);
}

// rm -f a.out ; gcc pylistkv.c; a.out ; rm -f a.out


