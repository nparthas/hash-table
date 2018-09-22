#include "project/hash_table.h"

static hashitem DELETED_ITEM = {NULL, NULL};

static long power_l(int base, int exponent) {
    int result = 1;
    while (exponent) {
        if (exponent & 1) {
            result *= base;
        }
        exponent >>= 1;
        base *= base;
    }
    return result;
}

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

hashtable *new_hashtable() {
    hashtable *table = malloc(sizeof(hashtable));
    table->size = 53;
    table->count = 0;
    table->items = calloc((size_t)table->size, sizeof(hashitem *));
    return table;
}

void del_hashtable(hashtable *table) {
    for (int i = 0; i < table->size; i++) {
        hashitem *item = table->items[i];
        if (item) {
            del_hashitem(item);
        }
    }
}

static int hash(const char *str, const int hash_prime, const int num_buckets) {
    long hash = 0;
    const int str_len = strlen(str);
    for (int i = 0; i < str_len; i++) {
        hash += power_l(hash_prime, str_len - (i + 1) * str[i]);
        hash %= num_buckets;
    }
    return (int)hash;
}

// just so that it can compile
#define HT_PRIME_1 1
#define HT_PRIME_2 2

static int get_hash(const char *str, const int num_buckets, const int attempt) {
    const int hash_a = hash(str, HT_PRIME_1, num_buckets);
    const int hash_b = hash(str, HT_PRIME_2, num_buckets);
    return (hash_a + (attempt * (hash_b + 1))) % num_buckets;
}

// this is messy, refactor
void hashtable_insert(hashtable *ht, const char *key, const char *value) {
    hashitem *item = new_hashitem(key, value);
    int index = get_hash(item->key, ht->size, 0);
    hashitem *cur_item = ht->items[index];
    int i = 1;
    while (cur_item != NULL) {
        if (cur_item != &DELETED_ITEM) {
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
        if (item != &DELETED_ITEM) {
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
    int index = get_hash(key, ht->size, 0);
    hashitem *item = ht->items[index];
    int i = 1;
    while (item != NULL) {
        if (item != &DELETED_ITEM) {
            if (strcmp(item->key, key) == 0) {
                del_hashitem(item);
                ht->items[index] = &DELETED_ITEM;
            }
        }
        index = get_hash(key, ht->size, i);
        item = ht->items[index];
        i++;
    }
    ht->count--;
}
