#include <stdio.h>
#include "project/hash_table.h"


int main() {

    char *key = "key";
    char *value = "value";

    hashtable *table = new_hashtable();
    del_hashtable(table);


    printf("Hello, World!\n");
    return 0;
}