#include <stdio.h>
#include "list.h"

/*System Condition, only if it's Compiled in windows, includes the library*/
#ifdef _WIN32
    #include <windows.h>
#endif

/*Cleans The Screen (yup, i know that is obvious, but i have to comment that)*/
void clear_Screen()
{
    #ifdef _WIN32
    {
        system("cls");
    }
    #else
    {
        system("reset");
    }
    #endif
}

/*Menu (yup, i know that is obvious, but i have to comment that)*/
int menu ()
{
    printf("Bienvenid@ a Moai-AnimeList! OwO\nQue te gustaria hacer hoy?(\'w\')\na)Ver el catalogo de anime\nb)ingresar anime a lista\nc)Ver lista\nd)Ver los Tops\ne)Limpiar la pantalla\nSi no quieres nada, solo presiona la tecla enter uwu\n");
    char option;
    short int flag;
    flag =(-1);
    char garbageBin;//papelera para la basura que se situa en stdin
    scanf("%c",&garbageBin);
    do
    {
        if (flag==(1))
        {
            printf("Te gustaria Hacer algo Mas?(\'w\')\na)Ver el catalogo de anime\nb)ingresar anime a lista\nc)Ver lista\nd)Ver los Tops\ne)Limpiar la pantalla\nSi no quieres nada, solo presiona la tecla enter uwu\n");
        }
        scanf("%c",&option);
            switch (option)
            {
                case 'a': //caso a, Ver el catalogo de anime
                {
                    print_Catalogue();
                    break;
                }
                case 'b': //caso b, Ingresar anime a lista
                {
                    anime* toInsert;
                    int filter_option;
                    printf("A que lista le gustaria ingresar un anime?\n1)Lista de anime visto\n2)Lista de anime favorito\n3)Lista de anime odiado\n");
                    scanf("%d",&filter_option);
                    while(1)//bucle en el cual se valida el valor ingresado
                    {
                        switch (filter_option)//switch donde se entra a funcion segun valor ingresado
                        {
                            case 1://si se ingresó un 1, se ingresa a la lista de vistos
                            {
                                printf("Le gustaria aplicar algun filtro de busqueda?, a continuacion se mostraran los disponibles\n1)Ver solo anime de \'x\' Año\n2)Ver solo anime de \'x\' Tipo\n3)Ver solo anime de \'x\' Genero\n4)Ver catalogo sin filtrar\n");
                                scanf("%d",&filter_option);
                                while (1) //bucle para validacion de valor ingresado
                                {
                                    switch (filter_option)//switch que define el filtro de busqueda segun opcion seleccionada por usuario
                                    {
                                        case 1://filtrado por año
                                        {
                                            int year;
                                            printf("Ingrese el año a buscar!!!\n");
                                            scanf("%d",&year);
                                            clear_Screen();
                                            toInsert =filteredSearch(&year,filter_option);
                                            break;
                                        }
                                        case 2://filtrado por tipo
                                        {
                                            char type[11];
                                            printf("Ingrese el tipo a buscar!!!\n");
                                            scanf("%c",&garbageBin);
                                            scanf("%s",type);
                                            clear_Screen();
                                            toInsert =filteredSearch(type,filter_option);
                                            break;
                                        }
                                        case 3://filtrado por genero
                                        {
                                            char genre[150];
                                            printf("Ingrese el genero a buscar!!!\n");
                                            scanf("%c",&garbageBin);
                                            scanf("%s",genre);
                                            clear_Screen();
                                            toInsert =filteredSearch(genre,filter_option);
                                            break;
                                        }
                                        case 4://ingreso sin filtrar
                                        {
                                            clear_Screen();
                                            toInsert =non_filteredSearch();
                                            break;
                                        }
                                        default://en caso de ingresarse un valor distinto a las opciones dadas
                                            printf("Entrada invalida!\nIngrese Opcion nuevamente\n");
                                            scanf("%d",&filter_option);
                                            continue;
                                    }
                                    break;
                                }
                                filter_option=1;//una vez almacenado el anime a ingresar a la lista de vistos, se vuelve el valor del filtro a 1 (el 1 indica la lista de vistos)
                                if (toInsert!=NULL)//si realmente se almacenó un valor en la variable (de tipo anime), esta se ingresa a la lista de vistos
                                {
                                    addTo_list(filter_option,toInsert);
                                }
                                else//sinó, se muestra mensaje en pantalla y se vuelve al menú
                                {
                                    printf("No seleccionaste anime asi que no se ingresó nada! :C\n");
                                }
                                break;
                            }
                            case 2://si se ingresó la opcion 2, se accede a la lista de favoritos
                            {
                                toInsert =searchFrom_list();//se guarda la salida de la funcion, la cual es el anime a ingresar
                                filter_option=2;//se devuelve el filtro al valor 2, ya que este indica la lista de favoritos
                                if (toInsert!=NULL)//si realmente se almacenó un valor, este se ingresa
                                {
                                    addTo_list(filter_option,toInsert);//a lista de favoritos
                                    addTo_list(4,toInsert);//a lista de tops (esta se modifica con cada anime que se ingresa a favoritos)
                                }
                                else//sinó, se muestra mensaje
                                {
                                    printf("No seleccionaste anime asi que no se ingresó nada! :C\n");
                                }
                                break;
                            }
                            case 3://si se ingresó la opcion 3, se accede a la lista de odiados
                            {
                                toInsert =searchFrom_list();//se guarda la salida de la funcion, la cual es el anime a ingresar
                                filter_option=3;//se devuelve el filtro al valor 3, ya que este indica la lista de odiados
                                if (toInsert!=NULL)//si realmente se almacenó un valor, este se ingresa
                                {
                                    addTo_list(filter_option,toInsert);//a lista de odiados
                                    addTo_list(5,toInsert);//a lista de tops (esta se modifica con cada anime que se ingresa a odiados)
                                }
                                else//sinó, se muestra mensaje
                                {
                                    printf("No seleccionaste anime asi que no se ingresó nada! :C\n");
                                }
                                break;
                            }
                            default://en caso de ingresarse un valor distinto a las opciones dadas se solicita ingresar una opcion nuevamente
                                printf("OPCION INVALIDA, INTENTE NUEVAMENTE\n");
                                scanf("%d",&filter_option);
                                continue;
                        }
                        break;
                    }
                    //clear_Screen();
                    break;
                }
                case 'c': //caso c, Ver lista
                {
                    int option;
                    printf("Que lista desea ver?\n1)lista de anime visto\n2)lista de anime favorito\n3)lista de anime odiado\n");
                    scanf("%d",&option);//se solicita al usuario definir que lista quiere ver
                    while (1)//bucle para validacion
                    {
                        switch (option)//switch para la opcion ingresada
                        {
                            case 1://se imprime lista de vistos
                            {
                                clear_Screen();
                                print_list(1);
                                break;
                            }
                            case 2://se imprime lista de favoritos
                            {
                                clear_Screen();
                                print_list(2);
                                break;
                            }   
                            case 3://se imprime lista de odiados
                            {
                                clear_Screen();
                                print_list(3);
                                break;
                            }
                            default://si la opcion es invalida, se solicita ingresar opcion nuevamente
                            {
                                clear_Screen();
                                printf("Entrada invalida!\nIngrese Opcion nuevamente\n");
                                scanf("%d",&option);
                                continue;
                            }
                        }
                        break;
                    }
                    break;
                }
                case 'd': //caso d, recomendaciones
                {
                    char option;
                    scanf("%c",&option);//limpieza de buffer
                    printf("Que top desea ver?\na)Top Favoritos\nb)Top Odiados\n");
                    scanf("%c",&option);//se solicita especificar que top quiere ver
                    while (1)
                    {
                        switch (option)
                        {
                            case 'a'://top favoritos
                            {
                                clear_Screen();
                                top_loved();
                                break;
                            }
                            case 'b'://top odiados
                            {
                                clear_Screen();
                                top_hated();
                                break;
                            }
                            default://entrada invalida
                                printf("Entrada Invalida!!!\nIngrese nuevamente\n");
                                scanf("%c",&option);
                                continue;
                        }
                        break;
                    }
                    break;
                }
                case 'e': //caso e, limpiar pantalla
                {
                    clear_Screen();
                    break;
                }
                case '\n': //caso enter, cambia el flag
                {
                    flag =(0);
                    break;
                }
                default: //caso cualquier otra tecla (excepto la ñ, que no la toma como letra, me parece que por el numero en la tabla ascii)
                    clear_Screen();
                    printf("Entrada invalida!");
                    break;
            }
            if (flag!=0)
            {
                scanf("%c",&garbageBin);//llamada a la papelera para la basura que se situa en stdin
                flag =(1);
            }
    }
    while (flag!=(0)); //si el flag es 0 (apretaron enter), se corta el while y cierra el programa
    return(0);
}
