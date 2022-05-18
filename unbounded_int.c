#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "unbounded_int.h"
#include "chiffre.h"

#define SIZE_INT_IN_STRING 3
#define CONST_SOUSTRACTION 10
#define UNKNOW_SIZE 256

static int verification(const unbounded_int *i) {
    if(i -> signe == '*') {
        return 0;
    } else {
        return 1;
    }
}

static unbounded_int *unbounded_int_creer_zero() {
    unbounded_int *tmp = malloc(sizeof(unbounded_int));
    if(tmp == NULL) {
        tmp -> signe = '*';
    }
    tmp -> len = 1;
    tmp -> premier = chiffre_creer_char('0');
    tmp -> dernier = tmp -> premier;
    return tmp;
}

unbounded_int *unbounded_int_creer() {
    unbounded_int *tmp = malloc(sizeof(unbounded_int));
    if(tmp == NULL) {
        tmp -> signe = '*';
    }
    tmp -> len = 0;
    tmp -> premier = NULL;
    tmp -> dernier = NULL;
    return tmp;
}

unbounded_int *unbounded_int_ajouter_chiffre_fin(unbounded_int *i, chiffre *c) {
    if(verification(i)) {
        if(i -> premier == NULL) {
            i -> premier = c;
            i -> dernier = c;
        } else {
            i -> dernier -> suivant = c;
            c -> precedent = i -> dernier;
            i -> dernier = c;
        }
        i -> len += 1;
    }
    return i;
}

unbounded_int *unbounded_int_ajouter_char_fin(unbounded_int *i, char c) {
    chiffre *tmp = chiffre_creer_char(c);
    if(tmp == NULL) {
        if(!verification(i)) {
            i -> signe = '*';
        }
        return i;
    }
    if(verification(i)) {
        if(i -> premier == NULL) {
            i -> premier = tmp;
            i -> dernier = tmp;
        } else {
            i -> dernier -> suivant = tmp;
            tmp -> precedent = i -> dernier;
            i -> dernier = tmp;
        }
        i -> len += 1;
    }
    return i;
}

unbounded_int *unbounded_int_ajouter_chiffre_debut(unbounded_int *i, chiffre *c) {
    if(verification(i)) {
        if(i -> premier == NULL) {
            i -> premier = c;
            i -> dernier = c;
        } else {
            i -> premier -> precedent = c;
            c -> suivant = i -> premier;
            i -> premier = c;
        }
        i -> len += 1;
    }
    return i;
}

unbounded_int *unbounded_int_ajouter_char_debut(unbounded_int *i, char c) {
    chiffre *tmp = chiffre_creer_char(c);
    if(tmp == NULL) {
        if(!verification(i)) {
            i -> signe = '*';
        }
        return i;
    }
    if(verification(i)) {
        if(i -> premier == NULL) {
            i -> premier = tmp;
            i -> dernier = tmp;
        } else {
            i -> premier -> precedent = tmp;
            tmp -> suivant = i -> premier;
            i -> premier = tmp;
        }
        i -> len += 1;
    }
    return i;
}

unbounded_int string2unbounded_int(const char *e) {
    unbounded_int *tmp = unbounded_int_creer();
    int cpt = 0;
    while(e[cpt] != '\0') {
        tmp = unbounded_int_ajouter_char_fin(tmp, e[cpt]);
        cpt++;
    }
    if(atoi(e) < 0) {
        tmp -> signe = '-';
    } else {
        tmp -> signe = '+';
    }
    return *tmp;
}

unbounded_int ll2unbounded_int(long long i) {
    unbounded_int *tmp = unbounded_int_creer();
    char e[UNKNOW_SIZE]; 
    sprintf(e, "%lld", i);
    int cpt = 0;
    while(e[cpt] != '\0') {
        tmp = unbounded_int_ajouter_char_fin(tmp, e[cpt]);
        cpt++;
    }
    if(i < 0) {
        tmp -> signe = '-';
    } else {
        tmp -> signe = '+';
    }
    return *tmp;
}

char *unbounded_int2string(unbounded_int i) {
    int len = i.len,
        cpt = 0;
    chiffre *c = i.premier;
    if(i.signe == '-') {
        len++;
    }
    char *e = malloc(sizeof(char) * len);
    if(e == NULL) {
        return NULL;
    }
    if(i.signe == '-') {
        e[cpt] = i.signe;
        cpt++;
    }
    while(cpt < len) {
        e[cpt] = c -> c;
        c = c -> suivant;
        cpt++;
    }
    return e;
}

int unbounded_int_cmp_unbounded_int(unbounded_int a, unbounded_int b) {
    if(a.signe == '-' && b.signe == '+') {
        return -1;
    }
    if(a.signe == '+' && b.signe == '-') {
        return 1;
    }
    if(a.len < b.len) {
        if(a.signe == '+' && b.signe == '+') {
            return -1;
        } 
        return 1;
    }
    if(a.len > b.len) {
        if(a.signe == '+' && b.signe == '+') {
            return 1;
        } 
        return -1;
    }
    chiffre *i = a.premier,
            *j = b.premier;
    while(i != NULL && j != NULL) {
        if(i -> c < j -> c) {
            if(a.signe == '+' && b.signe == '+') {
                return -1;
            } else {
                return 1;
            }
        }
        if(i -> c > j -> c) {
            if(a.signe == '+' && b.signe == '+') {
                return 1;
            } else {
                return -1;
            }
        }
        i = i -> suivant;
        j = j -> suivant;
    }
    return 0;
}

int unbounded_int_cmp_ll(unbounded_int a, long long b) {
    unbounded_int b_bis = ll2unbounded_int(b);
    return unbounded_int_cmp_unbounded_int(a, b_bis);
}

static int unbounded_int_cmp_unbounted_int_no_sign(unbounded_int a, unbounded_int b) {
    if(a.len < b.len) {
        return -1;
    }
    if(a.len > b.len) {
        return 1;
    }
    chiffre *i = a.premier,
            *j = b.premier;
    while(i != NULL && j != NULL) {
        if(i -> c < j -> c) {
            if(a.signe == '+' && b.signe == '+') {
                return -1;
            } else {
                return 1;
            }
        } else if(i -> c > j -> c) {
            if(a.signe == '+' && b.signe == '+') {
                return 1;
            } else {
                return -1;
            }
        }
        i = i -> suivant;
        j = j -> suivant;
    }
    return 0;
}

unbounded_int unbounded_int_somme(unbounded_int a, unbounded_int b) {
    unbounded_int *res = unbounded_int_creer();
    int retenu = 0;
    chiffre *i = a.dernier,
            *j = b.dernier;
    while(i != NULL && j != NULL) {
        int int_i = i -> c - '0',
            int_j = j -> c - '0',
            somme;
        if(a.signe == '-' && b. signe == '+') {
            somme = int_j - int_i + retenu;
        } else if(a.signe == '+' && b.signe == '-'){
            somme = int_i - int_j + retenu;
        } else {
            somme = int_i + int_j + retenu;
        }
        retenu = 0;
        if(somme >= 10) {
            retenu = 1;
        }
        char chiffre[SIZE_INT_IN_STRING];
        sprintf(chiffre, "%d", somme);
        if(retenu == 0) {
            unbounded_int_ajouter_char_debut(res, chiffre[0]);
        } else {
            unbounded_int_ajouter_char_debut(res, chiffre[1]);
        }
        i = i -> precedent;
        j = j -> precedent;
    }
    while(i != NULL) {
        int int_i = i -> c - '0',
            somme;
        if(a.signe == '-') {
            somme = int_i - retenu;
        } else {
            somme = retenu + int_i;
        }
        retenu = 0;
        if(somme >= 10) {
            retenu = 1;
        }
        char chiffre[SIZE_INT_IN_STRING];
        sprintf(chiffre, "%d", somme);
        if(retenu == 0) {
            unbounded_int_ajouter_char_debut(res, chiffre[0]);
        } else {
            unbounded_int_ajouter_char_debut(res, chiffre[1]);
        }
        i = i -> precedent;
    }
    while(j != NULL) {
        int int_j = j -> c - '0',
            somme;
        if(b.signe == '-') {
            somme = int_j - retenu;
        } else {
            somme = retenu + int_j;
        }
        retenu = 0;
        if(somme >= 10) {
            retenu = 1;
        }
        char chiffre[SIZE_INT_IN_STRING];
        sprintf(chiffre, "%d", somme);
        if(retenu == 0) {
            unbounded_int_ajouter_char_debut(res, chiffre[0]);
        } else {
            unbounded_int_ajouter_char_debut(res, chiffre[1]);
        }
        j = j -> precedent;
    }
    if(retenu == 1) {
        unbounded_int_ajouter_char_debut(res, '1');
    }
    if(a.signe == '+' && b.signe == '+') {
        res -> signe = '+';
    } else if(a.signe == '-' && b.signe == '-') {
        res -> signe = '-';
    } else if(a.signe == '-' && b.signe == '+') {
        if(unbounded_int_cmp_unbounted_int_no_sign(a, b) == 1) {
            res -> signe = '-';
        } else {
            res -> signe = '+';
        }
    } else {
        if(unbounded_int_cmp_unbounted_int_no_sign(a, b) == -1) {
            res -> signe = '-';
        } else {
            res -> signe = '+';
        }
    }
    return *res;
}

unbounded_int unbounded_int_difference(unbounded_int a, unbounded_int b) {
    unbounded_int *res = unbounded_int_creer();
    int retenu = 0;
    chiffre *i = a.dernier,
            *j = b.dernier;
    if(unbounded_int_cmp_unbounted_int_no_sign(a, b) == -1) {
        i = b.dernier;
        j = a.dernier;
    }
    while(i != NULL && j != NULL) {
        int int_i = i -> c - '0',
            int_j = j -> c - '0',
            difference;
        if(a.signe == '+' && b.signe == '+') { 
            if(int_i < int_j) {
                difference = CONST_SOUSTRACTION + int_i - int_j + retenu;
                retenu = 1;
            } else {
                difference = int_i - int_j + retenu;
                retenu = 0;
            }
        } else if(a.signe == '-' && b.signe == '-') {
            if(int_i < int_j) {
                difference = CONST_SOUSTRACTION - int_i + int_j + retenu;
                retenu = 1;
            } else {
                difference = int_j - int_i + retenu;
                retenu = 0;
            }
        } else {
            if(int_i < int_j) {

            }
            difference = int_i + int_j + retenu;
            retenu = 0;
            if(difference >= 10) {
                retenu = 1;
            }
        }
        char chiffre[SIZE_INT_IN_STRING];
        sprintf(chiffre, "%d", difference);
        unbounded_int_ajouter_char_debut(res, chiffre[0]);
        i = i -> precedent;
        j = j -> precedent;
    }
    while(i != NULL) {
        int int_i = i -> c - '0',
            difference;
        difference = int_i - retenu;
        retenu = 0;
        char chiffre[SIZE_INT_IN_STRING];
        sprintf(chiffre, "%d", difference);
        unbounded_int_ajouter_char_debut(res, chiffre[0]);
        i = i -> precedent;
    }
    while(j != NULL) {
        int int_j = j -> c - '0',
            difference;
        difference = int_j + retenu;
        retenu = 0;
        if(difference >= 10) {
            retenu = 1;
        }
        char chiffre[SIZE_INT_IN_STRING];
        sprintf(chiffre, "%d", difference);
        if(retenu == 0) {
            unbounded_int_ajouter_char_debut(res, chiffre[0]);
        } else {
            unbounded_int_ajouter_char_debut(res, chiffre[1]);
        }
        j = j -> precedent;
    }
    if(retenu == 1) {
        unbounded_int_ajouter_char_debut(res, '1');
    }
    if(a.signe == '+' && b.signe == '+') {
        if(unbounded_int_cmp_unbounted_int_no_sign(a, b) == 1) {
            res -> signe = '+';
        } else {
            res -> signe = '-';
        }
    } else if(a.signe == '-' && b.signe == '-') {
        if(unbounded_int_cmp_unbounted_int_no_sign(a, b) == 1) {
            res -> signe = '-';
        } else {
            res -> signe = '+';
        }
    } else if(a.signe == '-' && b.signe == '+') {
        res -> signe = '-';
    } else {
        res -> signe = '+';
    }
    return *res;
}

static int dix_puissance(int acc, int n) {
    if(n == 0) {
        return acc;
    }
    return dix_puissance(acc * 10, n - 1);
}

unbounded_int unbounded_int_produit( unbounded_int a, unbounded_int b) {
    unbounded_int *tmp = unbounded_int_creer(),
                  *somme = unbounded_int_creer_zero();
    int retenu = 0,
        cpt = 0,
        produit;
    chiffre *j = b.dernier;
    while(j != NULL) {
        chiffre *i = a.dernier;
        int int_j = j -> c - '0';
        while(i != NULL) {
            int int_i = i -> c - '0';
            produit = int_j * int_i + retenu;
            retenu = 0;
            char chiffre[SIZE_INT_IN_STRING];
            sprintf(chiffre, "%d", produit);
            if(produit >= 10) {
                retenu = chiffre[0] - '0';
                unbounded_int_ajouter_char_debut(tmp, chiffre[1]);
            } else {
                unbounded_int_ajouter_char_debut(tmp, chiffre[0]);
            }
            i = i -> precedent;
        }
        char *produit_in_char = unbounded_int2string(*tmp),
             *ptrEnd;
        long long produit_in_long_long = strtoll(produit_in_char, &ptrEnd, 10);
        if(cpt > 0) {
            produit_in_long_long *= dix_puissance(1, cpt);
        }
        unbounded_int produit_in_unbounded_int = ll2unbounded_int(produit_in_long_long);
        *somme = unbounded_int_somme(*somme, produit_in_unbounded_int);
        tmp = unbounded_int_creer();
        cpt++;
        j = j -> precedent;
    }
    if((a.signe == '+' && b.signe == '+') || (a.signe == '-' && b.signe == '-')) {
        somme -> signe = '+';
    } else {
        somme -> signe = '-';
    }
    return *somme;
}
/*
int main() {

    char chiffre_string1[] = "1234567890";
    unbounded_int chiffre1 = string2unbounded_int(chiffre_string1);
    char *tmp1 = unbounded_int2string(chiffre1);
    printf("Chiffre 1 = %s\n", tmp1);

    char chiffre_string2[] = "10";
    unbounded_int chiffre2 = string2unbounded_int(chiffre_string2);
    char *tmp2 = unbounded_int2string(chiffre2);
    printf("Chiffre 2 = %s\n", tmp2);

    unbounded_int chiffre3 = unbounded_int_somme(chiffre1, chiffre2);
    char *tmp3 = unbounded_int2string(chiffre3);
    printf("Chiffre 3 = %s + %s = %s\n", tmp1, tmp2, tmp3);

    unbounded_int chiffre4 = unbounded_int_difference(chiffre3, chiffre2);
    char *tmp4 = unbounded_int2string(chiffre4);
    printf("Chiffre 4 = %s - %s = %s\n", tmp3, tmp2, tmp4);

    unbounded_int chiffre5 = unbounded_int_produit(chiffre1, chiffre2);
    char *tmp5 = unbounded_int2string(chiffre5);
    printf("%s\n", tmp5);*/
/*
    printf("Chiffre 5 = %s * %s = %s\n", tmp1, tmp2, tmp5);

    unbounded_int chiffre6 = unbounded_int_somme(chiffre2, chiffre1);
    char *tmp6 = unbounded_int2string(chiffre6);
    printf("Chiffre 6 = %s + %s = %s\n", tmp2, tmp1, tmp6);

    unbounded_int chiffre7 = unbounded_int_difference(chiffre2, chiffre1);
    char *tmp7 = unbounded_int2string(chiffre7);
    printf("%s\n", tmp7);*/
/*
    long long chiffre_long_long = -10;
    unbounded_int chiffre8 = ll2unbounded_int(chiffre_long_long);
    printf("Chiffre 7 = %s - %s = %s\n", tmp2, tmp1, tmp7);

    long long chiffre8_ll = -1234567890;
    unbounded_int chiffre8 = ll2unbounded_int(chiffre8_ll);
    char *tmp8 = unbounded_int2string(chiffre8);
    printf("Chiffre 8 = %s\n", tmp8);

    unbounded_int chiffre9 = unbounded_int_somme(chiffre1, chiffre8);
    char *tmp9 = unbounded_int2string(chiffre9);
    printf("Chiffre 9 = %s + %s = %s\n", tmp1, tmp8, tmp9);

    unbounded_int chiffre10 = unbounded_int_somme(chiffre2, chiffre8);
    char *tmp10 = unbounded_int2string(chiffre10);
    printf("Chiffre 10 = %s + %s = %s\n", tmp2, tmp8, tmp10);

    unbounded_int chiffre11 = unbounded_int_difference(chiffre1, chiffre8);
    char *tmp11 = unbounded_int2string(chiffre11);
    printf("Chiffre 11 = %s - %s = %s\n", tmp1, tmp8, tmp11);

    unbounded_int chiffre12 = unbounded_int_difference(chiffre8, chiffre1);
    char *tmp12 = unbounded_int2string(chiffre12);
    printf("Chiffre 12 = %s - %s = %s\n", tmp8, tmp1, tmp12);

    long long chiffre13_ll = -2;
    unbounded_int chiffre13 = ll2unbounded_int(chiffre13_ll);
    char *tmp13 = unbounded_int2string(chiffre13);
    printf("Chiffre 13 = %s\n", tmp13);

    unbounded_int chiffre14 = unbounded_int_produit(chiffre13, chiffre1);
    char *tmp14 = unbounded_int2string(chiffre14);
    printf("Chiffre 5 = %s * %s = %s\n", tmp13, tmp1, tmp14);
}
*/
