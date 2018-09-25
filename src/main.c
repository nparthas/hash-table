#include <stdio.h>
#include "project/hash_table.h"
#include "project/primes.h"

int main() {
    const char *key = "key";
    const char *value = "value";

    hashtable *table = new_hashtable();
    del_hashtable(table);

    bool p = is_prime(16);
    int c = next_prime(17);

    printf("16 is prime: %d, next prime after 17: %d\n", p, c);

    return 0;
}