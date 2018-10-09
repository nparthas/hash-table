#include "project/primes.h"

// temp

#include <stdio.h>

uint64_t power(uint32_t base, uint32_t exponent) {
    uint64_t result = 1;
    while (exponent) {
        if (exponent & 1) {
            result *= base;
        }
        exponent >>= 1;
        base *= base;
    }
    return result;
}

bool is_prime(int num) {
    // 1 not is prime
    if (num == 1) return false;
    // 2 and 3 are prime
    if (num < 4) return true;
    // all even numbers are not prime
    if (num % 2 == 0) return false;

    // 5, 7 are prime
    if (num < 9) return true;
    // all multiples of 3 are not prime
    if (num % 3 == 0) return false;

    // check divisibility for all numbers less than sqrt
    int64_t r = (int64_t)sqrtl(num);
    int64_t f = 5;
    while (f <= r) {
        if (num % f == 0) return false;
        if (num % (f + 2) == 0) return false;
        f += 6;
    }
    return true;
}

int64_t next_prime(int num) {
    // if the number is already prime, return it
    if (is_prime(num)) return num;

    // special case 2, it's even and a prime
    if (num == 1) return 2;

    // if the new number is even, make it odd
    int64_t num_l = (int64_t)num;
    if (!(num_l % 2)) num_l++;

    while (!is_prime(num_l)) {
        num_l += 2;
    }
    return num_l;
}

static uint32_t count_one_bits(uint32_t num) {
    uint32_t count = 0;
    while (num != 0) {
        if (num & 1) {
            count++;
        }
        num = num >> 1;
    }
    return count;
}

static bool fnv_inequality(uint64_t num) {
    uint64_t left = num % ((1ull << 40) - (1ull << 24) - 1);
    uint64_t right = (1ull << 24) | (1ull << 8) | (1ull << 7);
    return left > right;
}

static uint32_t generate_b(uint32_t seed) {
    uint32_t bit_count = count_one_bits(seed);
    while (bit_count != 4 && bit_count != 5) {
        bit_count = count_one_bits(++seed);
    }
    return seed;
}

// param bitsize: takes size of prime in bits, eg. 32, 64
// must be a power of 2
int64_t find_fnv1_prime(uint32_t bitsize) {
    // computer s from bitsize, it's always a power of 2 so find the first set
    // bit
    // 4 < s < 11
    uint32_t s = 0;
    while (!(bitsize & 1)) {
        bitsize >>= 1;
        s++;
    }

    uint32_t t = (5 + (1 << s)) / 12;
    uint32_t b = 15;
    uint64_t candidate = (1 << (t * 8)) + (1 << 8) + b;

    while (!is_prime(candidate) || !fnv_inequality(candidate)) {
        b = generate_b(++b);
        candidate = (1 << (t * 8)) + (1 << 8) + b;
    }
    return candidate;
}