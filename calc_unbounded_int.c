#include "unbounded_int.c"

#include <stdio.h>
#include <stdlib.h>



//Fonction boolean qui regarde si l'option i est présente dans les arguments du main.
//Renvoie 1 si elle y est, sinon 0
int option_i(char *argv[]){
    int i = 0;
    while(argv[i] != "\0"){
        if (argv[i] == "-i"){
            return 1;
        }
    }
    return 0;
}
//Fonction boolean qui regarde si l'option o est presente dans lesa rguments du main.
//Renvoie 1 si elle y est, sinon 0
int option_o(char *argv[]){
    int i = 0;
    while(argv[i] != "\0"){
        if (argv[i] == "-o"){
            return 1;
        }
    }
    return 0;
}








void main(int argc, char *argv[]){
    int bool_i = option_i(argv);
    int bool_o = option_o(argv);

}