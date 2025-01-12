#include "unbounded_int.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define LEN 1024

//Structure permettant de stocker les variables que nous lirons.
typedef struct variable{
    char *nom;
    unbounded_int valeur;
}variable;

//Creation d'une liste pour stocker les variables du programme.
typedef struct element{
    variable *var;
    struct element *suivant;

}element;

typedef struct liste_variable{
    element *premier;
}liste_variable;

/*Fonction qui va creer une nouvelle variable.
*@params nom pour le nom de la variable, valeur pour sa valeur.
*retourne la nouvelle variable.
*/
static variable *new_var(char *nom, char *valeur){
    unbounded_int val = string2unbounded_int(valeur);
    if(val.signe != '*') {
        variable *var = malloc(sizeof(variable));    
        if (var == NULL){
            exit(EXIT_FAILURE);
        }
        var->nom = nom;
        var->valeur = val;
        return var;
    } else {
        exit(EXIT_FAILURE);
    }
}
//Fonction qui initialise une nouvelle liste.
static liste_variable *initialisation(){
    liste_variable *liste = (liste_variable*)malloc(sizeof(liste_variable));
    element *elem = (element*)malloc(sizeof(element));

    if (liste == NULL || elem == NULL){
        printf("erreur a la creation de la liste");
        exit(EXIT_FAILURE);
    }

    elem -> var = NULL;
    elem->suivant = NULL;
    liste->premier = elem;

    return liste;
}

/* Fonction qui cherche la variable qui porte le nom "nom";
*  retourne la variable associe, sinon NULL
*/
static variable *get_variable(liste_variable *liste, char *nom){
    if (liste == NULL || nom == NULL){
        exit(EXIT_FAILURE);
    }

    element *tmp = liste->premier;
    while (tmp != NULL){
        if (tmp->var == NULL){ 
            return NULL;
        }
        if (strcmp(tmp->var->nom, nom) == 0){
            return tmp->var;
        }
        tmp = tmp->suivant;
    }
    return NULL;
}

/*Fonction qui permet d'ajouter une variable dans une liste.
*Nous devons neanmoins verifier que la variable n'est pas deja dans la liste.
* renvoie 1 si l'insertion a pu se faire, 0 sinon
*/
static int insertion(liste_variable *liste, variable *a){
    variable *verif = get_variable(liste,a->nom);
    if (verif != NULL){
        free(verif);
        return 0;
    }
    element *nouveau = (element *)malloc(sizeof(element));
    if (liste == NULL || nouveau == NULL){
        exit(EXIT_FAILURE);
    }
    nouveau->var = a;

    nouveau->suivant = liste->premier;
    liste->premier = nouveau;
    return 1;
}

//Fonction boolean qui regarde si l'option i est présente dans les arguments du main.
//Renvoie 1 si elle y est, sinon 0
static int option_i(int argc, char **argv){
    int i = 0;
    while(strcmp(argv[i],"\0") != 0){
        char *char_i = "-i";
        if (strcmp(argv[i], char_i) == 0){
            return 1;
        }
        i++;
        if (argc == i){
            return 0;
        }
    }
    return 0;
}
//Fonction boolean qui regarde si l'option o est presente dans les arguments du main.
//Renvoie 1 si elle y est, sinon 0
static int option_o(int argc, char **argv){
    int i = 0;
    while(strcmp(argv[i],"\0") != 0){
        char *char_o = "-o";
        if (strcmp(argv[i], char_o) == 0){
            return 1;
        }
        i++;
        if (argc == i){
            return 0;
        }
    }
    return 0;
}
//Fonction qui verifie si la chaine de caractere en parametre est valable en tant que variable, c'est a dire que la chaine est remplis uniquement de charactere a-z
//retourne 1 si la chaine est valide, 0 sinon.
static int arg_valable(char *mot){
    int val_ascii = 0;
     for(int i =0; mot[i] != '\0';++i){
        val_ascii = (int)mot[i];
        if (val_ascii < 97 || val_ascii > 122){
            return 0;
        }
    }
    return 1;
}

//Fonction qui verifie que la chaine est bien un nombre, en verifiant les characteres ASCII
//pour nous, un nombre peut aussi avoir "+" et "-" au debut de son nom, pour le signe du nombre.
static int nombre_valable(char *nombre){
    int val_ascii = 0;
    for(int i =0; nombre[i] != '\0';++i){
        val_ascii = (int)nombre[i];
        if(val_ascii < 48 || val_ascii > 57){ //Valeur ASCII de 0 et de 9
            if (i == 0){
                if (val_ascii != 43 && val_ascii != 45){ //valeur ASCII du + et -
                    return 0;
                }
            }
        }
    }
    return 1;
}
/*Fonction qui retourne l'index du tableau de charactere ou se trouve le fichier de l'option o.
* Le fichier de l'option o se trouve juste après avoir rentré "-o", donc a (l'index de l'option o) + 1;
* Retourne 0 si le fichier n'est pas trouvé.
*/

static int index_option_o(int argc, char **argv){
    int i = 0;
    while(strcmp(argv[i],"\0") != 0){
        char *char_o = "-o";
        if(strcmp(argv[i], char_o) == 0){
            i++;
            return i;
        }
        i++;
        if (argc == i){
            return 0;
        }
    }
    return 0;
}

/*Fonction qui retourne l'index du tableau de charactere ou se trouve le fichier de l'option i.
* Le fichier de l'option i se trouve juste après avoir rentré "-i", donc a (l'index de l'option i) + 1;
* Retourne 0 si le fichier n'est pas trouvé.
*/
static int index_option_i(char **argv){
    int i = 0;
    while(strcmp(argv[i],"\0") != 0){
        char *char_i = "-i";
        if(strcmp(argv[i], char_i) == 0){
            i++;
            return i;
        }
        i++;
    }
    return 0;
}


/*Fonction qui va effectuer le calcul que l'on trouve dans la lecture.
@param "res" pour le resultat du calcul, "a" pour la premiere variable, "b" pour la seconde variable, et "operation" pour le signe de l'operation effectue.*/

static void calcul(variable *res, char *a, char *b, char *operation){
    unbounded_int int_a = string2unbounded_int(a);
    unbounded_int int_b = string2unbounded_int(b);
    if (strcmp(operation,"+") == 0){
        unbounded_int calc = unbounded_int_somme(int_a,int_b);
        res-> valeur = calc;
    }
    if (strcmp(operation,"/") == 0){
        unbounded_int calc = unbounded_int_quotient(int_a,int_b);
        res-> valeur = calc;
    }
    if (strcmp(operation,"-") == 0){
        unbounded_int calc = unbounded_int_difference(int_a,int_b);
        res-> valeur = calc;
    }
    if (strcmp(operation,"*") == 0){
        unbounded_int calc = unbounded_int_produit(int_a,int_b);
        res-> valeur = calc;
    }
}

/*Fonction qui permet d'afficher la valeur de la variable correspondant a la string "nom".
*@params nom la variable portant cette string, "boolean" qui indique si l'option o a été mise,
*"liste" qui va nous permettre de recuperer la variable si elle existe,
*et "file" le chemin vers le fichier où on souhaite ecrire (si on en a besoin)
*/

static void print_var(char *nom, int boolean, char *file, liste_variable *liste){
    variable *var = get_variable(liste,nom);
    if (var == NULL){
    }
    char buffer[LEN];
    strcpy(buffer,nom); strcat(buffer," = ");
    if (var != NULL){
        char *tmp = unbounded_int2string(var->valeur);
        strcat(buffer,tmp);
        strcat(buffer,"\n");
    } else {
        strcat(buffer,"0\n");
    }
    if (boolean == 1){
        FILE *outputFile = fopen(file,"a+");
        fputs(buffer,outputFile);
        fclose(outputFile);
    } else {
        printf("%s",buffer);
    }
}
//PHASE DE DEBOGUAGE
void lecture(int argc, char **argv){
    int bool_i = option_i(argc,argv);
    int bool_o = option_o(argc,argv);
    char *buffer = (char *)malloc(LEN);
    liste_variable *l_var = initialisation();

    //Cas ou l'option i est utilise :
    FILE *inputFile = stdin;
    if (bool_i == 1){
        int index_i = index_option_i(argv);
        inputFile = fopen(argv[index_i],"r+");
    }
    if (inputFile == NULL){
            printf("Ne peut pas ouvrir le fichier\n");
            exit(EXIT_FAILURE);
    }
    //On lis le fichier
    while (!feof(inputFile)){
        fgets(buffer,LEN,inputFile);
        if (ferror(inputFile)){
            fprintf(stderr, "Erreur %d\n", errno);
            break;
        }
            //On separe la phrase en plusieurs segment pour l'analyser plus facilement, qu'on stockera dans un tableau de "string"
            const char *separateurs = " =\n";
            int compteur = 0; 
            char *tableau_sep[5]; // Taille 5 parce que pour nos instructions, aucune ne peut depasser 5.
            char *ligne_sep = strtok(buffer,separateurs);
            while(ligne_sep != NULL){
                tableau_sep[compteur] = ligne_sep;
                compteur++;
                ligne_sep = strtok(NULL, separateurs);
            }
           
            //Enfin, on regarde cas par cas, des 3 instructions possible. on récupère d'abord le premier mot pour voir dans quel cas on se situe
            char *decoupage = tableau_sep[0];
            if (strcmp(decoupage,"print") == 0){
               int index_file = index_option_o(argc,argv);
               if(strlen(tableau_sep[1]) == 0){
                   printf("Il n'y a rien a afficher.\n");
                   exit(EXIT_FAILURE);
               } else {
                    print_var(tableau_sep[1],bool_o,argv[index_file],l_var);
               }
             } else {
                if(arg_valable(decoupage) == 1){
                    if(strlen(tableau_sep[2]) == 0 || strlen(tableau_sep[2]) > 1){ //Cas ou on attribut seulement une valeur a une variable : en effet, si la 3eme case du tableau est vide, alors nous n'avons pas d'operateur, il reste donc que le cas ou nous devons attribuer une variable.
                      //On regarde ensuite si la variable existe deja. Si c'est le cas, alors on change juste la valeur de la variable, sinon on l'initialise. 
                       
                        variable *var = get_variable(l_var,tableau_sep[0]);
                        if (var != NULL){
                            unbounded_int valeur = string2unbounded_int(tableau_sep[1]);
                            var->valeur = valeur;
                        } else {
                            char *copy = malloc(sizeof(char) * strlen(tableau_sep[0]));
                            strcpy(copy,tableau_sep[0]);
                            variable *nv_var = new_var(copy,tableau_sep[1]);
                            insertion(l_var,nv_var);
                            free(var); //On libere l'espace pris pour l'ancienne var
                        } 
                    } else if (strlen(tableau_sep[2]) == 1){ //Cas ou nous avons une operation
                    //On verifie que l'operation est valide.

                    if(tableau_sep[2][0] != '+' && tableau_sep[2][0] != '/' && tableau_sep[2][0] != '*' && tableau_sep[2][0] != '-'){
                        printf("Calcul invalide");
                        fclose(inputFile);
                        exit(EXIT_FAILURE);
                    }
                    variable *var_res = get_variable(l_var,tableau_sep[0]);
                    if (var_res == NULL){
                        char *copy = malloc(sizeof(char) * strlen(tableau_sep[0]));
                            strcpy(copy,tableau_sep[0]);
                            variable *nv_var = new_var(copy,"0");
                            insertion(l_var,nv_var);
                            var_res = nv_var;
                    }
                    //On regarde ensuite le cas ou nous avons 2 variables (ex : " a * a")
                    if(arg_valable(tableau_sep[1]) == 1 && arg_valable(tableau_sep[3]) == 1){
                        variable *var_a = get_variable(l_var,tableau_sep[1]);
                        variable *var_b = get_variable(l_var,tableau_sep[3]);
                        calcul(var_res,unbounded_int2string(var_a->valeur),unbounded_int2string(var_b->valeur),tableau_sep[2]);
                    } else {
                        //On regarde ensuite le cas ou les deux sont des nombres (ex : "3 + 5")
                        if(nombre_valable(tableau_sep[1]) == 1 && nombre_valable(tableau_sep[3]) == 1){
                            calcul(var_res,tableau_sep[1],tableau_sep[3],tableau_sep[2]);    
                        } else {
                        //On regarde ensuite le cas ou le premier argument est une variable, et le deuxieme un nombre (ex : "a * 3")
                            if(arg_valable(tableau_sep[1]) == 1 && nombre_valable(tableau_sep[3]) == 1){
                                variable *var_a = get_variable(l_var,tableau_sep[1]);
                                calcul(var_res,unbounded_int2string(var_a->valeur),tableau_sep[3],tableau_sep[2]);
                            } else {
                                //enfin, on regarde si le premier argument est un nombre et le deuxieme une variable (ex : "3 * a")
                                if(nombre_valable(tableau_sep[1]) == 1 && arg_valable(tableau_sep[3]) == 1){ 
                                    variable *var_b = get_variable(l_var,tableau_sep[3]);
                                    calcul(var_res,tableau_sep[1],unbounded_int2string(var_b->valeur),tableau_sep[2]);
                                } else {
                                    printf("Calcul invalide");
                                    fclose(inputFile);
                                    exit(EXIT_FAILURE);
                                }
                            }
                        }
                    }
                }
            } else {
                    printf("Variable non conforme");
                    exit(EXIT_FAILURE);
                }
             }
        }
    fclose(inputFile);
    free(buffer);
    free(l_var);    
}






int main(int argc, char **argv){   
   lecture(argc,argv);
} 
