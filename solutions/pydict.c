#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct dnode {
    char *key;
    char *value;
    struct dnode *prev;
    struct dnode *next;
};

struct pydict {
  struct dnode *head;
  struct dnode *tail;
  int count;
};

/* Constructor */
struct pydict * pydict_new() {
    struct pydict *p = malloc(sizeof(*p));
    p->head = NULL;
    p->tail = NULL;
    p->count = 0;
    return p;
}

/* Destructor */
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

struct dnode* pydict_find(struct pydict* self, char *key)
{
    struct dnode *cur;
    if ( key == NULL ) return NULL;
    for(cur = self->head; cur != NULL ; cur = cur->next ) {
        if(strcmp(key, cur->key) == 0 ) return cur;
    }
    return NULL;
}

// x.get(key) - Returns NULL if not found
char* pydict_get(struct pydict* self, char *key)
{
    struct dnode *retval = pydict_find(self, key);
    if ( retval == NULL ) return NULL;
    return retval->value;
}

int pydict_len(const struct pydict* self)
{
    return self->count;
}

// x[key] = value;
struct pydict* pydict_put(struct pydict* self, char *key, char *value) {

    struct dnode *old, *new;
    char *new_key, *new_value;

    if ( key == NULL || value == NULL ) return self;

    // First look up
    old = pydict_find(self, key);
    if ( old != NULL ) {
        free(old->value);
        new_value = malloc(strlen(value)+1);
        strcpy(new_value, value);
        old->value = new_value;
        return self;
    }

    // Not found - time to insert
    new = malloc(sizeof(*new));
    new->next = NULL;
    if ( self->head == NULL ) self->head = new;
    if ( self->tail != NULL ) self->tail->next = new;
    new->prev = self->tail;
    self->tail = new;

    new_value = malloc(strlen(value)+1);
    strcpy(new_value, value);
    new->value = new_value;

    new_key = malloc(strlen(key)+1);
    strcpy(new_key, key);
    new->key = new_key;


    self->count++;

    return self; // To allow chaining
}

int pydict_index(struct pydict* self, char *str)
{
    struct dnode *cur;
    int i;
    if ( str == NULL ) return -1;
    for(i=0, cur = self->head; cur != NULL ; i++, cur = cur->next ) {
         if ( strcmp(str, cur->value) == 0 ) return i;
    }
    return -1;
}

void private_pydict_swap(struct pydict* self, struct dnode *cur){

    struct dnode *prev, *next, *rest;

    // Save the cur values
    next = cur->next;        // Will always exist
    prev = cur->prev;        // May be null if at the front
    rest = cur->next->next;  // May be null if next is at the end

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
struct pydict* pydict_ksort(struct pydict* self) {

    struct dnode *cur;
    int i;

    if ( self->head == NULL ) return self;

    for (i=0; i<=self->count; i++) {
        for(cur = self->head; cur != NULL ; cur = cur->next ) {
            if ( cur->next == NULL ) continue;  // Last item in the list
            // In order already
            if ( strcmp(cur->key, cur->next->key) <= 0 ) continue;

            // Lets swap
            // printf("Flipping %s %s\n", cur->value, cur->next->value);
            private_pydict_swap(self, cur);
        }
    }
    return self;
}

// Bubble sort by value - Order N**2
struct pydict* pydict_vsort(struct pydict* self) {

    struct dnode *prev, *cur, *next, *rest;
    int i;

    if ( self->head == NULL ) return self;

    for (i=0; i<=self->count; i++) {
        for(cur = self->head; cur != NULL ; cur = cur->next ) {
            if ( cur->next == NULL ) continue;  // Last item in the list
            // In order already
            if ( strcmp(cur->value, cur->next->value) <= 0 ) continue;

            // printf("Flipping %s %s\n", cur->value, cur->next->value);
            private_pydict_swap(self, cur);
        }
    }
    return self;
}


int main(void)
{
    struct pydict * dct = pydict_new();
    pydict_put(dct, "z", "Catch phrase");
    pydict_print(dct);
    pydict_put(dct, "z", "W value in z");
    pydict_print(dct);
    pydict_put(dct, "y", "B value in y");
    pydict_put(dct, "c", "C value in c");
    pydict_put(dct, "a", "D value in a");
    pydict_print(dct);

    printf("z=%s\n", pydict_get(dct, "z"));
    printf("x=%s\n", pydict_get(dct, "x"));

    pydict_ksort(dct);
    printf("\nSorted by key\n");
    pydict_print(dct);

    pydict_vsort(dct);
    printf("\nSorted by value\n");
    pydict_print(dct);

    printf("\nDump\n");
    for(struct dnode * cur = dct->head; cur != NULL ; cur = cur->next ) {
        printf("%s=%s\n", cur->key, cur->value);
    }

    pydict_del(dct);
}

// rm -f a.out ; gcc pydict.c; a.out ; rm -f a.out


