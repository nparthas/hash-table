#include <stdlib.h>

#include <stdio.h>
#include "project/hash_table.h"
#include "project/primes.h"

#define HASHTABLE_ITEM_COUNT 50

int main() {
    const char *key1 = "insert key 1";
    const char *value = "insert value 1";

    const char *key2 = "insert key 2";
    const char *value2 = "insert value 2";

    hashtable *table = new_hashtable();

    printf("inserting key 1\n");
    hashtable_insert(table, key1, value2);

    printf("inserting key 2\n");
    hashtable_insert(table, key2, value2);

    const char *result_1 = hashtable_search(table, key1);
    const char *result_2 = hashtable_search(table, key1);

    printf("got key 1: %s\n", result_1);
    printf("got key 2: %s\n", result_2);

    printf("removing key 1\n");
    hashtable_remove(table, key1);

    printf("removing key 2\n");
    hashtable_remove(table, key2);

    const char *del_item_1 = hashtable_search(table, key1);
    const char *del_item_2 = hashtable_search(table, key1);

    printf("get deleted key 1: %p\n", del_item_1);
    printf("get deleted key 2: %p\n", del_item_2);

    char *keys[HASHTABLE_ITEM_COUNT] = {};
    char *values[HASHTABLE_ITEM_COUNT] = {};

    for (int i = 0; i < HASHTABLE_ITEM_COUNT; i++) {
        keys[i] = malloc(sizeof(char) * HASHTABLE_ITEM_COUNT * 2);
        values[i] = malloc(sizeof(char) * HASHTABLE_ITEM_COUNT * 2);

        sprintf(keys[i], "insert key %d", i);
        sprintf(values[i], "insert value %d, adding random: %d", i, rand());

        hashtable_insert(table, keys[i], values[i]);
    }

    for (int i = 0; i < HASHTABLE_ITEM_COUNT; i++) {
        const char *result = hashtable_search(table, keys[i]);
        printf("key, value: %s %s\n", keys[i], values[i]);
    }

    del_hashtable(table);

    return 0;
}