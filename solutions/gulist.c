#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct gulist {
  int alloc;
  int length;
  char **items;
};

/* Constructor - lst = list() */
struct gulist * gulist_new() {
    struct gulist *p = malloc(sizeof(*p));
    p->length = 0;
    p->alloc = 2;
    p->items = malloc(p->alloc * sizeof(char *));
    return p;
}

/* Destructor - del(lst) */
void gulist_del(struct gulist* self) {
    int i;
    for(i=0; i< self->length; i++ ) {
        free(self->items[i]);
    }
    free((void *)self->items);
    free((void *)self);
}

/* print(lst) */
void gulist_print(struct gulist* self)
{
    int i;
    int first = 1;
    printf("[");
    for(i=0; i< self->length; i++ ) {
         if ( ! first ) printf(", ");
         printf("'%s'", self->items[i]);
         first = 0;
    }
    printf("]\n");
}

/* len(lst) */
int gulist_len(const struct gulist* self)
{
    return self->length;
}

/* lst.append("Hello world") */
void gulist_append(struct gulist* self, char *str) {
    
    // Extend if necessary
    if ( self->length >= self->alloc ) {
        self->alloc = self->alloc + 10;
        printf("Extending...\n");
        self->items = (char **) realloc(self->items, (self->alloc * sizeof(char *))i );
    }

    char *saved = malloc(strlen(str)+1);
    strcpy(saved, str);
    self->items[self->length] = saved;
    self->length++;
}

/* lst.index("Hello world") - if not found -1 */
int gulist_index(struct gulist* self, char *str)
{
    int i;
    if ( str == NULL ) return -1;
    for(i=0; i< self->length; i++ ) {
        if ( strcmp(str, self->items[i]) == 0 ) return i;
    }
    return -1;
}

int main(void)
{
    struct gulist * lst = gulist_new();
    gulist_append(lst, "Hello world");
    gulist_print(lst);
    gulist_append(lst, "Catch phrase");
    gulist_print(lst);
    gulist_append(lst, "Brian");
    gulist_print(lst);
    printf("Length = %d\n", gulist_len(lst));
    printf("Brian? %d\n", gulist_index(lst, "Brian"));
    printf("Bob? %d\n", gulist_index(lst, "Bob"));
    gulist_del(lst);
}

// rm -f a.out ; gcc gulist.c; a.out ; rm -f a.out

