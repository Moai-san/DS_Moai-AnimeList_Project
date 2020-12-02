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
    printf("Bienvenid@ a Moai-AnimeList! OwO\nQue te gustaria hacer hoy?(\'w\')\na)Ver el catalogo de anime\nb)ingresar anime a lista\nc)Ver lista\nd)Limpiar la pantalla\nSi no quieres nada, solo presiona la tecla enter uwu\n");
    char option;
    short int flag;
    flag =(-1);
    char garbageBin;//papelera para la basura que se situa en stdin
    scanf("%c",&garbageBin);
    do
    {
        if (flag==(1))
        {
            printf("Te gustaria Hacer algo Mas?(\'w\')\na)Ver el catalogo de anime\nb)ingresar anime a lista\nc)Ver lista\nd)Limpiar la pantalla\nSi no quieres nada, solo presiona la tecla enter uwu\n");
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
                    if (filter_option==1)
                    {
                        printf("Le gustaria aplicar algun filtro de busqueda?, a continuacion se mostraran los disponibles\n1)Ver solo anime de \'x\' Año\n2)Ver solo anime de \'x\' Tipo\n3)Ver solo anime de \'x\' Genero\n4)Ver catalogo sin filtrar\n");
                        scanf("%d",&filter_option);
                        while (1)
                        {
                            switch (filter_option)
                            {
                                case 1:
                                {
                                    int year;
                                    printf("Ingrese el año a buscar!!!\n");
                                    scanf("%d",&year);
                                    clear_Screen();
                                    toInsert =filteredSearch(&year,filter_option);
                                    break;
                                }
                                case 2:
                                {
                                    char type[11];
                                    printf("Ingrese el tipo a buscar!!!\n");
                                    scanf("%c",&garbageBin);
                                    scanf("%s",type);
                                    clear_Screen();
                                    toInsert =filteredSearch(type,filter_option);
                                    break;
                                }
                                case 3:
                                {
                                    char genre[150];
                                    printf("Ingrese el genero a buscar!!!\n");
                                    scanf("%c",&garbageBin);
                                    scanf("%s",genre);
                                    clear_Screen();
                                    toInsert =filteredSearch(genre,filter_option);
                                    break;
                                }
                                case 4:
                                {
                                    clear_Screen();
                                    non_filteredSearch();
                                    break;
                                }
                                default:
                                {
                                    printf("Entrada invalida!\nIngrese Opcion nuevamente\n");
                                    scanf("%d",&filter_option);
                                    continue;
                                }
                            }
                            break;
                        }
                        filter_option=1;
                        addTo_list(filter_option,toInsert);
                    }
                    if (filter_option==2)
                    {
                        toInsert =searchFrom_list();
                        filter_option=2;
                        addTo_list(filter_option,toInsert);
                    }
                    if (filter_option==3)
                    {
                        toInsert =searchFrom_list();
                        filter_option=3;
                        addTo_list(filter_option,toInsert);
                    }
                    //clear_Screen();
                    break;
                }
                case 'c': //caso c, Ver lista
                {
                    int option;
                    printf("Que lista desea ver?\n1)lista de anime visto\n2)lista de anime favorito\n3)lista de anime odiado\n");
                    scanf("%d",&option);
                    while (1)
                    {
                        switch (option)
                        {
                            case 1:
                            {
                                clear_Screen();
                                print_list(1);
                                break;
                            }
                            case 3:
                            {
                                clear_Screen();
                                print_list(2);
                                break;
                            }   
                            case 2:
                            {
                                clear_Screen();
                                print_list(3);
                                break;
                            }
                            default:
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
                case 'd': //caso d, limpiar pantalla
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
