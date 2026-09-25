#ifndef RSA_CRYPTO_H
#define RSA_CRYPTO_H

unsigned long long mod_exp(unsigned long long base, unsigned long long exp, unsigned long long mod);
int est_premier(int n);
int generer_petit_premier();
unsigned long long pgcd(unsigned long long a, unsigned long long b);
unsigned long long calculer_e(unsigned long long phi);
unsigned long long calculer_d(unsigned long long e, unsigned long long phi);
unsigned long long chiffrer(unsigned long long message, unsigned long long e, unsigned long long n);
unsigned long long dechiffrer(unsigned long long message_chiffre, unsigned long long d, unsigned long long n);

#endif