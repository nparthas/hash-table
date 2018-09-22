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
    int r = sqrt(num);
    int f = 5;
    while (f <= r) {
        if (num % f == 0) return false;
        if (num % (f + 2) == 0) return false;
        f += 6;
    }
    return true;
}

int next_prime(int num) {
    if (num % 2) num++;
    while (!is_prime(num)) {
        num += 2;
    }
    return num;
}