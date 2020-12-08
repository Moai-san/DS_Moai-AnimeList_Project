#include <stdio.h>
#include <stdlib.h>
#include "hashtable.h"
#include "treemap.h"
#include "login.h"
#include "functions.c"
#include "menu.c"

int main ()
{
    char input ='\n';
    char* username =((char*)calloc(20,(sizeof(char))));
    anime* aux;
    while((atol(&input)!=(1))&&(atol(&input)!=(2)))
    {
        printf("Desea registrarse? Ingrese 1\nTiene Cuenta? Ingrese 2\n");
        scanf("%c",&input);
        if((input!='1')&&(input!='2'))
        {
            while (input!='\n')
            {
                scanf("%c",&input);
            }
        }
    }
    if ((atol(&input))==1)
    {
        username =sign_up();
    }
    username =log_me();
    init_var();
    importCatalogue();
    importLists(username);
    importGlobal_lists();
    clear_Screen();
    menu();
    export_userData(username);
    export_top(1);
    export_top(2);
    return(0);
}