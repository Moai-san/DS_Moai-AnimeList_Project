#include <stdio.h>
#include <stdlib.h>
#include "hashtable.h"
#include "treemap.h"
#include "login.h"
#include "functions.c"
#include "menu.c"

int main ()
{
    char input ='\n'; //aqui se guardara la eleccion del usuario (registrarse o logearse)
    char* username =((char*)calloc(20,(sizeof(char)))); //aqui se guardara el nombre de usuario
    while((atol(&input)!=(1))&&(atol(&input)!=(2))) //mientras que el caracter ingresado no sea ni 1 ni 2
    {
        printf("Desea registrarse? Ingrese 1\nTiene Cuenta? Ingrese 2\n");
        scanf("%c",&input);//se lee la opcion ingresada por el usuario
        if((input!='1')&&(input!='2'))//si la opcion ingresada no es valida, entonces
        {
            while (input!='\n') //en caso de que el usuario no haga caso, y en vez de ingresar un caracter, ingrese una cadena, este while vaciara el bufer
            {
                scanf("%c",&input); //se leen los caracteres hasta llegar a un salto de linea
            }
        }
    }
    if ((atol(&input))==1)//si la opcion ingresada fue un 1 significa que el usuario se registrará
    {
        username =sign_up();//se procede a registrar el usuario
    }
    username =log_me();//se procede a iniciar sesion y se guarda el username para importar su lista
    init_var();//se inicializan las variables globales
    importCatalogue(); //se importa el catalogo (base de datos en formato csv)
    importLists(username);//se importan las listas del usuario a las estructuras de datos de este
    importGlobal_lists();//se importan los tops a las estructuras de datos publicas
    clear_Screen();//se limpia la pantalla
    menu();//se inicia el menú
    export_userData(username);//se exportan los datos de usuario para finalizar la ejecucion
    //export_top(1);
    //export_top(2);
    return(0);//se finaliza la ejecucion
}