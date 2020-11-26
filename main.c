#include <stdio.h>
#include <stdlib.h>
#include "hashtable.h"
#include "treemap.h"
#include "login.h"
#include "functions.c"
//#include "menu.c"

int main ()
{
    short int input =(0);
    /*char* username =((char*)calloc(20,(sizeof(char))));
    while((input!=(1))&&(input!=(2)))
    {
        printf("Desea registrarse? Ingrese 1\nTiene Cuenta? Ingrese 2\n");
        scanf("%hd",&input);
    }
    if (input==1)
    {
        username =sign_up();
    }
    username =log_me();*/
    init_var();
    importCatalogue();
    print_Catalogue();
    return(0);
}