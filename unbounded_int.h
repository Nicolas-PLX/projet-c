#ifndef UNBOUNDED_INT_H
#define UNBOUNDED_INT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include "chiffre.h"

typedef struct chiffre {
    struct chiffre  *suivant;
    char c;
    struct chiffre *precedent;
} chiffre;

chiffre *chiffre_creer(chiffre *suivant, char c, chiffre *precedent);

typedef struct {
    char signe; /* soit ’+’ soit ’-’ */ 
    size_t len; /* longueur de la liste */
    chiffre *premier; /* pointeur vers le premier élément de la liste*/
    chiffre *dernier; /*pointeur vers le dernier élément de la liste*/ 
} unbounded_int;

unbounded_int *unbounded_int_creer(void);
unbounded_int *unbounded_int_ajouter_chiffre(unbounded_int *i, chiffre *c);
unbounded_int *unbounded_int_ajouter_char(unbounded_int *i, char c);

unbounded_int string2unbounded_int(const char *e);
unbounded_int ll2unbounded_int(long long i);
char *unbounded_int2string(unbounded_int i);
int unbounded_int_cmp_unbounded_int(unbounded_int a, unbounded_int b);
int unbounded_int_cmp_ll(unbounded_int a, long long b);
unbounded_int unbounded_int_somme(unbounded_int a, unbounded_int b);
unbounded_int unbounded_int_difference( unbounded_int a, unbounded_int b);
unbounded_int unbounded_int_produit( unbounded_int a, unbounded_int b);

#endif