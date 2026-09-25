#include <stdio.h>
#include <stdlib.h>
#include "rsa_crypto.h"

unsigned long long mod_exp(unsigned long long base, unsigned long long exp, unsigned long long mod) {
    unsigned long long resultat = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1) resultat = (resultat * base) % mod;
        exp = exp / 2;
        base = (base * base) % mod;
    }
    return resultat;
}

unsigned long long chiffrer(unsigned long long message, unsigned long long e, unsigned long long n) {
    return mod_exp(message, e, n);
}

unsigned long long dechiffrer(unsigned long long message_chiffre, unsigned long long d, unsigned long long n) {
    return mod_exp(message_chiffre, d, n);
}

unsigned long long pgcd(unsigned long long a, unsigned long long b) {
    unsigned long long temp;
    while (b != 0) {
        temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int est_premier(int n) {
    if (n <= 1) return 0;
    if (n <= 3) return 1;
    if (n % 2 == 0 || n % 3 == 0) return 0;
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return 0;
    }
    return 1;
}

int generer_petit_premier() {
    int nb;
    do {
        nb = (rand() % 90) + 10; 
    } while (!est_premier(nb));
    return nb;
}

unsigned long long calculer_e(unsigned long long phi) {
    unsigned long long e = 3;
    while (e < phi) {
        if (pgcd(e, phi) == 1) return e;
        e++;
    }
    return 0;
}

unsigned long long calculer_d(unsigned long long e, unsigned long long phi) {
    long long t = 0, newt = 1;
    long long r = phi, newr = e;
    while (newr != 0) {
        long long quotient = r / newr;
        long long temp_t = t;
        t = newt;
        newt = temp_t - quotient * newt;
        long long temp_r = r;
        r = newr;
        newr = temp_r - quotient * newr;
    }
    if (r > 1) return 0;
    if (t < 0) t = t + phi;
    return (unsigned long long)t;
}