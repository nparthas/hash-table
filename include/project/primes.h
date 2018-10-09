#ifndef HASH_TABLE_PRIMES_H
#define HASH_TABLE_PRIMES_H

#include <math.h>
#include <stdbool.h>
#include <stdint.h>

bool is_prime(int num);
int64_t next_prime(int num);
int64_t find_fnv1_prime(uint32_t bitsize);

#endif  // HASH_TABLE_PRIMES_H
