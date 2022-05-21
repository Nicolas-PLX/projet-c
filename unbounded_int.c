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
    tmp -> signe = '+';
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
    if(unbounded_int_cmp_unbounted_int_no_sign(a, b) < 0) {
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
        if(retenu == 0) {
            unbounded_int_ajouter_char_debut(res, chiffre[0]);
        } else {
            unbounded_int_ajouter_char_debut(res, chiffre[1]);
        }
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
        if(retenu == 0) {
            unbounded_int_ajouter_char_debut(res, chiffre[0]);
        } else {
            unbounded_int_ajouter_char_debut(res, chiffre[1]);
        }
        i = i -> precedent;
    }
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
            if(unbounded_int_cmp_unbounted_int_no_sign(a, b) >= 0) {
                res.signe = '+';
            } else {
                res.signe = '-';
            }
        }
    } else {
        if(unbounded_int_cmp(b) >= 0) {
            res = unbounded_int_difference_simple(b, unbounded_int_absolute_value(a));
            if(unbounded_int_cmp_unbounted_int_no_sign(a, b) >= 0) {
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
            if(unbounded_int_cmp_unbounted_int_no_sign(a, b) >= 0) {
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
            if(unbounded_int_cmp_unbounted_int_no_sign(a, b) >= 0) {
                res.signe = '-';
            } else {
                res.signe = '+';
            }
        }
    }
    return res;
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

    char string1[] = "999",
         string2[] = "1",
         string3[] = "-999",
         string4[] = "-1";
    unbounded_int chiffre1 = string2unbounded_int(string1),
                  chiffre2 = string2unbounded_int(string2),
                  chiffre3 = string2unbounded_int(string3),
                  chiffre4 = string2unbounded_int(string4);
    char *tmp1 = unbounded_int2string(chiffre1),
         *tmp2 = unbounded_int2string(chiffre2),
         *tmp3 = unbounded_int2string(chiffre3),
         *tmp4 = unbounded_int2string(chiffre4);

    printf("Chiffre 1 = %s\n", tmp1);
    printf("Chiffre 2 = %s\n", tmp2);
    printf("Chiffre 3 = %s\n", tmp3);
    printf("Chiffre 4 = %s\n", tmp4);

    printf("\n");
    
    unbounded_int somme1 = unbounded_int_somme(chiffre1, chiffre1),
                  somme2 = unbounded_int_somme(chiffre1, chiffre2),
                  somme3 = unbounded_int_somme(chiffre1, chiffre3),
                  somme4 = unbounded_int_somme(chiffre1, chiffre4),
                  somme5 = unbounded_int_somme(chiffre2, chiffre1),
                  somme6 = unbounded_int_somme(chiffre2, chiffre2),
                  somme7 = unbounded_int_somme(chiffre2, chiffre3), 
                  somme8 = unbounded_int_somme(chiffre2, chiffre4),
                  somme9 = unbounded_int_somme(chiffre3, chiffre1),
                  somme10 = unbounded_int_somme(chiffre3, chiffre2), 
                  somme11 = unbounded_int_somme(chiffre3, chiffre3),
                  somme12 = unbounded_int_somme(chiffre3, chiffre4),
                  somme13 = unbounded_int_somme(chiffre4, chiffre1),
                  somme14 = unbounded_int_somme(chiffre4, chiffre2),
                  somme15 = unbounded_int_somme(chiffre4, chiffre3),
                  somme16 = unbounded_int_somme(chiffre4, chiffre4);
    char *tmp_somme1 = unbounded_int2string(somme1),
         *tmp_somme2 = unbounded_int2string(somme2),
         *tmp_somme3 = unbounded_int2string(somme3),
         *tmp_somme4 = unbounded_int2string(somme4),
         *tmp_somme5 = unbounded_int2string(somme5),
         *tmp_somme6 = unbounded_int2string(somme6),
         *tmp_somme7 = unbounded_int2string(somme7),
         *tmp_somme8 = unbounded_int2string(somme8),
         *tmp_somme9 = unbounded_int2string(somme9),
         *tmp_somme10 = unbounded_int2string(somme10),
         *tmp_somme11 = unbounded_int2string(somme11),
         *tmp_somme12 = unbounded_int2string(somme12),
         *tmp_somme13 = unbounded_int2string(somme13),
         *tmp_somme14 = unbounded_int2string(somme14),
         *tmp_somme15 = unbounded_int2string(somme15),
         *tmp_somme16 = unbounded_int2string(somme16);

    printf("Somme 1 : %s + %s = %s\n", tmp1, tmp1, tmp_somme1);
    printf("Somme 2 : %s + %s = %s\n", tmp1, tmp2, tmp_somme2);   
    printf("Somme 3 : %s + %s = %s\n", tmp1, tmp3, tmp_somme3);
    printf("Somme 4 : %s + %s = %s\n", tmp1, tmp4, tmp_somme4);
    printf("Somme 5 : %s + %s = %s\n", tmp2, tmp1, tmp_somme5);
    printf("Somme 6 : %s + %s = %s\n", tmp2, tmp2, tmp_somme6);   
    printf("Somme 7 : %s + %s = %s\n", tmp2, tmp3, tmp_somme7);
    printf("Somme 8 : %s + %s = %s\n", tmp2, tmp4, tmp_somme8); 
    printf("Somme 9 : %s + %s = %s\n", tmp3, tmp1, tmp_somme9);
    printf("Somme 10 : %s + %s = %s\n", tmp3, tmp2, tmp_somme10);   
    printf("Somme 11 : %s + %s = %s\n", tmp3, tmp3, tmp_somme11);
    printf("Somme 12 : %s + %s = %s\n", tmp3, tmp4, tmp_somme12); 
    printf("Somme 13 : %s + %s = %s\n", tmp4, tmp1, tmp_somme13);
    printf("Somme 14 : %s + %s = %s\n", tmp4, tmp2, tmp_somme14);   
    printf("Somme 15 : %s + %s = %s\n", tmp4, tmp3, tmp_somme15);
    printf("Somme 16 : %s + %s = %s\n", tmp4, tmp4, tmp_somme16);    

    printf("\n");   

    unbounded_int difference1 = unbounded_int_difference(chiffre1, chiffre1),
                  difference2 = unbounded_int_difference(chiffre1, chiffre2),
                  difference3 = unbounded_int_difference(chiffre1, chiffre3),
                  difference4 = unbounded_int_difference(chiffre1, chiffre4),
                  difference5 = unbounded_int_difference(chiffre2, chiffre1),
                  difference6 = unbounded_int_difference(chiffre2, chiffre2),
                  difference7 = unbounded_int_difference(chiffre2, chiffre3), 
                  difference8 = unbounded_int_difference(chiffre2, chiffre4),
                  difference9 = unbounded_int_difference(chiffre3, chiffre1),
                  difference10 = unbounded_int_difference(chiffre3, chiffre2), 
                  difference11 = unbounded_int_difference(chiffre3, chiffre3),
                  difference12 = unbounded_int_difference(chiffre3, chiffre4),
                  difference13 = unbounded_int_difference(chiffre4, chiffre1),
                  difference14 = unbounded_int_difference(chiffre4, chiffre2),
                  difference15 = unbounded_int_difference(chiffre4, chiffre3),
                  difference16 = unbounded_int_difference(chiffre4, chiffre4);
    char *tmp_difference1 = unbounded_int2string(difference1),
         *tmp_difference2 = unbounded_int2string(difference2),
         *tmp_difference3 = unbounded_int2string(difference3),
         *tmp_difference4 = unbounded_int2string(difference4),
         *tmp_difference5 = unbounded_int2string(difference5),
         *tmp_difference6 = unbounded_int2string(difference6),
         *tmp_difference7 = unbounded_int2string(difference7),
         *tmp_difference8 = unbounded_int2string(difference8),
         *tmp_difference9 = unbounded_int2string(difference9),
         *tmp_difference10 = unbounded_int2string(difference10),
         *tmp_difference11 = unbounded_int2string(difference11),
         *tmp_difference12 = unbounded_int2string(difference12),
         *tmp_difference13 = unbounded_int2string(difference13),
         *tmp_difference14 = unbounded_int2string(difference14),
         *tmp_difference15 = unbounded_int2string(difference15),
         *tmp_difference16 = unbounded_int2string(difference16);

    printf("Différence 1 : %s - %s = %s\n", tmp1, tmp1, tmp_difference1);
    printf("Différence 2 : %s - %s = %s\n", tmp1, tmp2, tmp_difference2);   
    printf("Différence 3 : %s - %s = %s\n", tmp1, tmp3, tmp_difference3);
    printf("Différence 4 : %s - %s = %s\n", tmp1, tmp4, tmp_difference4);
    printf("Différence 5 : %s - %s = %s\n", tmp2, tmp1, tmp_difference5);
    printf("Différence 6 : %s - %s = %s\n", tmp2, tmp2, tmp_difference6);   
    printf("Différence 7 : %s - %s = %s\n", tmp2, tmp3, tmp_difference7);
    printf("Différence 8 : %s - %s = %s\n", tmp2, tmp4, tmp_difference8); 
    printf("Différence 9 : %s - %s = %s\n", tmp3, tmp1, tmp_difference9);
    printf("Différence 10 : %s - %s = %s\n", tmp3, tmp2, tmp_difference10);   
    printf("Différence 11 : %s - %s = %s\n", tmp3, tmp3, tmp_difference11);
    printf("Différence 12 : %s - %s = %s\n", tmp3, tmp4, tmp_difference12); 
    printf("Différence 13 : %s - %s = %s\n", tmp4, tmp1, tmp_difference13);
    printf("Différence 14 : %s - %s = %s\n", tmp4, tmp2, tmp_difference14);   
    printf("Différence 15 : %s - %s = %s\n", tmp4, tmp3, tmp_difference15);
    printf("Différence 16 : %s - %s = %s\n", tmp4, tmp4, tmp_difference16);    

    printf("\n"); 

    unbounded_int produit1 = unbounded_int_produit(chiffre1, chiffre1),
                  produit2 = unbounded_int_produit(chiffre1, chiffre2),
                  produit3 = unbounded_int_produit(chiffre1, chiffre3),
                  produit4 = unbounded_int_produit(chiffre1, chiffre4),
                  produit5 = unbounded_int_produit(chiffre2, chiffre1),
                  produit6 = unbounded_int_produit(chiffre2, chiffre2),
                  produit7 = unbounded_int_produit(chiffre2, chiffre3), 
                  produit8 = unbounded_int_produit(chiffre2, chiffre4),
                  produit9 = unbounded_int_produit(chiffre3, chiffre1),
                  produit10 = unbounded_int_produit(chiffre3, chiffre2), 
                  produit11 = unbounded_int_produit(chiffre3, chiffre3),
                  produit12 = unbounded_int_produit(chiffre3, chiffre4),
                  produit13 = unbounded_int_produit(chiffre4, chiffre1),
                  produit14 = unbounded_int_produit(chiffre4, chiffre2),
                  produit15 = unbounded_int_produit(chiffre4, chiffre3),
                  produit16 = unbounded_int_produit(chiffre4, chiffre4);
    char *tmp_produit1 = unbounded_int2string(produit1),
         *tmp_produit2 = unbounded_int2string(produit2),
         *tmp_produit3 = unbounded_int2string(produit3),
         *tmp_produit4 = unbounded_int2string(produit4),
         *tmp_produit5 = unbounded_int2string(produit5),
         *tmp_produit6 = unbounded_int2string(produit6),
         *tmp_produit7 = unbounded_int2string(produit7),
         *tmp_produit8 = unbounded_int2string(produit8),
         *tmp_produit9 = unbounded_int2string(produit9),
         *tmp_produit10 = unbounded_int2string(produit10),
         *tmp_produit11 = unbounded_int2string(produit11),
         *tmp_produit12 = unbounded_int2string(produit12),
         *tmp_produit13 = unbounded_int2string(produit13),
         *tmp_produit14 = unbounded_int2string(produit14),
         *tmp_produit15 = unbounded_int2string(produit15),
         *tmp_produit16 = unbounded_int2string(produit16);

    printf("Produit 1 : %s * %s = %s\n", tmp1, tmp1, tmp_produit1);
    printf("Produit 2 : %s * %s = %s\n", tmp1, tmp2, tmp_produit2);   
    printf("Produit 3 : %s * %s = %s\n", tmp1, tmp3, tmp_produit3);
    printf("Produit 4 : %s * %s = %s\n", tmp1, tmp4, tmp_produit4);
    printf("Produit 5 : %s * %s = %s\n", tmp2, tmp1, tmp_produit5);
    printf("Produit 6 : %s * %s = %s\n", tmp2, tmp2, tmp_produit6);   
    printf("Produit 7 : %s * %s = %s\n", tmp2, tmp3, tmp_produit7);
    printf("Produit 8 : %s * %s = %s\n", tmp2, tmp4, tmp_produit8); 
    printf("Produit 9 : %s * %s = %s\n", tmp3, tmp1, tmp_produit9);
    printf("Produit 10 : %s * %s = %s\n", tmp3, tmp2, tmp_produit10);   
    printf("Produit 11 : %s * %s = %s\n", tmp3, tmp3, tmp_produit11);
    printf("Produit 12 : %s * %s = %s\n", tmp3, tmp4, tmp_produit12); 
    printf("Produit 13 : %s * %s = %s\n", tmp4, tmp1, tmp_produit13);
    printf("Produit 14 : %s * %s = %s\n", tmp4, tmp2, tmp_produit14);   
    printf("Produit 15 : %s * %s = %s\n", tmp4, tmp3, tmp_produit15);
    printf("Produit 16 : %s * %s = %s\n", tmp4, tmp4, tmp_produit16);    

    printf("\n"); 

}

