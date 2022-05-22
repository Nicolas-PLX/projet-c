#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "unbounded_int.h"

#define SIZE_INT_IN_STRING 3
#define CONST_SOUSTRACTION 10
#define UNKNOW_SIZE 1024
#define BIG_UNKNOW_SIZE 2048

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
        return tmp;
    }
    tmp -> signe = '+';
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

/*Determine si un unbounded_int a est vide
  Retourne 0 si a n'est pas vide 
           1 si a est vide  */
static int unbounded_int_is_empty(unbounded_int a) {
    if(a.len != 0) {
        return 0;
    }
    if(a.premier != NULL) {
        return 0;
    }
    if(a.dernier != NULL) {
        return 0;
    }
    return 1;
}

/*Determine si un unbounded_int a est equivalent a 0
  Retourne 0 si a != 0 
           1 si a = 0  */
static int unbounded_int_is_zero(unbounded_int a) {
    chiffre *i = a.premier;
    while(i != NULL) {
        if(i -> c != '0') {
            return 0;
        }
        i = i -> suivant;
    }
    a = *unbounded_int_creer_zero();
    return 1;
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
        if(isdigit(e[cpt])) {
            tmp = unbounded_int_ajouter_char_fin(tmp, e[cpt]);
        }
        cpt++;
    }
    if(e[0] == '-') {
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
    if(unbounded_int_is_zero(i)) {
        return "0";
    }
    if(i.signe == '-') {
        len++;
    }
    char *e = malloc(sizeof(char) * len + 1);
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
    e[len] = '\0';
    return e;
}

//Cinvertir un unbounded_int en long long
static long long unbounded_int2ll(unbounded_int a) {
    long long cpt = 0,
        decalage = 1;
    chiffre *i = a.premier;
    while(i != NULL) {
        int chiffre = i -> c - '0';
        cpt = (cpt * decalage) + chiffre;
        if(decalage == 1) {
            decalage = 10;
        }
        i = i -> suivant;
    }
    if(a.signe == '-') {
        cpt *= -1;
    }
    return cpt;
}

//Convertir un unbounded_int a en sa valeur absolue
static unbounded_int unbounded_int_absolute_value(unbounded_int a) {
    unbounded_int *res = unbounded_int_creer();
    chiffre *i = a.premier;
    while(i != NULL) {
        unbounded_int_ajouter_char_fin(res, i -> c);
        i = i -> suivant;
    }
    res -> signe = '+';
    return *res;
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
static int unbounded_int_cmp_unbounted_int_absolute_value(unbounded_int a, unbounded_int b) {
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
            return -1;
        } else if(i -> c > j -> c) {
            return 1;
        } 
        i = i -> suivant;
        j = j -> suivant;
    }
    return 0;
}

/* Comparer un unbounded_int a a 0
   Retourne -1 si a < 0
             0 si a = 0
             1 si a > 0 */
static int unbounded_int_cmp(unbounded_int a) {
    if(a.signe == '-') {
        return -1;
    }
    if(a.len == 1 && a.premier -> c == '0') {
        return 0;
    }
    return 1;
}

//Calculer la somme de deux unbounded_int a et b positifs
static unbounded_int unbounded_int_somme_simple(unbounded_int a, unbounded_int b) {
    unbounded_int *res = unbounded_int_creer(); //Unbounded_int contenant le resultat de l'addition
    chiffre *i = a.dernier,
            *j = b.dernier;
    int retenu = 0; //Int representant la retenu dans une addition
    while(i != NULL && j != NULL) {
        int int_i = i -> c - '0',
            int_j = j -> c - '0',
            somme = int_i + int_j + retenu;
        //Reinitialisation de la retenu après utilisation
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
    //Cas ou le unbounded_int a est plus long que le unbounded_int b
    while(i != NULL) {
        int int_i = i -> c - '0',
            somme = int_i + retenu;
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
            somme = int_j + retenu;
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
    return *res;
}

//Calculer la difference de deux unbounded_int a et b positifs ou a >= b
static unbounded_int unbounded_int_difference_simple(unbounded_int a, unbounded_int b) {
    unbounded_int *res = unbounded_int_creer(); //Unbounded_int contenant le resultat de l'addition
    chiffre *i = a.dernier,
            *j = b.dernier;
    if(unbounded_int_cmp_unbounted_int_absolute_value(a, b) < 0) {
        i = b.dernier;
        j = a.dernier;
    }
    int retenu = 0; //Int representant la retenu dans une addition
    while(i != NULL && j != NULL) { 
        int int_i = i -> c - '0',
            int_j = j -> c - '0',
            difference;
        if(int_i < int_j + retenu) {
            difference = (CONST_SOUSTRACTION + int_i) - (int_j + retenu);
            retenu = 1;
        } else {
            difference = int_i - (int_j + retenu);
            retenu = 0;
        }
        i = i -> precedent;
        j = j -> precedent;
        //Ajout du resultat
        char chiffre[SIZE_INT_IN_STRING];
        sprintf(chiffre, "%d", difference);
        unbounded_int_ajouter_char_debut(res, chiffre[0]);
    } 
    //Cas ou a est plus long que b
    while(i != NULL) {
         int int_i = i -> c - '0',
            difference;
        if(int_i < retenu) {
            difference = (CONST_SOUSTRACTION + int_i) - retenu;
            retenu = 1;
        } else {
            difference = int_i - retenu;
            retenu = 0;
        }
        //Ajout du resultat
        char chiffre[SIZE_INT_IN_STRING];
        sprintf(chiffre, "%d", difference);
        unbounded_int_ajouter_char_debut(res, chiffre[0]);
        i = i -> precedent;
    }
    long long tmp = unbounded_int2ll(*res);
    *res = ll2unbounded_int(tmp);
    return *res;
}

//Calculer la somme de deux unbounded_int a et b
unbounded_int unbounded_int_somme(unbounded_int a, unbounded_int b) {
    unbounded_int res;
    if(unbounded_int_cmp(a) >= 0) {
        if(unbounded_int_cmp(b) >= 0) {
            res = unbounded_int_somme_simple(a, b);
            res.signe = '+';
        } else {
            res = unbounded_int_difference_simple(a, unbounded_int_absolute_value(b));
            if(unbounded_int_cmp_unbounted_int_absolute_value(a, b) >= 0) {
                res.signe = '+';
            } else {
                res.signe = '-';
            }
        }
    } else {
        if(unbounded_int_cmp(b) >= 0) {
            res = unbounded_int_difference_simple(b, unbounded_int_absolute_value(a));
            if(unbounded_int_cmp_unbounted_int_absolute_value(a, b) >= 0) {
                res.signe = '-';
            } else {
                res.signe = '+';
            }
        } else {
            res = unbounded_int_somme_simple(unbounded_int_absolute_value(a), unbounded_int_absolute_value(b));
            res.signe = '-';
        }
    }
    return res;
}

//Calculer la difference de deux unbounded_int a et b
unbounded_int unbounded_int_difference(unbounded_int a, unbounded_int b) {
    unbounded_int res;
    if(unbounded_int_cmp(a) >= 0) {
        if(unbounded_int_cmp(b) >= 0) {
            res = unbounded_int_difference_simple(a, b);
            if(unbounded_int_cmp_unbounted_int_absolute_value(a, b) >= 0) {
                res.signe = '+';
            } else {
                res.signe = '-';
            }
        } else {
            res = unbounded_int_somme_simple(a, unbounded_int_absolute_value(b));
            res.signe = '+';
        }
    } else {
        if(unbounded_int_cmp(b) >= 0) {
            res = unbounded_int_somme_simple(b, unbounded_int_absolute_value(a));
            res.signe = '-';
        } else {
            res = unbounded_int_difference_simple(unbounded_int_absolute_value(a), unbounded_int_absolute_value(b));
            if(unbounded_int_cmp_unbounted_int_absolute_value(a, b) >= 0) {
                res.signe = '-';
            } else {
                res.signe = '+';
            }
        }
    }
    return res;
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
        if(retenu) {
            char chiffre[SIZE_INT_IN_STRING];
            sprintf(chiffre, "%d", retenu);
            unbounded_int_ajouter_char_debut(tmp, chiffre[0]);
            retenu = 0;
        }
        for(int i = 0; i < cpt; i++) {
            unbounded_int_ajouter_char_fin(tmp, '0');
        }
        //Addition du produit
        *somme = unbounded_int_somme(*somme, *tmp);
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

//Calculer le quotient de la division entiere d'un unbounded_int a par un unbounded_int b
unbounded_int unbounded_int_quotient(unbounded_int a, unbounded_int b) {
    if(unbounded_int_is_zero(b)) {
        printf("On ne peut pas diviser un entier par 0.\n");
        abort();
    }
    if(unbounded_int_cmp_unbounted_int_absolute_value(a, b) == -1) {
        return *unbounded_int_creer_zero();
    }
    unbounded_int *res = unbounded_int_creer();
    long long cpt = 0,
              ll = unbounded_int2ll(b),
              reste;
    int decalage = 1,
        quotient;
    chiffre *i = a.premier;
    if(b.signe == '-') {
        ll *= -1;
    }
    while(i != NULL) {
        int int_i = i -> c - '0';
        cpt = (cpt * decalage) + int_i;
        if(decalage == 1) {
            decalage = 10;
        }
        if(ll <= cpt) {
            quotient = cpt / ll;
            reste = cpt % ll;
            char chiffre[SIZE_INT_IN_STRING];
            sprintf(chiffre, "%d", quotient);
            unbounded_int_ajouter_char_fin(res, chiffre[0]);
            cpt = reste;
        } else if(cpt == 0) {
            if(!unbounded_int_is_empty(*res)) {
                unbounded_int_ajouter_char_fin(res, '0');
            }
        }
        i = i -> suivant;
    }
    if(a.signe == '+') {
        if(b.signe == '+') {
            res -> signe = '+';
        } else {
            res -> signe = '-';
        }
    } else {
        if(b.signe == '+') {
            res -> signe = '-';
        } else {
            res -> signe = '+';
        }
    }

    return *res;
}

//Calculer le reste de la division d'un unbounded_int a par un unbounded_int b
unbounded_int unbounded_int_modulo( unbounded_int a, unbounded_int b) {
    if(unbounded_int_is_zero(b)) { 
        printf("On ne peut pas diviser un entier par 0.\n");
        abort();
    }
    if(a.signe == '-') {
        printf("On considère qu'on ne peut pas calculer le modulo d'un nombre négatif.\n");
        abort();
    }
    if(b.signe == '-') {
        printf("On considère qu'on ne peut pas calculer le modulo d'un nombre négatif.\n");
        abort();
    }
    if(unbounded_int_cmp_unbounted_int_absolute_value(a, b) == -1) {
        char *tmp = unbounded_int2string(a);
        unbounded_int res = string2unbounded_int(tmp);
        res.signe = '+';
        return res;
    }
    long long cpt = 0,
              ll = unbounded_int2ll(b),
              reste;
    int decalage = 1;
    chiffre *i = a.premier;
    if(b.signe == '-') {
        ll *= -1;
    }
    while(i != NULL) {
        int int_i = i -> c - '0';
        if(cpt) {
            cpt = (cpt * decalage) + int_i;
        } else {
            cpt += int_i;
        }
        if(decalage == 1) {
            decalage = 10;
        }
        if(ll <= cpt) {
            reste = cpt % ll;
            cpt = reste;
            decalage = 1;
        }
        i = i -> suivant;
    }
    if(reste < 0) {
        reste *= -1;
    }
    char nombre[BIG_UNKNOW_SIZE];
    sprintf(nombre, "%lld", reste);
    unbounded_int res = string2unbounded_int(nombre);
    res.signe = '+';
    return res;
}
