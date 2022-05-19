#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "unbounded_int.h"

#define SIZE_INT_IN_STRING 3
#define CONST_SOUSTRACTION 10
#define UNKNOW_SIZE 256

//Creer un chiffre en précisant s'il est suivi ou precede d'un autre chiffre
static chiffre *chiffre_creer(chiffre *suivant, char c, chiffre *precedent) {
    if(!isdigit(c)) {
        return NULL;
    }
    chiffre *tmp = malloc(sizeof(chiffre));
    if(tmp == NULL) {
        return NULL;
    }
    tmp -> suivant = suivant;
    tmp -> c = c;
    tmp -> precedent = precedent;
    return tmp;
}

//Creer un chiffre
static chiffre *chiffre_creer_char(char c) {
    return chiffre_creer(NULL, c, NULL);
}

static int verification(const unbounded_int *i) {
    if(i -> signe == '*') {
        return 0;
    } else {
        return 1;
    }
}

//Creer un unbounded_int valant 0
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

//Creer un unbounded_int vide
static unbounded_int *unbounded_int_creer() {
    unbounded_int *tmp = malloc(sizeof(unbounded_int));
    if(tmp == NULL) {
        tmp -> signe = '*';
    }
    tmp -> len = 0;
    tmp -> premier = NULL;
    tmp -> dernier = NULL;
    return tmp;
}

//Ajouter un chiffre a la fin d'un unbounded_int
static unbounded_int *unbounded_int_ajouter_chiffre_fin(unbounded_int *i, chiffre *c) {
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

//Ajouter un caractere valant un chiffre a la fin d'un unbounded_int
static unbounded_int *unbounded_int_ajouter_char_fin(unbounded_int *i, char c) {
    chiffre *tmp = chiffre_creer_char(c);
    if(tmp == NULL) {
        if(!verification(i)) {
            i -> signe = '*';
        }
        return i;
    }
    return unbounded_int_ajouter_chiffre_fin(i, tmp);
}

//Ajouter un chiffre au debut d'un unbounded_int
static unbounded_int *unbounded_int_ajouter_chiffre_debut(unbounded_int *i, chiffre *c) {
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

//Ajouter un caractere valant un chiffre au debut d'un unbounded_int
static unbounded_int *unbounded_int_ajouter_char_debut(unbounded_int *i, char c) {
    chiffre *tmp = chiffre_creer_char(c);
    if(tmp == NULL) {
        if(!verification(i)) {
            i -> signe = '*';
        }
        return i;
    }
    return unbounded_int_ajouter_chiffre_debut(i, tmp);
}

//Convertir une chaine de caractere en un unbounded_int
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

//Convertir un long long en un unbounded_int
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

//Convertir un unbounded_int en une chaine de caracteres
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

/* Comparer deux unbounded_int a et b
   Retourne -1 si a < b
             0 si a = b
             1 si a > b */
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

/* Comparer un unbounded_int a et un long long b
   Retourne -1 si a < b
             0 si a = b
             1 si a > b */
int unbounded_int_cmp_ll(unbounded_int a, long long b) {
    unbounded_int b_bis = ll2unbounded_int(b);
    return unbounded_int_cmp_unbounded_int(a, b_bis);
}

/* Comparer la valeur absolue de deux unbounded_int a et b
   Retourne -1 si |a| < |b|
             0 si |a| = |b|
             1 si |a| > |b| */
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

//Calculer la somme de deux unbounded_int a et b
unbounded_int unbounded_int_somme(unbounded_int a, unbounded_int b) {
    unbounded_int *res = unbounded_int_creer(); //Unbounded_int contenant le resultat de l'addition
    int retenu = 0; //Int representant la retenu dans une addition
    chiffre *i = a.dernier,
            *j = b.dernier;
    while(i != NULL && j != NULL) {
        int int_i = i -> c - '0',
            int_j = j -> c - '0',
            somme;
        //Calcul de la somme des deux chiffres
        if(a.signe == '-' && b. signe == '+') {
            somme = int_j - int_i + retenu;
        } else if(a.signe == '+' && b.signe == '-'){
            somme = int_i - int_j + retenu;
        } else {
            somme = int_i + int_j + retenu;
        }
        //Reinitialisation de la retenu après utilisation
        retenu = 0;
        //Mis en place de la retenu si besoin
        if(somme >= 10) {
            retenu = 1;
        }
        //Ajout du résultat
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
    //Cas ou le unbounded_int a est plus long que le unbounded_int b
    while(i != NULL) {
        int int_i = i -> c - '0',
            somme;
        //Calcul de la somme
        if(a.signe == '-') {
            somme = int_i - retenu;
        } else {
            somme = retenu + int_i;
        }
        //Reinitialisation de la retenu après utilisation
        retenu = 0;
        //Mis en place de la retenu si besoin
        if(somme >= 10) {
            retenu = 1;
        }
        //Ajout du résultat
        char chiffre[SIZE_INT_IN_STRING];
        sprintf(chiffre, "%d", somme);
        if(retenu == 0) {
            unbounded_int_ajouter_char_debut(res, chiffre[0]);
        } else {
            unbounded_int_ajouter_char_debut(res, chiffre[1]);
        }
        i = i -> precedent;
    }
    //Cas ou le unbounded_int b est plus long que le unbounded_int a
    while(j != NULL) {
        int int_j = j -> c - '0',
            somme;
        //Calcul de la somme
        if(b.signe == '-') {
            somme = int_j - retenu;
        } else {
            somme = retenu + int_j;
        }
        //Reinitialisation de la retenu après utilisation
        retenu = 0;
        //Mis en place de la retenu si besoin
        if(somme >= 10) {
            retenu = 1;
        }
        //Ajout du résultat
        char chiffre[SIZE_INT_IN_STRING];
        sprintf(chiffre, "%d", somme);
        if(retenu == 0) {
            unbounded_int_ajouter_char_debut(res, chiffre[0]);
        } else {
            unbounded_int_ajouter_char_debut(res, chiffre[1]);
        }
        j = j -> precedent;
    }
    //Ajout de la retenu si elle existe toujours lorsqu'on finit d'additionner a et b
    if(retenu == 1) {
        unbounded_int_ajouter_char_debut(res, '1');
    }
    //Mis en place du signe du resultat
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

//Calculer la difference de deux unbounded_int a et b
unbounded_int unbounded_int_difference(unbounded_int a, unbounded_int b) {
    unbounded_int *res = unbounded_int_creer();
    int retenu = 0;
    chiffre *i = a.dernier,
            *j = b.dernier;
    //Cas ou la valeur de a est strinctement plus petite à celle de b
    if(unbounded_int_cmp_unbounded_int(a, b) == -1) {
        i = b.dernier;
        j = a.dernier;
    }
    while(i != NULL && j != NULL) {
        int int_i = i -> c - '0',
            int_j = j -> c - '0',
            difference;
        //Calcul de la difference
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
            difference = int_i + int_j + retenu;
            retenu = 0;
            if(difference >= 10) {
                retenu = 1;
            }
        }
        //Ajout du resultat
        char chiffre[SIZE_INT_IN_STRING];
        sprintf(chiffre, "%d", difference);
        if(retenu == 0) {
            unbounded_int_ajouter_char_debut(res, chiffre[0]);
        } else {
            unbounded_int_ajouter_char_debut(res, chiffre[1]);
        }
        i = i -> precedent;
        j = j -> precedent;
    }
    //Cas ou a est plus long que b
    while(i != NULL) {
        int int_i = i -> c - '0',
            difference;
        //Calcul de la difference
        difference = int_i - retenu;
        //Reinitialisation de la retenu
        retenu = 0;
        //La retenu ne changera pas, car on suppose que a ne commencera jamais par un nombre quelconque de 0
        //Ajout du resultat
        char chiffre[SIZE_INT_IN_STRING];
        sprintf(chiffre, "%d", difference);
        unbounded_int_ajouter_char_debut(res, chiffre[0]);
        i = i -> precedent;
    }
    //Cas ou b est plus long que a
    while(j != NULL) {
        int int_j = j -> c - '0',
            difference;
        //Calcul de la difference
        difference = int_j + retenu;
        //Reinitialisation de la retenu
        retenu = 0;
        //Mis en place de la retenu
        if(difference >= 10) {
            retenu = 1;
        }
        //Ajout du resultat
        char chiffre[SIZE_INT_IN_STRING];
        sprintf(chiffre, "%d", difference);
        if(retenu == 0) {
            unbounded_int_ajouter_char_debut(res, chiffre[0]);
        } else {
            unbounded_int_ajouter_char_debut(res, chiffre[1]);
        }
        j = j -> precedent;
    }
    //Cas ou il existe toujours une retenu apres avoir soustrait b a a
    if(retenu == 1) {
        unbounded_int_ajouter_char_debut(res, '1');
    }
    //Mis en place du signe du resultat
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

//Calculer 10 a un certaine puissance n
static int dix_puissance(int acc, int n) {
    if(n == 0) {
        return acc;
    }
    return dix_puissance(acc * 10, n - 1);
}

//Calculer le produit de deux unbounded_int a et b
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
            //Calcul du produit
            produit = int_j * int_i + retenu;
            //Reinitialisation de la retenu
            retenu = 0;
            //Ajout du resultat du produit
            char chiffre[SIZE_INT_IN_STRING];
            sprintf(chiffre, "%d", produit);
            if(produit >= 10) {
                //Mis en place de la retenu
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
        //Ajout du decalage
        if(cpt > 0) {
            produit_in_long_long *= dix_puissance(1, cpt);
        }
        //Addition du produit
        unbounded_int produit_in_unbounded_int = ll2unbounded_int(produit_in_long_long);
        *somme = unbounded_int_somme(*somme, produit_in_unbounded_int);
        tmp = unbounded_int_creer();
        cpt++;
        j = j -> precedent;
    }
    //Mis en place du signe du resultat
    if((a.signe == '+' && b.signe == '+') || (a.signe == '-' && b.signe == '-')) {
        somme -> signe = '+';
    } else {
        somme -> signe = '-';
    }
    return *somme;
}

//Calculer le quotien de la division entiere d'un unbounded_int a par un unbounded_int b
unbounded_int unbounded_int_quotien(unbounded_int a, unbounded_int b) {
    if(unbounded_int_cmp_unbounted_int_no_sign(a, b) == -1) {
        return *unbounded_int_creer_zero();
    }
    unbounded_int *res = unbounded_int_creer();
    
    return *res;
}

//Calculer le reste de la division d'un unbounded_int a par un unbounded_int b
unbounded_int unbounded_int_modulo( unbounded_int a, unbounded_int b) {
    if(unbounded_int_cmp_unbounted_int_no_sign(a, b) == -1) {
        char *tmp = unbounded_int2string(a);
        return string2unbounded_int(tmp);
    }
    return *unbounded_int_creer();
}


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
    printf("%s\n", tmp5);
    printf("Chiffre 5 = %s * %s = %s\n", tmp1, tmp2, tmp5);

    unbounded_int chiffre6 = unbounded_int_somme(chiffre2, chiffre1);
    char *tmp6 = unbounded_int2string(chiffre6);
    printf("Chiffre 6 = %s + %s = %s\n", tmp2, tmp1, tmp6);

    long long chiffre_long_long = -10;
    unbounded_int chiffre7 = ll2unbounded_int(chiffre_long_long);
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
    printf("Chiffre 14 = %s * %s = %s\n", tmp13, tmp1, tmp14);

    unbounded_int chiffre15 = unbounded_int_produit(chiffre14, chiffre13);
    char *tmp15 = unbounded_int2string(chiffre15);
    printf("Chiffre 15 = %s * %s = %s\n", tmp14, tmp13, tmp15);
}

