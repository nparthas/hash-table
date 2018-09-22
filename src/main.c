#include <stdio.h>
#include "project/hash_table.h"
#include "project/primes.h"


#include<math.h>

int main() {


    bool b = is_prime(56);
    int a = sqrt(16);

    char *key = "key";
    char *value = "value";

    hashtable *table = new_hashtable();
    del_hashtable(table);


    printf("Including math.h, %d\n", a);
    printf("%d\n", b);
    return 0;
}