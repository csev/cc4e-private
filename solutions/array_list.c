#include <stdio.h>
#include <stdlib.h>

struct lnode {
    int value;
    struct lnode *next;
};

struct list {
  struct lnode *head;
  struct lnode *tail;
};

void list_add(lst, value)
    struct list *lst;
    int value;
{
      struct lnode *new = (struct lnode *) malloc(sizeof(struct lnode));
      new->value = value;
      if ( lst->tail != NULL ) lst->tail->next = new;
      new->next = NULL;
      lst->tail = new;
      if ( lst->head == NULL ) lst->head = new;
}


struct lnode * list_find(lst, value)
    struct list *lst;
    int value;
{
    struct lnode *cur;
    for(cur=lst->head; cur != NULL; cur=cur->next) {
        if ( value == cur->value ) return cur;
    }
    return NULL;
}

int list_remove(lst, value)
    struct list *lst;
    int value;
{
    struct lnode *cur;
    struct lnode *prev = NULL;
    for(cur=lst->head; cur != NULL; cur=cur->next) {
        if ( value == cur->value ) {
            if ( prev == NULL ) {
                lst->head = cur->next;
            } else {
                prev->next = cur->next;
                if ( cur->next == NULL) {
                    lst->tail = prev;
                }
            }
            free(cur);
            return 1;
        }
        prev = cur;
    }
    return 0;
}

void list_dump(lst)
    struct list *lst;
{
    struct lnode *cur;
    printf("\nDump:\n");
    for(cur=lst->head; cur != NULL; cur=cur->next) {
        printf("  %d\n", cur->value);
    }
}

int main()
{
    struct list mylist;
    void list_add();

    mylist.head = NULL;
    mylist.tail = NULL;

    list_add(&mylist, 10);
    list_add(&mylist, 20);
    list_add(&mylist, 30);


    list_dump(&mylist);
    list_remove(&mylist, 42);

    list_dump(&mylist);

    list_remove(&mylist, 20);
    list_dump(&mylist);

    list_add(&mylist, 40);
    list_dump(&mylist);

    list_remove(&mylist, 10);
    list_dump(&mylist);

    list_remove(&mylist, 40);
    list_dump(&mylist);

    list_remove(&mylist, 30);
    list_dump(&mylist);

}
