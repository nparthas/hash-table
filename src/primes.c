#include "project/primes.h"

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