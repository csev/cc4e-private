#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "map_tree.c"

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

// rm -f a.out ; gcc map_tree_test.c; a.out ; rm -f a.out

