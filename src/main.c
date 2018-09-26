#include <stdio.h>
#include "project/hash_table.h"
#include "project/primes.h"

int main() {
    const char *key = "insert key";
    const char *value = "insert value";

    hashtable *table = new_hashtable();

    hashtable_insert(table, key, value);

    const char *result = hashtable_search(table, key);

    hashtable_remove(table, key);

    const char *del_item = hashtable_search(table, key);

    del_hashtable(table);

    return 0;
}