#ifndef HASH_TABLE_HASH_TABLE_H
#define HASH_TABLE_HASH_TABLE_H

#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../project/primes.h"

typedef struct hashitem_t {
    char *key;
    char *value;
} hashitem;

typedef struct hashtable_t {
    int capacity;
    int base_capacity;
    int count;
    hashitem **items;
} hashtable;

hashtable *new_hashtable();

hashtable *new_hashtable_with_capacity(const int base_capacity);

void del_hashtable(hashtable *table);

void hashtable_insert(hashtable *ht, const char *key, const char *value);

char *hashtable_search(hashtable *ht, const char *key);

void hashtable_remove(hashtable *ht, const char *key);

#endif  // HASH_TABLE_HASH_TABLE_H
