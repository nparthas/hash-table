#ifndef HASH_TABLE_HASH_TABLE_H
#define HASH_TABLE_HASH_TABLE_H

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../project/primes.h"

typedef uint32_t (*hashfunction_f)(const char *, const int, const int);

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
uint32_t hash_naive(const char *str, const int hash_prime,
                    const int num_buckets);

uint32_t hash_fnv1a(const char *str, const int hash_prime,
                    const int num_buckets);

uint32_t hash_murmur3(const char *str, const int hash_prime,
                      const int num_buckets);

#endif  // HASH_TABLE_HASH_TABLE_H
