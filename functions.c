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
List* catalogue;
List* faved;
List* hated;
List* watched;
TreeMap* most_faved;
TreeMap* most_hated;
HashMap* genreMap;
HashMap* yearMap;
HashMap* typeMap;

void init_var()
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

anime* create_anime(anime* newanime)
{
    newanime =((anime*)calloc(1,sizeof(anime)));
    newanime->genre =((char*)calloc(140,sizeof(char)));
    newanime->studio =((char*)calloc(140,sizeof(char)));
    return(newanime);
}

void addAnime(anime* toAdd)
{
    //Declaraciones/inicializaciones
    TreeMap* type;
    TreeMap* year;
    TreeMap* genre;
    type =searchMap(typeMap,toAdd->type);
    year =searchMap(yearMap,&(toAdd->year));
    genre =searchMap(genreMap,toAdd->genre);
    if (type==NULL)
    {
        type =createTreeMap(lower_than_int);
        insertTreeMap(type,&(toAdd->mal_id),toAdd);
        insertMap(typeMap,toAdd->type,type);
    }
    else
    {
        insertTreeMap(type,&(toAdd->mal_id),toAdd);
    }
    if (year==NULL)
    {
        year =createTreeMap(lower_than_int);
        insertTreeMap(year,&(toAdd->mal_id),toAdd);
        insertMap(yearMap,&(toAdd->year),year);
    }
    else
    {
        insertTreeMap(year,&(toAdd->mal_id),toAdd);
    }
    if (genre==NULL)
    {
        type =createTreeMap(lower_than_int);
        insertTreeMap(genre,&(toAdd->mal_id),toAdd);
        insertMap(genreMap,toAdd->genre,genre);
    }
    else
    {
        insertTreeMap(genre,&(toAdd->mal_id),toAdd);
    }
    push_back(catalogue,toAdd);
}

void* searchFrom_globalList(int option, char* name)
{
    TreeNode* output =NULL;
    TreeMap* lista;
    if (option==1)
    {
        lista =most_faved;
    }
    else
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

void addTo_list(int option, anime* toAdd)
{
    switch (option)
    {
        case 1:
        {
            push_back(watched,toAdd);
            break;
        }
        case 2:
        {
            push_back(faved,toAdd);
            break;
        }
        case 3:
        {
            push_back(hated,toAdd);
            break;
        }
        case 4:
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
        case 5:
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


void importLists(char username[20])
{
    FILE* input;
    char ruta[150];
    char line[420];
    const char* aux;
    snprintf(ruta,sizeof(ruta),"User/Fav/%s%s",username,".csv");
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
        addTo_list(2,toAdd);
    }
    fclose(input);
    snprintf(ruta,sizeof(ruta),"User/Hate/%s%s",username,".csv");
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
        addTo_list(3,toAdd);
    }
    fclose(input);
    snprintf(ruta,sizeof(ruta),"User/Watched/%s%s",username,".csv");
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
        addTo_list(1,toAdd);
    }
    fclose(input);
}

void importGlobal_lists()
{
    struct dirent *dp;
    FILE* input;
    char line[420];
    const char* aux;
    char ruta[300];
    DIR *dir = opendir("User/Fav");
    while (dp=readdir(dir))
    {
        if(strlen(dp->d_name)>5)
        {
            snprintf(ruta,sizeof(ruta),"User/Fav/%s",dp->d_name);
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
                addTo_list(4,toAdd);
            }
            fclose(input);
        }
    }
    closedir(dir);
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

void export_userData(char* username)
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
void export_top(short int option)
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

void print_Catalogue()
{
    char cadena[520];
    char option;
    anime* toAdd =first(catalogue);
    scanf("%c",&option);
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
        printf("En caso de querer cambiar de pagina presione enter\nEn caso de querer cancelar la ejecucion ingrese cualquier caracter\n");
        scanf("%c",&option);
        if (option=='\n')
        {
            clear_Screen();
            continue;
        }
        break;
    }
    clear_Screen();
    return;
}

void* filteredSearch(void* key, int filter)
{
    TreeMap* animelist;
    anime* toAdd;
    char option ='\n';
    int steps =0;
    char cadena[512];
    switch (filter)
    {
        case 1:
        {
            animelist =searchMap(yearMap,key);
            if (animelist==NULL)
            {
                printf("No Hay anime de ese año! :c\n");
                return(NULL);
            }
            break;
        }
        case 2:
        {
            animelist =searchMap(typeMap,key);
            if (animelist==NULL)
            {
                printf("No Hay anime de ese tipo! :c\n");
                return(NULL);
            }
            break;
        }
        case 3:
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

void* searchFrom_list()
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

void print_list(int option)
{
    anime* aux;
    List* touse;
    switch (option)
    {
        case 1:
        {
            touse =(watched);
            break;
        }
        case 2:
        {
            touse =(faved);
            break;
        }
        case 3:
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

void* non_filteredSearch()
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

void top_loved()
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
                return;
            }
            continue;
        }
        return;
    }
    return;
}

void top_hated()
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