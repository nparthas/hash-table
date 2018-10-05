#ifndef HASH_TABLE_HASH_TABLE_H
#define HASH_TABLE_HASH_TABLE_H

#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../project/primes.h"

typedef int (*hashfunction_f)(const char *, const int, const int);

typedef struct hashitem_t {
    char *key;
    char *value;
} hashitem;

typedef struct hashtable_t {
    int capacity;
    int base_capacity;
    int count;
    hashfunction_f hash_algorithm;
    hashitem **items;
} hashtable;

hashtable *new_hashtable(hashfunction_f algorithm);

hashtable *new_hashtable_with_capacity(hashfunction_f algorithm,
                                       const int base_capacity);

void del_hashtable(hashtable *table);

void hashtable_insert(hashtable *ht, const char *key, const char *value);

char *hashtable_search(hashtable *ht, const char *key);

void hashtable_remove(hashtable *ht, const char *key);

// hashing algorithms
int naive_hash(const char *str, const int hash_prime, const int num_buckets);

#endif  // HASH_TABLE_HASH_TABLE_H
