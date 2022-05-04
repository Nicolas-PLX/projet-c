#ifndef CHIFFRE_H
#define CHIFFRE_H

typedef struct chiffre {
    struct chiffre  *suivant;
    char c;
    struct chiffre *precedent;
} chiffre;

chiffre *chiffre_creer(chiffre *suivant, char c, chiffre *precedent);
chiffre *chiffre_creer_char(char c);

#endif