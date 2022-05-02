#include "unbounded_int.c"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

static variable *new_var(char *nom, char *valeur){
    unbounded_int val = string2unbounded_int(valeur);
    if(val->signe != '*'){
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

static void insertion(liste_variable *liste, variable *a){
    element *nouveau = (element *)malloc(sizeof(element));
    if (liste == NULL || nouveau == NULL){
        exit(EXIT_FAILURE);
    }
    nouveau->var = a;

    nouveau->suivant = liste->premier;
    liste->premier = nouveau;
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
            return tmp->var->valeur;
        }
        tmp = tmp->suivant;
    }
    return NULL;
}

//Fonction boolean qui regarde si l'option i est présente dans les arguments du main.
//Renvoie 1 si elle y est, sinon 0
static int option_i(char **argv){
    int i = 0;
    char *nul = "\0";
    while(strcmp(argv[i], nul) != 0){
        char *char_i = "-i";
        if (strcmp(argv[i], char_i) == 0){
            return 1;
        }
        i++;
    }
    return 0;
}
//Fonction boolean qui regarde si l'option o est presente dans les arguments du main.
//Renvoie 1 si elle y est, sinon 0
static int option_o(char **argv){
    int i = 0;
    char *nul = "\0";
    while(strcmp(argv[i], nul) != 0){
        char *char_o = "-o";
        if (strcmp(argv[i], char_o) == 0){
            return 1;
        }
        i++;
    }
    return 0;
}
//Fonction qui verifie si la chaine de caractere en parametre est valable en tant que variable, c'est a dire que la chaine est remplis uniquement de charactere a-z
//retourne 1 si la chaine est valide, 0 sinon.
static int arg_valable(char *mot){
    int n = strlen(mot);
    int i = 0;
    int val_ascii = 0;
    while(mot[i] != "\0"){
        val_ascii = (int)mot[i];
        if (val_ascii < 97 || val_ascii > 122){
            return 0;
        }
        i++;
    }
    return 1;
}

//Fonction qui verifie que la chaine est bien un nombre, en verifiant les characteres ASCII
static int nombre_valable(char *nombre){
    int n = strlen(nombre);
    int i = 0;
    int val_ascii = 0;
    while(mot[i] != "\0"){
        val_ascii = (int)mot[i];
        if(val_ascii < 48 || val_ascii > 57){
            return 0;
        }
        i++;
    }
    return 1;

}
/*Fonction qui retourne l'index du tableau de charactere ou se trouve le fichier de l'option o.
* Le fichier de l'option o se trouve juste après avoir rentré "-o", donc a (l'index de l'option o) + 1;
* Retourne 0 si le fichier n'est pas trouvé.
*/

static int index_option_o(char **argv){
    int i = 0;
    while(strcmp(argv[i],"\0") != 0){
        char *char_o = "-o";
        if(strcmp(argv[i], char_o) == 0){
            i++;
            return i;
        }
        i++;
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

static void calcul(variable *res, variable *a, variable *b, char *operation ){
    //TODO : vérifier que res existe
    if (strcmp(operation,"+") == 0){
        // unbounded_int calc = unbounded_int_somme(a->valeur,b->valeur);
        // res-> valeur = calc;
    }
    if (strcmp(operation,"/") == 0){
        // unbounded_int calc = unbounded_int_quotient(a->valeur,b->valeur);
        // res-> valeur = calc;
    }
    if (strcmp(operation,"-") == 0){
        // unbounded_int calc = unbounded_int_difference(a->valeur,b->valeur);
        // res-> valeur = calc;
    }
    if (strcmp(operation,"*") == 0){
        // unbounded_int calc = unbounded_int_produit(a->valeur,b->valeur);
        // res-> valeur = calc;
    }
}
/*Fonction qui permet d'afficher la valeur de la variable correspondant a la string "nom".
*@params nom la variable portant cette string, "boolean" qui indique si l'option o a été mise,
*"liste" qui va nous permettre de recuperer la variable si elle existe,
*et "file" le chemin vers le fichier où on souhaite ecrire (si on en a besoin)
*/

static void print_var(char *nom, int boolean, char *file, liste_variable *liste){
    variable *var = get_variable(liste,nom);
    char buffer[LEN];
    strcpy(buffer,nom); strcat(buffer," = ");
    if (var != NULL){
        strcat(buffer,unbounded_int2string(var->valeur));
    } else {
        strcat(buffer,"0");
    }
    if (boolean == 1){
        FILE *outputFile = fopen(file,"a+");
        fputs(buffer,outputFile);
        fclose(outputFile);
    } else {
        printf("%s",buffer);
    }
}

void lecture(char **argv){
    int bool_i = option_i(argv);
    int bool_o = option_o(argv);
    char *buffer = (char *)malloc(LEN); //TODO : verifier s'il faut un tableau ou non
    liste_variable *l_var = initialisation();

    //Cas ou l'option i est utilise :
    if (bool_i == 1){
        FILE *inputFile = fopen(argv[1],"r+");
        if (inputFile == NULL){
            printf("Ne peut pas ouvrir le fichier %s\n", argv[1]);
            exit(EXIT_FAILURE);
        }
        //On lis le fichier
        while (feof(inputFile)){
            fgets(buffer,LEN,inputFile);

            if (ferror(inputFile)){
                fprintf(stderr, "Erreur %d\n", errno);
                break;
            }
            //On separe la phrase en plusieurs segment pour l'analyser plus facilement, qu'on stockera dans un tableau de "string"
            const char *separateurs = " =";
            int compteur = 0; 
            char *tableau_sep[4]; // Taille 4 parce que pour nos instructions, aucune ne peut depasser 4.
            char *ligne_sep = strtok(buffer,separateurs);
            while (ligne_sep != NULL){
                tableau_sep[compteur] = ligne_sep;
                compteur++;
                ligne_sep = strtok(NULL, separateurs);
            }
            //Enfin, on regarde cas par cas, des 3 instructions possible. on récupère d'abord le premier mot pour voir dans quel cas on se situe
            char *decoupage = tableau_sep[0];
            if (strcmp(decoupage,"print")){
               int index_file = index_option_o(argv);
               print_var(tableau_sep[1],bool_o,argv[index_file],l_var);

             } else {

                if(arg_valable(decoupage) == 1){
                    if(tableau_sep[2] == NULL){ //Cas ou on attribut seulement une valeur a une variable : en effet, si la 3eme case du tableau est vide, alors nous n'avons pas d'operateur, il reste donc que le cas ou nous devons attribuer une variable.
                      //On regarde ensuite si la variable existe deja. Si c'est le cas, alors on change juste la valeur de la variable, sinon on l'initialise. 
                       
                        variable *var = get_variable(l_var,tableau_sep[0]);
                        if (var != NULL){
                            unbounded_int valeur = string2unbounded_int(tableau_sep[1]);
                            var->valeur = valeur;
                        } else {
                            variable *nv_var = new_var(tableau_sep[0],tableau_sep[1]);
                            insertion(l_var,nv_var);
                            free(var); //On libere l'espace pris pour l'ancienne var
                        }
                    } else { //Cas ou nous avons une operation
                    //TODO : regarder si tab[1] et tab[3] sont des chiffres (fonction nombre_valable)
                    // si c'est le cas, on appelle directement la fonction de calcul
                    //sinon, on vérifie si ce sont des variables valables
                    //si c'est le cas, on appelle la fonction de calcul, sinon on stop le processus

                    }
                } else {
                    printf("Variable non conforme");
                    exit(EXIT_FAILURE);
                }
             }


            //Maintenant on analyse chaque element
        }



    }
    free(buffer);    
}






int main(int argc, char **argv){
    //int bool_i = option_i(argv);
    //int bool_o = option_o(argv);
    //lecture(argv);

    
    const char * separateurs = " ";
    char str[] = "";
    char *tableau_sep[4];
    int compteur = 0;
    char *ligne_sep = strtok(str,separateurs);
        while (ligne_sep != NULL){
            //printf("%s\n", ligne_sep);
            tableau_sep[compteur] = ligne_sep;
            printf("%s \n", tableau_sep[compteur]);
            compteur++;
            ligne_sep = strtok(NULL, separateurs);
        }
    printf("toui");
    return 0;
    
   char *string = " ";
   char a[] = "a";
   char *egale = " = ";
   char *valeur = "132";
   strncat(string,a,10);
   printf("%s\n",string);
} 