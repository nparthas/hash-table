#include <stdio.h>
#include <stdlib.h>

#include <limits.h>

#include "project/hash_table.h"
#include "project/primes.h"

#define HASHTABLE_ITEM_COUNT 50
#define HASHTABLE_UPDATE_COUNT 10

int main() {
    const char *key1 = "insert key 1";
    const char *value1 = "insert value 1";

    const char *key2 = "insert key 2";
    const char *value2 = "insert value 2";

    hashtable *table = new_hashtable(naive_hash);

    printf("inserting key 1\n");
    hashtable_insert(table, key1, value1);

    printf("inserting key 2\n");
    hashtable_insert(table, key2, value2);

    printf("searching for keys\n");
    const char *result_1 = hashtable_search(table, key1);
    const char *result_2 = hashtable_search(table, key2);

    printf("got key 1: %s\n", result_1);
    printf("got key 2: %s\n", result_2);

    printf("removing key 1\n");
    hashtable_remove(table, key1);

    printf("removing key 2\n");
    hashtable_remove(table, key2);

    printf("searching for deleted keys\n");
    const char *del_item_1 = hashtable_search(table, key1);
    const char *del_item_2 = hashtable_search(table, key1);

    printf("get deleted key 1: %p\n", del_item_1);
    printf("get deleted key 2: %p\n", del_item_2);

    printf("inserting many values and testing resize up\n");
    char *keys[HASHTABLE_ITEM_COUNT] = {};
    char *values[HASHTABLE_ITEM_COUNT] = {};

    const int old_cap = table->capacity;
    for (int i = 0; i < HASHTABLE_ITEM_COUNT; i++) {
        keys[i] = malloc(sizeof(char) * HASHTABLE_ITEM_COUNT * 2);
        values[i] = malloc(sizeof(char) * HASHTABLE_ITEM_COUNT * 2);

        sprintf(keys[i], "insert key %d", i);
        sprintf(values[i], "insert value %d, adding random: %d", i, rand());

        hashtable_insert(table, keys[i], values[i]);
    }

    const int new_cap = table->capacity;
    if (new_cap <= old_cap) {
        printf("table did not resize up\n");
    } else {
        printf("table successfully resized\n");
    }

    for (int i = 0; i < HASHTABLE_ITEM_COUNT; i++) {
        const char *result = hashtable_search(table, keys[i]);
        if (strcmp(result, values[i])) {
            printf(
                "search does not match insert for key: %s\n\texpected: %s got "
                "%s\n",
                keys[i], values[i], result);
        }
    }

    printf("testing update\n");
    // update some random keys with a new value
    char *update_string = "updated value";
    for (int i = 0; i < HASHTABLE_UPDATE_COUNT; i++) {
        int index = rand() % HASHTABLE_ITEM_COUNT;
        values[index] = update_string;
    }

    for (int i = 0; i < HASHTABLE_ITEM_COUNT; i++) {
        hashtable_insert(table, keys[i], values[i]);
    }

    for (int i = 0; i < HASHTABLE_ITEM_COUNT; i++) {
        const char *result = hashtable_search(table, keys[i]);
        if (strcmp(result, values[i])) {
            printf(
                "search does not match insert for key: %s\n\texpected: %s got "
                "%s\n",
                keys[i], values[i], result);
        }
    }
    printf("table successfully updated\n");

    del_hashtable(table);

    return 0;
}