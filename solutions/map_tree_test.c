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
    struct MapEntry *__next;
    struct MapEntry *__left;
    struct MapEntry *__right;
};

/*
 * A MapIter contains the current item.
 */
struct MapIter {
   struct MapEntry *__current;

   struct MapEntry* (*next)(struct MapIter* self);
   void (*del)(struct MapIter* self);
};

/*
 * This is our Map class
 */
struct Map {
   /* Attributes */
   struct MapEntry *__head;
   struct MapEntry *__root;
   int __count;

   /* Methods */
   void (*put)(struct Map* self, char *key, int value);
   int (*get)(struct Map* self, char *key, int def);
   int (*size)(struct Map* self);
   void (*dump)(struct Map* self);
   struct MapIter* (*iter)(struct Map* self);
   void (*del)(struct Map* self);
};

/**
 * Destructor for the Map Class
 *
 * Loops through and frees all the keys and entries in the map.
 * The values are integers and so there is no need to free them.
 */
void __Map_del(struct Map* self) {
    struct MapEntry *cur, *next;
    cur = self->__head;
    while(cur) {
        free(cur->key);
        /* value is just part of the struct */
        next = cur->__next;
        free(cur);
        cur = next;
    }
    free((void *)self);
}

/**
 * Destructor for the MapIter Class
 */
void __MapIter_del(struct MapIter* self) {
    free((void *)self);
}

/**
 * __Map_dump_tree - traverse and print the tree
 */
void __Map_dump_tree(struct MapEntry *cur, int depth)
{
    if ( cur == NULL ) return;
    for(int i=0;i<depth;i++) printf("| ");
    printf("%s=%d\n", cur->key, cur->value);
    if ( cur->__left != NULL ) {
        __Map_dump_tree(cur->__left, depth+1);
    }
    if ( cur->__right != NULL ) {
        __Map_dump_tree(cur->__right, depth+1);
    }
}

/**
 * map->dump - In effect a toString() except we print the contents of the Map to stdout
 *
 * self - The pointer to the instance of this class.
 */
void __Map_dump(struct Map* self)
{
    struct MapEntry *cur;
    printf("Object Map@%p count=%d\n", self, self->__count);
    for(cur = self->__head; cur != NULL ; cur = cur->__next ) {
         printf("  %s=%d\n", cur->key, cur->value);
    }
    printf("\n");

    // Recursively print the tree view
    __Map_dump_tree(self->__root, 0);
    printf("\n");
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

    struct MapEntry *cur, *left, *right;
    int cmp;
    struct MapEntry *old, *new;
    char *new_key;

    if ( key == NULL ) return;

    // printf("searching for %s\n", key);

    cur = self->__root;
    left = NULL;
    right = NULL;
    while(cur != NULL ) {
        cmp = strcmp(key, cur->key);
        if(cmp == 0 ) {
            // printf("replacing %s=%d\n",key, value);
            cur->value = value;
            return;
        }
        if( cmp < 0 ) {
            right = cur;
            cur = cur->__left;
        } else {
            left = cur;
            cur = cur->__right;
        }
    }

    /* Not found - time to insert into the linked list after old */
    new = malloc(sizeof(*new));
    new->__next = NULL;
    new->__left = NULL;
    new->__right = NULL;
    new_key = malloc(strlen(key)+1);
    strcpy(new_key, key);
    new->key = new_key;
    new->value = value;
    self->__count++;

    // Empty list - add first entry
    if ( self->__head == NULL ) {
        self->__head = new;
        self->__root = new;
        // printf("First item inserted\n");
        return;
    }

    printf("%s < %s > %s\n", (left ? left->key : "0"),
            key, (right ? right->key : "0") );

    // Insert into the sorted linked list
    if ( left == NULL ) {
        if ( self->__head != right ) {
            printf("FAIL self->__head != right\n");
        }
        new->__next = self->__head;
        self->__head = new;
    } else {
        if ( left->__next != right ) {
            printf("FAIL left->__next != right\n");
        }
        new->__next = right;
        left->__next = new;
    }

    // Insert into the tree
    if ( right != NULL && right->__left == NULL ) {
        right->__left = new;
    } else if ( left != NULL && left->__right == NULL ) {
        left->__right = new;
    } else {
        printf("FAIL Neither right->__left nor left->__right are available\n");
    }

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
    int cmp;
    struct MapEntry *cur;

    if ( self == NULL || key == NULL || self->__root == NULL ) return def;

    cur = self->__root;
    while(cur != NULL ) {
        cmp = strcmp(key, cur->key);
        if(cmp == 0 ) return cur->value;
        else if(cmp < 0 ) cur = cur->__left;
        else cur = cur->__right;

    }
    return def;
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
    struct MapEntry* retval;
    if ( self->__current == NULL) return NULL;
    retval = self->__current;
    self->__current = self->__current->__next;
    return retval;
}

/**
 * map->iter - Create an iterator from the head of the Map 
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
struct MapIter* __Map_iter(struct Map* self)
{
    struct MapIter *iter = malloc(sizeof(*iter));
    iter->__current = self->__head;
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

    p->__head = NULL;
    p->__root = NULL;
    p->__count = 0;

    p->put = &__Map_put;
    p->get = &__Map_get;
    p->size = &__Map_size;
    p->dump = &__Map_dump;
    p->iter = &__Map_iter;
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

    map->put(map, "r", 8);
    map->put(map, "y", 2);
    map->put(map, "a", 4);
    map->put(map, "b", 3);
    map->put(map, "r", 1);
    map->dump(map);

    printf("r=%d\n", map->get(map, "r", 42));
    printf("x=%d\n", map->get(map, "x", 42));

    printf("\nIterate\n");
    iter = map->iter(map);
    while(1) {
        cur = iter->next(iter);
        if ( cur == NULL ) break;
        printf(" %s=%d\n", cur->key, cur->value);
    }
    iter->del(iter);

    map->del(map);
}

// rm -f a.out ; gcc map_tree.c; a.out ; rm -f a.out

