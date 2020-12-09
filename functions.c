#include <stdio.h>
#include <stdlib.h>
#include "hashtable.h"
#include "treemap.h"
#include "list.h"
#include "lectura_csv.c"
#include "mymenu.h"
#include <dirent.h>

typedef struct anime anime;

struct anime
{
   long mal_id; //myanimelist_id
   char name[110]; //nombre
   char type[11]; //tipo de anime (movie,tv,web-series,special,ova)
   int episodes; //cantidad de episodios
   char duration[20]; //duracion
   char rating[20]; //rating (calificacion de contenido)
   int year; //año lanzamiento
   char* studio; //estudio animacion
   char* genre; //genero anime
};

//variables globales
List* catalogue; //catalogo
List* faved;//lista de favoritos del usuario logueado
List* hated;//lista de odiados del usuario logueado
List* watched;//lista de vistos del usuario logueado
TreeMap* most_faved;//lista de los mas añadidos a favoritos (top global)
TreeMap* most_hated;//lista de los mas añadidos a odiados (top global)
HashMap* genreMap;//mapa para filtrado por genero
HashMap* yearMap;//mapa para filtrado por año
HashMap* typeMap;//mapa para filtrado por tipo

void init_var()//inicializacion de variables, se reserva memoria para cada variable global
{
   catalogue =create_list();
   faved =create_list();
   hated =create_list();
   watched =create_list();
   most_faved =createTreeMap(lower_than_int);
   most_hated =createTreeMap(lower_than_int);
   genreMap =createMap(100);
   yearMap =createMap(70);
   typeMap =createMap(6);
}

anime* create_anime(anime* newanime) //crear anime, se reserva memoria para los parametros que lo requieren
{
    newanime =((anime*)calloc(1,sizeof(anime)));
    newanime->genre =((char*)calloc(140,sizeof(char)));
    newanime->studio =((char*)calloc(140,sizeof(char)));
    return(newanime);//se retorna el anime creado
}

void addAnime(anime* toAdd)//añadir anime a catalogo y vistas filtradas
{
    //Declaraciones
    TreeMap* type;
    TreeMap* year;
    TreeMap* genre;
    //se inicializan los 3 arboles como el resultado de la busqueda con clave respectiva (cada nodo del hashmap debe contener un arbol con sus respectivos anime segun filtro)
    type =searchMap(typeMap,toAdd->type);
    year =searchMap(yearMap,&(toAdd->year));
    genre =searchMap(genreMap,toAdd->genre);
    if (type==NULL)//si no se encontró un arbol para la clave dentro de la tabla hash, se crea uno, se inicializa, y una vez con el anime dentro, se introduce a la tabla
    {
        type =createTreeMap(lower_than_int);
        insertTreeMap(type,&(toAdd->mal_id),toAdd);
        insertMap(typeMap,toAdd->type,type);
    }
    else//del contrario, solo se inserta
    {
        insertTreeMap(type,&(toAdd->mal_id),toAdd);
    }
    if (year==NULL)//lo mismo que caso anterior, tambien sucede con genero
    {
        year =createTreeMap(lower_than_int);
        insertTreeMap(year,&(toAdd->mal_id),toAdd);
        insertMap(yearMap,&(toAdd->year),year);
    }
    else
    {
        insertTreeMap(year,&(toAdd->mal_id),toAdd);
    }
    if (genre==NULL)//mismo que en tipo
    {
        genre =createTreeMap(lower_than_int);
        insertTreeMap(genre,&(toAdd->mal_id),toAdd);
        for (int i = 0; i < 10; i++)//se subdivide el string en cada genero y luego se ingresa el anime a todas las listas filtradas que corresponda
        {
            if (get_csv_field(toAdd->genre,i)==NULL)//si el string recortado es nulo, se rompe el bucle para no generar errores
            {
                break;
            }
            else//sinó
            {
                if (searchMap(genreMap,(char*)get_csv_field(toAdd->genre,i)))//se busca el filtro con el genero respectivo, de estar, solo se inserta el dato
                {
                    TreeMap* genreAux =searchMap(genreMap,(char*)get_csv_field(toAdd->genre,i));
                    insertTreeMap(genreAux,&(toAdd->mal_id),toAdd);
                }
                else//de no estar, se crea un arbol, y se ingresa el dato
                {
                    TreeMap* genre2 =createTreeMap(lower_than_int);
                    insertMap(genreMap,(void*)get_csv_field(toAdd->genre,i),genre2);
                }
            }
        }
    }
    else
    {
        insertTreeMap(genre,&(toAdd->mal_id),toAdd);//se ingresa dato al arbol
    }
    push_back(catalogue,toAdd);//se añade el anime al catalogo sin filtrar
}

void* searchFrom_globalList(int option, char* name)//buscar dentro del top
{
    //declaraciones
    TreeNode* output =NULL;
    TreeMap* lista;
    //si opcion ingresada por user es 1, se busca en favoritos
    if (option==1)
    {
        lista =most_faved;
    }
    else//sinó en odiados
    {
        lista =most_hated;
    }
    HashMap* forSearch =firstTreeMap(lista);
    while (forSearch!=NULL)
    {
        anime* aux =searchMap(forSearch,name);
        if(aux!=NULL)
        {
            output =(lista->current);
            break;
        }
        forSearch =nextTreeMap(lista);
    }
    return(output);
}

void addTo_list(int option, anime* toAdd)//añadir a listas
{
    switch (option)
    {
        case 1://lista vistos
        {
            push_back(watched,toAdd);
            break;
        }
        case 2://lista favoritos
        {
            push_back(faved,toAdd);
            break;
        }
        case 3://lista odiados
        {
            push_back(hated,toAdd);
            break;
        }
        case 4://top favoritos
        {
            TreeNode* positionNode =searchFrom_globalList(1,toAdd->name);
            HashMap* topush;
            if (positionNode==NULL)
            {
                topush =firstTreeMap(most_faved);
                if (topush!=NULL)
                {
                    insertMap(topush,toAdd->name,toAdd);
                }
                else
                {
                    int* key =(int*)calloc(1,sizeof(int));
                    *key =1;
                    topush =createMap(10);
                    insertMap(topush,toAdd->name,toAdd);
                    insertTreeMap(most_faved,key,topush);
                }
            }
            else
            {
                int* key = (int*)calloc(1,sizeof(int));
                *key =*(int*)(positionNode->key);
                *key =((*key)+1);
                topush =positionNode->value;
                eraseMap(topush,toAdd->name);
                topush =NULL;
                topush =searchTreeMap(most_faved,key);
                if (most_faved->current!=NULL)
                {
                    insertMap(topush,toAdd->name,toAdd);
                }
                else
                {
                    topush =createMap(10);
                    insertMap(topush,toAdd->name,toAdd);
                    insertTreeMap(most_faved,key,topush);
                }
            }
            break;
        }
        case 5://top odiados
        {
            TreeNode* positionNode =searchFrom_globalList(2,toAdd->name);
            HashMap* topush;
            if (positionNode==NULL)
            {
                topush =firstTreeMap(most_hated);
                if (topush!=NULL)
                {
                    insertMap(topush,toAdd->name,toAdd);
                }
                else
                {
                    int* key =(int*)calloc(1,sizeof(int));
                    *key =1;
                    topush =createMap(10);
                    insertMap(topush,toAdd->name,toAdd);
                    insertTreeMap(most_hated,key,topush);
                }
            }
            else
            {
                int* key = (int*)calloc(1,sizeof(int));
                *key =*(int*)(positionNode->key);
                *key =((*key)+1);
                topush =positionNode->value;
                eraseMap(topush,toAdd->name);
                topush =NULL;
                topush =searchTreeMap(most_hated,key);
                if (most_hated->current!=NULL)
                {
                    insertMap(topush,toAdd->name,toAdd);
                }
                else
                {
                    topush =createMap(10);
                    insertMap(topush,toAdd->name,toAdd);
                    insertTreeMap(most_hated,key,topush);
                }
            }
            break;
        }
    }
}


void importLists(char username[20])//importar listas de usuario
{
    FILE* input;
    char ruta[150];
    char line[420];
    const char* aux;
    snprintf(ruta,sizeof(ruta),"User/Fav/%s%s",username,".csv");//lista favoritos
    input =fopen(ruta,"r");
    while(fgets(line,421,input) != NULL) //creacion de anime e ingreso a lista
    {
        anime* toAdd =create_anime(toAdd);
        aux =(get_csv_field(line,0));
        toAdd->mal_id =atol(aux);
        aux =(get_csv_field(line,1));
        strcpy(toAdd->name,aux);
        aux =(get_csv_field(line,2));
        strcpy(toAdd->type,aux);
        aux =(get_csv_field(line,3));
        toAdd->episodes =atol(aux);
        aux =(get_csv_field(line,4));
        strcpy(toAdd->duration,aux);
        aux =(get_csv_field(line,5));
        strcpy(toAdd->rating,aux);
        aux =(get_csv_field(line,6));
        toAdd->year =atol(aux);
        aux =(get_csv_field(line,7));
        strcpy(toAdd->studio,aux);
        aux =(get_csv_field(line,8));
        strcpy(toAdd->genre,aux);
        addTo_list(2,toAdd);
    }
    fclose(input);
    snprintf(ruta,sizeof(ruta),"User/Hate/%s%s",username,".csv");//lista odiados
    input =fopen(ruta,"r");
    while(fgets(line,421,input) != NULL) //creacion de anime e ingreso a lista
    {
        anime* toAdd =create_anime(toAdd);
        aux =(get_csv_field(line,0));
        toAdd->mal_id =atol(aux);
        aux =(get_csv_field(line,1));
        strcpy(toAdd->name,aux);
        aux =(get_csv_field(line,2));
        strcpy(toAdd->type,aux);
        aux =(get_csv_field(line,3));
        toAdd->episodes =atol(aux);
        aux =(get_csv_field(line,4));
        strcpy(toAdd->duration,aux);
        aux =(get_csv_field(line,5));
        strcpy(toAdd->rating,aux);
        aux =(get_csv_field(line,6));
        toAdd->year =atol(aux);
        aux =(get_csv_field(line,7));
        strcpy(toAdd->studio,aux);
        aux =(get_csv_field(line,8));
        strcpy(toAdd->genre,aux);
        addTo_list(3,toAdd);
    }
    fclose(input);
    snprintf(ruta,sizeof(ruta),"User/Watched/%s%s",username,".csv");//lista vistos
    input =fopen(ruta,"r");
    while(fgets(line,421,input) != NULL) //creacion de anime e ingreso a lista
    {
        anime* toAdd =create_anime(toAdd);
        aux =(get_csv_field(line,0));
        toAdd->mal_id =atol(aux);
        aux =(get_csv_field(line,1));
        strcpy(toAdd->name,aux);
        aux =(get_csv_field(line,2));
        strcpy(toAdd->type,aux);
        aux =(get_csv_field(line,3));
        toAdd->episodes =atol(aux);
        aux =(get_csv_field(line,4));
        strcpy(toAdd->duration,aux);
        aux =(get_csv_field(line,5));
        strcpy(toAdd->rating,aux);
        aux =(get_csv_field(line,6));
        toAdd->year =atol(aux);
        aux =(get_csv_field(line,7));
        strcpy(toAdd->studio,aux);
        aux =(get_csv_field(line,8));
        strcpy(toAdd->genre,aux);
        addTo_list(1,toAdd);
    }
    fclose(input);
}

void importGlobal_lists()//importar tops
{
    struct dirent *dp;
    FILE* input;
    char line[420];
    const char* aux;
    char ruta[300];
    DIR *dir = opendir("User/Fav");
    while (dp=readdir(dir))//se recorre el directorio favoritos
    {
        if(strlen(dp->d_name)>5)//si el nombre del archivo tiene mas de 5 caracteres(1 nulo del final del string, 3 del formato, mas el punto)
        {
            snprintf(ruta,sizeof(ruta),"User/Fav/%s",dp->d_name);
            input =fopen(ruta,"r");//se abre el archivo
            while(fgets(line,421,input) != NULL) //se crea cada anime del archivo y se importa al top
            {
                anime* toAdd =create_anime(toAdd);
                aux =(get_csv_field(line,0));
                toAdd->mal_id =atol(aux);
                aux =(get_csv_field(line,1));
                strcpy(toAdd->name,aux);
                aux =(get_csv_field(line,2));
                strcpy(toAdd->type,aux);
                aux =(get_csv_field(line,3));
                toAdd->episodes =atol(aux);
                aux =(get_csv_field(line,4));
                strcpy(toAdd->duration,aux);
                aux =(get_csv_field(line,5));
                strcpy(toAdd->rating,aux);
                aux =(get_csv_field(line,6));
                toAdd->year =atol(aux);
                aux =(get_csv_field(line,7));
                strcpy(toAdd->studio,aux);
                aux =(get_csv_field(line,8));
                strcpy(toAdd->genre,aux);
                addTo_list(4,toAdd);
            }
            fclose(input);
        }
    }
    closedir(dir);// se cierra el directorio
    //lo mismo que recien, pero con los odiados
    dir = opendir("User/Hate");
    while (dp=readdir(dir))
    {
        if(strlen(dp->d_name)>5)
        {
            snprintf(ruta,sizeof(ruta),"User/Hate/%s",dp->d_name);
            input =fopen(ruta,"r");
            while(fgets(line,421,input) != NULL) 
            {
                anime* toAdd =create_anime(toAdd);
                aux =(get_csv_field(line,0));
                toAdd->mal_id =atol(aux);
                aux =(get_csv_field(line,1));
                strcpy(toAdd->name,aux);
                aux =(get_csv_field(line,2));
                strcpy(toAdd->type,aux);
                aux =(get_csv_field(line,3));
                toAdd->episodes =atol(aux);
                aux =(get_csv_field(line,4));
                strcpy(toAdd->duration,aux);
                aux =(get_csv_field(line,5));
                strcpy(toAdd->rating,aux);
                aux =(get_csv_field(line,6));
                toAdd->year =atol(aux);
                aux =(get_csv_field(line,7));
                strcpy(toAdd->studio,aux);
                aux =(get_csv_field(line,8));
                strcpy(toAdd->genre,aux);
                addTo_list(5,toAdd);
            }
            fclose(input);
        }
    }
    closedir(dir);
    return;
}
/*Import Catalogue File*/
void importCatalogue()
{
    FILE* input;
    input =fopen("DB.csv","r");
    if (input==NULL)
    {
        printf("Falta Base de datos! (ΠΦΠ)/\n");
        exit(1);
    }
    char line[420];
    const char* aux;
    while(fgets(line,421,input) != NULL) 
    {
        anime* toAdd =create_anime(toAdd);
        aux =(get_csv_field(line,0));
        toAdd->mal_id =atol(aux);
        aux =(get_csv_field(line,1));
        strcpy(toAdd->name,aux);
        aux =(get_csv_field(line,2));
        strcpy(toAdd->type,aux);
        aux =(get_csv_field(line,3));
        toAdd->episodes =atol(aux);
        aux =(get_csv_field(line,4));
        strcpy(toAdd->duration,aux);
        aux =(get_csv_field(line,5));
        strcpy(toAdd->rating,aux);
        aux =(get_csv_field(line,6));
        toAdd->year =atol(aux);
        aux =(get_csv_field(line,7));
        strcpy(toAdd->studio,aux);
        aux =(get_csv_field(line,8));
        strcpy(toAdd->genre,aux);
        addAnime(toAdd);
    }
    fclose(input);
    //clear_Screen();
}

void export_userData(char* username)//exportar listas del usuario (para no perder los datos despues de ejecutar)
{
    FILE* output;
    char ruta[150];
    char cadena[512];
    anime* aux =first(faved);
    snprintf(ruta,sizeof(ruta),"User/Fav/%s%s",username,".csv");
    output =fopen(ruta,"w");
    while(aux!=NULL)
    {
        snprintf(cadena,sizeof(cadena),"%ld,\"%s\",%s,%d,%s,%s,%d,\"%s\",\"%s\"\n",aux->mal_id,aux->name,aux->type,aux->episodes,aux->duration,aux->rating,aux->year,aux->studio,aux->genre);
        fputs(cadena,output);
        aux =next(faved);
    }
    fclose(output);
    snprintf(ruta,sizeof(ruta),"User/Hate/%s%s",username,".csv");
    output =fopen(ruta,"w");
    aux =first(hated);
    while(aux!=NULL)
    {
        snprintf(cadena,sizeof(cadena),"%ld,\"%s\",%s,%d,%s,%s,%d,\"%s\",\"%s\"\n",aux->mal_id,aux->name,aux->type,aux->episodes,aux->duration,aux->rating,aux->year,aux->studio,aux->genre);
        fputs(cadena,output);
        aux =next(hated);
    }
    fclose(output);
    snprintf(ruta,sizeof(ruta),"User/Watched/%s%s",username,".csv");
    output =fopen(ruta,"w");
    aux =first(watched);
    while(aux!=NULL)
    {
        snprintf(cadena,sizeof(cadena),"%ld,\"%s\",%s,%d,%s,%s,%d,\"%s\",\"%s\"\n",aux->mal_id,aux->name,aux->type,aux->episodes,aux->duration,aux->rating,aux->year,aux->studio,aux->genre);
        fputs(cadena,output);
        aux =next(watched);
    }
    fclose(output);
}
/*
void export_top(short int option)//exportar top, esta funcion no se usara ya que esta hecha para el testeo nomas
{
    FILE* output;
    HashMap* auxMap;
    anime* aux;
    char cadena[512];
    if (option==(1))
    {
        output =fopen("User/top_fav.csv","w");
        auxMap =firstTreeMap(most_faved);
        while(auxMap!=NULL) 
        {
            aux =firstMap(auxMap);
            while (aux!=NULL)
            {
                snprintf(cadena,sizeof(cadena),"%ld,\"%s\",%s,%d,%s,%s,%d,\"%s\",\"%s\"\n",aux->mal_id,aux->name,aux->type,aux->episodes,aux->duration,aux->rating,aux->year,aux->studio,aux->genre);
                fputs(cadena,output);
                aux =nextMap(auxMap);
            }
            auxMap =nextTreeMap(most_faved);
        }
    }
    else
    {
        output =fopen("User/top_hate.csv","w");
        auxMap =firstTreeMap(most_hated);
        while(auxMap!=NULL) 
        {
            aux =firstMap(auxMap);
            while (aux!=NULL)
            {
                snprintf(cadena,sizeof(cadena),"%ld,\"%s\",%s,%d,%s,%s,%d,\"%s\",\"%s\"\n",aux->mal_id,aux->name,aux->type,aux->episodes,aux->duration,aux->rating,aux->year,aux->studio,aux->genre);
                fputs(cadena,output);
                aux =nextMap(auxMap);
            }
            auxMap =nextTreeMap(most_faved);
        }
    }
   fclose(output);
}
*/

void print_Catalogue()//imprime catalogo
{
    char cadena[520];
    char option;
    anime* toAdd =first(catalogue);
    scanf("%c",&option);
    while (1)
    {
        for (int i = 0; i < 10; i =(i+1))//se imprimen 10 animes (0-9)
        {
            if (toAdd==NULL)
            {
                break;
            }
            //se crea cadena con todos los atributos del anime, y luego se imprime
            snprintf(cadena,sizeof(cadena),"%ld,%s,%s,%d,%s,%s,%d,%s,%s\n",toAdd->mal_id,toAdd->name,toAdd->type,toAdd->episodes,toAdd->duration,toAdd->rating,toAdd->year,toAdd->studio,toAdd->genre);
            printf("%d - %s\n",i,cadena);
            toAdd =next(catalogue);
        }
        printf("En caso de querer cambiar de pagina presione enter\nEn caso de querer cancelar la ejecucion ingrese cualquier caracter\n");
        scanf("%c",&option);//se pregunta al usuario si cancelar ejecucion o seguir cambiando de pagina
        if (option=='\n')
        {
            clear_Screen();//limpieza de pantalla
            continue;
        }
        break;
    }
    clear_Screen();
    return;
}

void* filteredSearch(void* key, int filter)//busqueda con filtros
{
    TreeMap* animelist;
    anime* toAdd;
    char option ='\n';
    int steps =0;
    char cadena[512];
    switch (filter)
    {
        case 1://filtro por año
        {
            animelist =searchMap(yearMap,key);
            if (animelist==NULL)
            {
                printf("No Hay anime de ese año! :c\n");
                return(NULL);
            }
            break;
        }
        case 2://filtro por tipo (pelicula,ova,ona,tv)
        {
            animelist =searchMap(typeMap,key);
            if (animelist==NULL)
            {
                printf("No Hay anime de ese tipo! :c\n");
                return(NULL);
            }
            break;
        }
        case 3://filtro por genero
        {
            animelist =searchMap(genreMap,key);
            if (animelist==NULL)
            {
                printf("No Hay anime de ese genero! :c\n");
                return(NULL);
            }
            break;
        }
    }   
    toAdd =firstTreeMap(animelist);
    scanf("%c",&option);
    int aux;
    while (1)//se muestran anime en intervalos de 10 hasta que el user elija uno
    {
        for (int i = 0; i < 10; i =(i+1))
        {
            if (toAdd==NULL)
            {
                break;
            }
            snprintf(cadena,sizeof(cadena),"%ld,%s,%s,%d,%s,%s,%d,%s,%s\n",toAdd->mal_id,toAdd->name,toAdd->type,toAdd->episodes,toAdd->duration,toAdd->rating,toAdd->year,toAdd->studio,toAdd->genre);
            printf("%d - %s\n",i,cadena);
            toAdd =nextTreeMap(animelist);
        }
        printf("Escriba el numero del anime a ingresar a la lista de vistos\nEn caso de querer cambiar de pagina presione enter\nEn caso de querer cancelar la ejecucion ingrese la letra \'x\'\n");
        scanf("%c",&option);
        if (option=='x')
        {
            return(NULL);
        }
        else
        {
            if(option =='\n')
            {
                steps= steps+10;
                clear_Screen();
                continue;
            }
            aux =(atoi(&option));
            if ((aux>=0)&&(aux<10))
            {
                steps=(steps+aux);
                toAdd =firstTreeMap(animelist);
                for (int i = 0; i < steps; i=(i+1))
                {
                    toAdd =nextTreeMap(animelist);
                }
                return(toAdd);
            }
        }       
    }
    return(NULL);
}

void* searchFrom_list()//buscar desde lista de visto, esta funcion la usan las funciones de agregar a favoritos y odiado, ya que no se puede añadir a favoritos un anime sin haberlo visto
{
    anime* toAdd;
    int steps;
    char option;
    char cadena[512];
    toAdd =first(watched);
    scanf("%c",&option);
    while (1)
    {
        if (toAdd==NULL)
        {
            break;
        }
        for (int i = 0; i < 10; i =(i+1))
        {
            if (toAdd==NULL)
            {
                break;
            }
            snprintf(cadena,sizeof(cadena),"%ld,%s,%s,%d,%s,%s,%d,%s,%s\n",toAdd->mal_id,toAdd->name,toAdd->type,toAdd->episodes,toAdd->duration,toAdd->rating,toAdd->year,toAdd->studio,toAdd->genre);
            printf("%d - %s\n",i,cadena);
            toAdd =next(watched);
        }
        printf("Escriba el numero del anime a ingresar a la lista de vistos\nEn caso de querer cambiar de pagina presione enter\nEn caso de querer cancelar la ejecucion ingrese la letra \'x\'\n");
        scanf("%c",&option);
        if (option=='x')
        {
            return(NULL);
        }
        else
        {
            if (option=='\n')
            {
                steps= steps+10;
                continue;
            }
            if ((atoi(&option)>=0)&&(atoi(&option)<10))
            {
                steps=(steps+(atoi(&option)));
                toAdd =first(watched);
                for (int i = 0; i < steps; i=(i+1))
                {
                    toAdd =next(watched);
                }
                return(toAdd);
            }
        }       
    }
    return(NULL);
}

void print_list(int option)//imprimir lista
{
    anime* aux;
    List* touse;
    switch (option)
    {
        case 1://lista vistos
        {
            touse =(watched);
            break;
        }
        case 2://lista favoritos
        {
            touse =(faved);
            break;
        }
        case 3://lista odiados
        {
            touse =(hated);
            break;
        }
    }
    aux =(first(touse));
    if (aux == NULL)
    {
        printf("Lista Vacia!!!\n");
        return;
    }
    printf("%-5s\t│%-50.50s │%-11.11s │%-4s │%-15.15s │%-17.17s │%-4s │%-30.30s │%-30.30s\n","ID","Nombre","Tipo","Capitulos","Duracion","Rating","Año","Estudio","Genero");
    printf("⎼⎼⎼⎼⎼⎼⎼⎼┼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼┼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼┼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼┼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼┼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼┼⎼⎼⎼⎼┼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼┼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼\n");
    while(aux!=NULL) 
    {
        printf("%-5ld\t│%-50.50s │%-11.11s │%-9d │%-15.15s │%-17.17s │%-4d│%-30.30s │%-30.30s\n",aux->mal_id,aux->name,aux->type,aux->episodes,aux->duration,aux->rating,aux->year,aux->studio,aux->genre);
        aux =next(touse);
    }
}

void* non_filteredSearch()//busqueda sin filtrar
{
    char cadena[520];
    char option;
    int steps;
    anime* toAdd =first(catalogue);
    while (1)
    {
        for (int i = 0; i < 10; i =(i+1))
        {
            if (toAdd==NULL)
            {
                break;
            }
            snprintf(cadena,sizeof(cadena),"%ld,%s,%s,%d,%s,%s,%d,%s,%s\n",toAdd->mal_id,toAdd->name,toAdd->type,toAdd->episodes,toAdd->duration,toAdd->rating,toAdd->year,toAdd->studio,toAdd->genre);
            printf("%d - %s\n",i,cadena);
            toAdd =next(catalogue);
        }
        printf("Escriba el numero del anime a ingresar a la lista de vistos\nEn caso de querer cambiar de pagina presione enter\nEn caso de querer cancelar la ejecucion ingrese la letra \'x\'\n");
        scanf("%c",&option);
        if (option=='x')
        {
            return(NULL);
        }
        else
        {
            if (option=='\n')
            {
                steps= steps+10;
                continue;
            }
            if ((atoi(&option)>=0)&&(atoi(&option)<10))
            {
                steps=(steps+(atoi(&option)));
                toAdd =first(catalogue);
                for (int i = 0; i < steps; i=(i+1))
                {
                    toAdd =next(catalogue);
                }
                return(toAdd);
            }
        }       
    }
    return(NULL);
}

void top_loved()//top favoritos
{
    if (most_faved->root==NULL)
    {
        printf("LISTA VACIA!!!!\n");
        return;
    }
    HashMap* toSearch =last_tree(most_faved);
    anime* recommend =firstMap(toSearch);
    char cadena [520];
    char option;
    scanf("%c",&option);
    while (1)
    {
        for (int i = 0; i < 10; i =(i+1))
        {
            if (recommend==NULL)
            {
                break;
            }
            snprintf(cadena,sizeof(cadena),"%ld,%s,%s,%d,%s,%s,%d,%s,%s\n",recommend->mal_id,recommend->name,recommend->type,recommend->episodes,recommend->duration,recommend->rating,recommend->year,recommend->studio,recommend->genre);
            printf("%d - %s\n",i,cadena);
            recommend =nextMap(toSearch);
        }
        printf("En caso de querer cambiar de pagina presione enter\nEn caso de querer cancelar la ejecucion ingrese cualquier caracter\n");
        scanf("%c",&option);
        if (option=='\n')
        {
            if (recommend==NULL)
            {
                printf("Se Acabó el top!!! :c\n");
                fflush(stdin);
                return;
            }
            continue;
        }
        return;
    }
    return;
}

void top_hated()//top odiados
{
    if (most_hated->root==NULL)
    {
        printf("LISTA VACIA!!!!\n");
        return;
    }
    HashMap* toSearch =last_tree(most_hated);
    anime* recommend =firstMap(toSearch);
    char cadena [520];
    char option;
    scanf("%c",&option);
    while (1)
    {
        for (int i = 0; i < 10; i =(i+1))
        {
            if (recommend==NULL)
            {
                break;
            }
            snprintf(cadena,sizeof(cadena),"%ld,%s,%s,%d,%s,%s,%d,%s,%s\n",recommend->mal_id,recommend->name,recommend->type,recommend->episodes,recommend->duration,recommend->rating,recommend->year,recommend->studio,recommend->genre);
            printf("%d - %s\n",i,cadena);
            recommend =nextMap(toSearch);
        }
        printf("En caso de querer cambiar de pagina presione enter\nEn caso de querer cancelar la ejecucion ingrese cualquier caracter\n");
        scanf("%c",&option);
        if (option=='\n')
        {
            if (recommend==NULL)
            {
                printf("Se Acabó el top!!! :c\n");
                return;
            }
            continue;
        }
        break;
    }
    return;
}