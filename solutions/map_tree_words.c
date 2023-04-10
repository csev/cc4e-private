#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
    char word[100];  // Yes, this is dangerous
    int count, maxvalue;
    char *maxkey;

    FILE *fp = fopen("romeo.txt", "r");
    
    // Loop over each word in the file
    while (fscanf(fp, "%s", word) != EOF) {
        for (int i = 0; word[i] != '\0'; i++) {
            word[i] = tolower(word[i]);
        }
        count = map->get(map, word, 0);
        map->put(map, word, count+1);
    }
    fclose(fp);

    map->dump(map);

    printf("\nFind max...\n");
    maxkey = NULL;
    maxvalue = -1;
    iter = map->iter(map);
    while(1) {
        cur = iter->next(iter);
        if ( cur == NULL ) break;
        if ( maxkey == NULL || cur->value > maxvalue ) {
            maxkey = cur->key;
            maxvalue = cur->value;
        }
    }
    iter->del(iter);
    printf("\nMAX %s=%d\n", maxkey, maxvalue);

    map->del(map);
}

// rm -f a.out ; gcc map_tree_words.c; a.out ; rm -f a.out

