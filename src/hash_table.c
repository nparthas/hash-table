#include "project/hash_table.h"

#include <stdio.h>

// FNV primes do a better job with hashing, will use those for the naive hash as
// well
// static const int HT_PRIME_1 = 1073741827;
// static const int HT_PRIME_2 = 1073741831;
static const int HT_BASE_CAPACITY = 53;

static const uint32_t FNV_OFFSET = 2166136261;
static const uint32_t FNV_PRIME_1 = 16777619;
static const uint32_t FNV_PRIME_2 = 16777639;

static hashitem HT_DELETED_ITEM = {NULL, NULL};

static char *strdup(const char *str) {
    size_t len = strlen(str) + 1;
    void *new = malloc(len);
    if (new == NULL) return NULL;
    return (char *)memcpy(new, str, len);
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

static void hashtable_resize(hashtable *ht, const int base_capacity) {
    // we don't want to reduce the size down from the base
    if (base_capacity <= HT_BASE_CAPACITY) return;

    // create the new table
    hashtable *temp_ht =
        new_hashtable_with_capacity(ht->hash_algorithm, base_capacity);

    // insert all entries into the new table
    for (int i = 0; i < ht->capacity; i++) {
        hashitem *item = ht->items[i];
        if (item != NULL && item != &HT_DELETED_ITEM) {
            hashtable_insert(temp_ht, item->key, item->value);
        }
    }

    ht->count = temp_ht->count;
    // swap the count for the delete call
    ht->capacity ^= temp_ht->capacity;
    temp_ht->capacity ^= ht->capacity;
    ht->capacity ^= temp_ht->capacity;

    hashitem **tmp = ht->items;
    ht->items = temp_ht->items;
    temp_ht->items = tmp;

    // delete the temp table
    del_hashtable(temp_ht);
}

static void hashtable_resize_up(hashtable *table) {
    const int new_capacity = table->base_capacity * 2;
    hashtable_resize(table, new_capacity);
}

static void hashtable_resize_down(hashtable *table) {
    const int new_capacity = table->base_capacity / 2;
    hashtable_resize(table, new_capacity);
}

hashtable *new_hashtable_with_capacity(hashfunction_f algorithm,
                                       const int base_capacity) {
    hashtable *table = malloc(sizeof(hashtable));
    table->hash_algorithm = algorithm;

    table->base_capacity = base_capacity;
    table->capacity = next_prime(base_capacity);

    table->count = 0;
    table->items = calloc((size_t)table->capacity, sizeof(hashitem *));
    return table;
}

hashtable *new_hashtable(hashfunction_f algorithm) {
    return new_hashtable_with_capacity(algorithm, HT_BASE_CAPACITY);
}

void del_hashtable(hashtable *ht) {
    for (int i = 0; i < ht->capacity; i++) {
        hashitem *item = ht->items[i];
        // we don't want to remove the deleted item marker
        if (item && item != &HT_DELETED_ITEM) {
            del_hashitem(item);
        }
    }
    free(ht);
}

uint32_t hash_naive(const char *str, const int hash_prime,
                    const int num_buckets) {
    uint64_t hash = 0;
    const size_t str_len = strlen(str);
    for (uint32_t i = 0; i < str_len; i++) {
        hash += (uint64_t)powl(hash_prime, str_len - (i + 1)) * str[i];
        hash %= num_buckets;
    }
    return (uint32_t)hash;
}

uint32_t hash_fnv1a(const char *str, const int hash_prime,
                    const int num_buckets) {
    // we don't need the number of buckets, but we have to match the signature
    (void)num_buckets;

    uint64_t hash = FNV_OFFSET;
    const size_t str_len = strlen(str);
    for (uint32_t i = 0; i < str_len; i++) {
        hash ^= (str[i] & 0xff);
        hash *= hash_prime;
    }
    return (uint32_t)hash;
}

uint32_t hash_murmur3(const char *str, const int hash_prime,
                      const int num_buckets) {
    // we don't need the number of buckets, but we have to match the signature
    (void)num_buckets;

    uint32_t len = strlen(str);
    uint32_t c1 = 0xcc9e2d51;
    uint32_t c2 = 0x1b873593;
    uint32_t r1 = 15;
    uint32_t r2 = 13;
    uint32_t m = 5;
    uint32_t n = 0xe6546b64;
    uint32_t h = 0;
    uint32_t k = 0;
    uint8_t *d = (uint8_t *)str;  // 32 bit extract from `key'
    const uint32_t *chunks = NULL;
    const uint8_t *tail = NULL;  // tail - last 8 bytes
    int i = 0;
    int l = len / 4;  // chunk length

    h = hash_prime;

    chunks = (const uint32_t *)(d + l * 4);  // body
    tail = (const uint8_t *)(d + l * 4);     // last 8 byte chunk of `key'

    // for each 4 byte chunk of `key'
    for (i = -l; i != 0; ++i) {
        // next 4 byte chunk of `key'
        k = chunks[i];

        // encode next 4 byte chunk of `key'
        k *= c1;
        k = (k << r1) | (k >> (32 - r1));
        k *= c2;

        // append to hash
        h ^= k;
        h = (h << r2) | (h >> (32 - r2));
        h = h * m + n;
    }

    k = 0;

    // remainder
    switch (len & 3) {  // `len % 4'
        case 3:
            k ^= (tail[2] << 16);
        //-fallthrough
        case 2:
            k ^= (tail[1] << 8);
        //-fallthrough
        case 1:
            k ^= tail[0];
            k *= c1;
            k = (k << r1) | (k >> (32 - r1));
            k *= c2;
            h ^= k;
    }

    h ^= len;

    h ^= (h >> 16);
    h *= 0x85ebca6b;
    h ^= (h >> 13);
    h *= 0xc2b2ae35;
    h ^= (h >> 16);

    return h;
}

static uint32_t get_hash(hashtable *ht, const char *str, const int num_buckets,
                         const int attempt) {
    const uint32_t hash_a = ht->hash_algorithm(str, FNV_PRIME_1, num_buckets);
    const uint32_t hash_b = ht->hash_algorithm(str, FNV_PRIME_2, num_buckets);
    return (hash_a + (attempt * (hash_b + 1))) % num_buckets;
}

// this is messy, refactor
void hashtable_insert(hashtable *ht, const char *key, const char *value) {
    // check if the table needs to be resized
    const int size_ratio = ((ht->count + 1) * 100) / ht->capacity;
    if (size_ratio > 70) hashtable_resize_up(ht);

    hashitem *item = new_hashitem(key, value);
    uint32_t index = get_hash(ht, item->key, ht->capacity, 0);
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
        index = get_hash(ht, item->key, ht->capacity, i);
        cur_item = ht->items[index];
        i++;
    }
    ht->items[index] = item;
    ht->count++;
}

char *hashtable_search(hashtable *ht, const char *key) {
    uint32_t index = get_hash(ht, key, ht->capacity, 0);
    hashitem *item = ht->items[index];
    int i = 1;
    while (item != NULL) {
        if (item != &HT_DELETED_ITEM) {
            if (strcmp(item->key, key) == 0) {
                return item->value;
            }
        }
        index = get_hash(ht, key, ht->capacity, i);
        item = ht->items[index];
        i++;
    }
    return NULL;
}

void hashtable_remove(hashtable *ht, const char *key) {
    // check if the table needs to be resized
    const int size_ratio = ((ht->count + 1) * 100) / ht->capacity;
    if (size_ratio < 10) hashtable_resize_down(ht);

    uint32_t index = get_hash(ht, key, ht->capacity, 0);
    hashitem *item = ht->items[index];
    int i = 1;
    while (item != NULL) {
        if (item != &HT_DELETED_ITEM) {
            if (strcmp(item->key, key) == 0) {
                del_hashitem(item);
                ht->items[index] = &HT_DELETED_ITEM;
            }
        }
        index = get_hash(ht, key, ht->capacity, i);
        item = ht->items[index];
        i++;
    }
    ht->count--;
}
