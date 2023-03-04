#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct dnode {
    char *key;
    char *value;
    struct dnode *next;
};

struct pydict {
  struct dnode *head;
  struct dnode *tail;
  int count;
};

/* Constructor - dct = dict() */
struct pydict * pydict_new() {
    struct pydict *p = malloc(sizeof(*p));
    p->head = NULL;
    p->tail = NULL;
    p->count = 0;
    return p;
}

/* Destructor - del(dct) */
void pydict_del(struct pydict* self) {
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

/* print(lst) */
/* {'z': 'W', 'y': 'B', 'c': 'C', 'a': 'D'} */
void pydict_print(struct pydict* self)
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

int pydict_len(const struct pydict* self)
{
    return self->count;
}

/* find a node - used in get and put */
struct dnode* pydict_find(struct pydict* self, char *key)
{
    struct dnode *cur;
    if ( key == NULL ) return NULL;
    for(cur = self->head; cur != NULL ; cur = cur->next ) {
        if(strcmp(key, cur->key) == 0 ) return cur;
    }
    return NULL;
}

/* x.get(key) - Returns NULL if not found */
char* pydict_get(struct pydict* self, char *key)
{
    struct dnode *retval = pydict_find(self, key);
    if ( retval == NULL ) return NULL;
    return retval->value;
}

/* x[key] = value; Insert or replace the value associated with a key */
void pydict_put(struct pydict* self, char *key, char *value) {

    struct dnode *old, *new;
    char *new_key, *new_value;

    if ( key == NULL || value == NULL ) return;

    // First look up
    old = pydict_find(self, key);
    if ( old != NULL ) {
        free(old->value);
        new_value = malloc(strlen(value)+1);
        strcpy(new_value, value);
        old->value = new_value;
        return;
    }

    // Not found - time to insert
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
    struct pydict * dct = pydict_new();
    pydict_put(dct, "z", "Catch phrase");
    pydict_print(dct);
    pydict_put(dct, "z", "W");
    pydict_print(dct);
    pydict_put(dct, "y", "B");
    pydict_put(dct, "c", "C");
    pydict_put(dct, "a", "D");
    pydict_print(dct);
    printf("Length =%d\n",pydict_len(dct));

    printf("z=%s\n", pydict_get(dct, "z"));
    printf("x=%s\n", pydict_get(dct, "x"));

    printf("\nDump\n");
    for(struct dnode * cur = dct->head; cur != NULL ; cur = cur->next ) {
        printf("%s=%s\n", cur->key, cur->value);
    }

    pydict_del(dct);
}

// rm -f a.out ; gcc pydict.c; a.out ; rm -f a.out


