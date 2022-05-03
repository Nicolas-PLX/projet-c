#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "chiffre.h"

chiffre *chiffre_creer(chiffre *suivant, char c, chiffre *precedent) {
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

int main() {

}