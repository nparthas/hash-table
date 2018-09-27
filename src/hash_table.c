#include "project/hash_table.h"

#include <stdio.h>

const static int HT_PRIME_1 = 211;
const static int HT_PRIME_2 = 257;
const static int HT_BASE_SIZE = 53;

static hashitem HT_DELETED_ITEM = {NULL, NULL};

static hashitem *new_hashitem(const char *key, const char *value) {
    hashitem *item = malloc(sizeof(hashitem));
    item->key = strdup(key);
    item->value = strdup(value);
    return item;
}

static void del_hashitem(hashitem *item) {
    free(item->key);
    free(item->value);
    free(item);
}

static void hashtable_resize(hashtable *ht, const int base_size) {
    // we don't want to reduce the size down from the base
    if (base_size <= HT_BASE_SIZE) return;

    // create the new table
    hashtable *temp_ht = new_hashtable_with_size(base_size);

    // insert all entries into the new table
    for (int i = 0; i < ht->size; i++) {
        hashitem *item = ht->items[i];
        if (item != NULL && item != &HT_DELETED_ITEM) {
            hashtable_insert(temp_ht, item->key, item->value);
        }
    }
    // swap references
    ht->base_size = temp_ht->base_size;
    ht->count = temp_ht->count;
    ht->size = temp_ht->size;

    printf("old %p new %p\n", ht->items, temp_ht->items);
    hashitem **tmp = ht->items;
    ht->items = temp_ht->items;
    temp_ht->items = tmp;

    // delete the temp table
    del_hashtable(temp_ht);
}

static void hashtable_resize_up(hashtable *table) {
    printf("sizing up\n");
    const int new_size = table->base_size * 2;
    hashtable_resize(table, new_size);
}

static void hashtable_resize_down(hashtable *table) {
    const int new_size = table->base_size / 2;
    hashtable_resize(table, new_size);
}

hashtable *new_hashtable_with_size(const int base_size) {
    hashtable *table = malloc(sizeof(hashtable));

    table->base_size = base_size;
    table->size = next_prime(base_size);

    table->count = 0;
    table->items = calloc((size_t)table->size, sizeof(hashitem *));
    return table;
}

hashtable *new_hashtable() { return new_hashtable_with_size(HT_BASE_SIZE); }

void del_hashtable(hashtable *ht) {
    for (int i = 0; i < ht->size; i++) {
        hashitem *item = ht->items[i];
        // we don't want to remove the deleted item marker
        if (item && item != &HT_DELETED_ITEM) {
            del_hashitem(item);
        }
    }
    free(ht);
}

static int hash(const char *str, const int hash_prime, const int num_buckets) {
    long hash = 0;
    const int str_len = strlen(str);
    for (int i = 0; i < str_len; i++) {
        hash += (long)powl(hash_prime, str_len - (i + 1)) * str[i];
        hash %= num_buckets;
    }
    return (int)hash;
}

static int get_hash(const char *str, const int num_buckets, const int attempt) {
    const int hash_a = hash(str, HT_PRIME_1, num_buckets);
    const int hash_b = hash(str, HT_PRIME_2, num_buckets);
    return (hash_a + (attempt * (hash_b + 1))) % num_buckets;
}

// this is messy, refactor
void hashtable_insert(hashtable *ht, const char *key, const char *value) {
    // check if the table needs to be resized
    const int size_ratio = ((ht->count + 1) * 100) / ht->size;
    printf("%d\n", size_ratio);
    if (size_ratio > 70) hashtable_resize_up(ht);

    hashitem *item = new_hashitem(key, value);
    int index = get_hash(item->key, ht->size, 0);
    hashitem *cur_item = ht->items[index];
    int i = 1;
    while (cur_item != NULL) {
        if (cur_item != &HT_DELETED_ITEM) {
            if (strcmp(cur_item->key, key) == 0) {
                del_hashitem(cur_item);
                ht->items[index] = item;
                return;
            }
        }
        index = get_hash(item->key, ht->size, i);
        cur_item = ht->items[index];
        i++;
    }
    ht->items[index] = item;
    ht->count++;
}

char *hashtable_search(hashtable *ht, const char *key) {
    int index = get_hash(key, ht->size, 0);
    hashitem *item = ht->items[index];
    int i = 1;
    while (item != NULL) {
        if (item != &HT_DELETED_ITEM) {
            if (strcmp(item->key, key) == 0) {
                return item->value;
            }
        }
        index = get_hash(key, ht->size, i);
        item = ht->items[index];
        i++;
    }
    return NULL;
}

void hashtable_remove(hashtable *ht, const char *key) {
    // check if the table needs to be resized
    const int size_ratio = ((ht->count + 1) * 100) / ht->size;
    if (size_ratio < 10) hashtable_resize_down(ht);

    int index = get_hash(key, ht->size, 0);
    hashitem *item = ht->items[index];
    int i = 1;
    while (item != NULL) {
        if (item != &HT_DELETED_ITEM) {
            if (strcmp(item->key, key) == 0) {
                del_hashitem(item);
                ht->items[index] = &HT_DELETED_ITEM;
            }
        }
        index = get_hash(key, ht->size, i);
        item = ht->items[index];
        i++;
    }
    ht->count--;
}
