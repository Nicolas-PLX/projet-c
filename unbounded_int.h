#ifndef UNBOUNDED_INT_H
#define UNBOUNDED_INT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct chiffre {
    struct chiffre  *suivant;
    char c;
    struct chiffre *precedent;
} chiffre;

typedef struct {
    char signe; /* soit ’+’ soit ’-’ */ 
    size_t len; /* longueur de la liste */
    chiffre *premier; /* pointeur vers le premier élément de la liste*/
    chiffre *dernier; /*pointeur vers le dernier élément de la liste*/ 
} unbounded_int;

//Convertir une chaine de caractere en un unbounded_int
unbounded_int string2unbounded_int(const char *e);
//Convertir un long long en un unbounded_int
unbounded_int ll2unbounded_int(long long i);
//Convertir un unbounded_int en une chaine de caracteres
char *unbounded_int2string(unbounded_int i);
/* Comparer deux unbounded_int a et b
   Retourne -1 si a < b
             0 si a = b
             1 si a > b */
int unbounded_int_cmp_unbounded_int(unbounded_int a, unbounded_int b);
/* Comparer un unbounded_int a et un long long b
   Retourne -1 si a < b
             0 si a = b
             1 si a > b */
int unbounded_int_cmp_ll(unbounded_int a, long long b);
//Calculer la somme de deux unbounded_int a et b
unbounded_int unbounded_int_somme(unbounded_int a, unbounded_int b);
//Calculer la difference de deux unbounded_int a et b
unbounded_int unbounded_int_difference( unbounded_int a, unbounded_int b);
//Calculer le produit de deux unbounded_int a et b
unbounded_int unbounded_int_produit( unbounded_int a, unbounded_int b);
//Calculer le quotient de la division entiere d'un unbounded_int a par un unbounded_int b
unbounded_int unbounded_int_quotient( unbounded_int a, unbounded_int b);
//Calculer le reste de la division d'un unbounded_int a par un unbounded_int b
unbounded_int unbounded_int_modulo( unbounded_int a, unbounded_int b);

#endif