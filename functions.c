#include <stdio.h>
#include <stdlib.h>
#include "hashtable.h"
#include "treemap.h"
#include "list.h"
#include "lectura_csv.c"

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
        push_back(faved,toAdd);
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
        push_back(hated,toAdd);
    }
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
        push_back(watched,toAdd);
    }
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

void export_top(short int option)
{
    FILE* output;
    anime* aux;
    char cadena[512];
    if (option==(1))
    {
        output =fopen("top_fav.csv","w");
        aux =last_tree(most_faved);
        while(aux!=NULL) 
        {
            snprintf(cadena,sizeof(cadena),"%ld,\"%s\",%s,%d,%s,%s,%d,\"%s\",\"%s\"\n",aux->mal_id,aux->name,aux->type,aux->episodes,aux->duration,aux->rating,aux->year,aux->studio,aux->genre);
            fputs(cadena,output);
            aux =prev_tree(most_faved);
        }
    }
    else
    {
        output =fopen("top_hate.csv","w");
        aux =last_tree(most_hated);
        while(aux!=NULL) 
        {
            snprintf(cadena,sizeof(cadena),"%ld,\"%s\",%s,%d,%s,%s,%d,\"%s\",\"%s\"\n",aux->mal_id,aux->name,aux->type,aux->episodes,aux->duration,aux->rating,aux->year,aux->studio,aux->genre);
            fputs(cadena,output);
            aux =prev_tree(most_hated);
        }
    }
   fclose(output);
}

void print_Catalogue()
{
   anime* aux;
   aux =(first(catalogue));
   printf("%-5ld │%-50.50s │%-11.11s │%-4d │%-15.15s │%-17.17s │%-4d │%-30.30s │%-30.30s\n",aux->mal_id,aux->name,aux->type,aux->episodes,aux->duration,aux->rating,aux->year,aux->studio,aux->genre);
   printf("⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼┼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼┼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼┼⎼⎼⎼⎼⎼┼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼┼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼┼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼┼⎼⎼⎼⎼⎼⎼⎼⎼\n");
   aux =next(catalogue);
   while(aux!=NULL) 
   {
        printf("%-5.5ld\t│%-50.50s │%-11.11s │%-4.4d │%-15.15s │%-17.17s │%-4.4d │%-30.30s │%-30.30s\n",aux->mal_id,aux->name,aux->type,aux->episodes,aux->duration,aux->rating,aux->year,aux->studio,aux->genre);
        aux =next(catalogue);
   }
   free(aux);
}

void* filteredSearch(void* key, int filter)
{
    TreeMap* animelist;
    anime* toAdd;
    char option ='\n';
    int steps;
    char cadena[512];
    switch (filter)
    {
        case 1:
        {
            animelist =searchMap(yearMap,key);
            break;
        }
        case 2:
        {
            animelist =searchMap(typeMap,key);
            break;
        }
        case 3:
        {
            animelist =searchMap(genreMap,key);
            break;
        }
    }   
    toAdd =firstTreeMap(animelist);
    while (1)
    {
        for (int i = 0; i < 10; i =(i+1))
        {
            snprintf(cadena,sizeof(cadena),"%ld,%s,%s,%d,%s,%s,%d,%s,%s\n",toAdd->mal_id,toAdd->name,toAdd->type,toAdd->episodes,toAdd->duration,toAdd->rating,toAdd->year,toAdd->studio,toAdd->genre);
            printf("%d - %s\n",i,cadena);
            toAdd =nextTreeMap(animelist);
        }
        printf("Escriba el anime a ingresar a la lista de vistos\nEn caso de querer cambiar de pagina presione enter\n");
        scanf("%c",&option);
        scanf("%c",&option);
        if (option=='x')
        {
            return(NULL);
        }
        else
        {
            if (option=='\n')
            {
                continue;
            }
            if ((atoi(&option)>=0)&&(atoi(&option)<10))
            {
                steps=(10-option);
                for (int i = 0; i <= steps; i=(i+1))
                {
                    toAdd =prev_tree(animelist);
                }
                return(toAdd);
            }
        }       
    }
    return(NULL);
}

void addTo_list(int option, void* toAdd)
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
    }
}


void* searchFrom_list()
{
    return (NULL);
}

void print_list(int option)
{

}

void non_filteredSearch()
{

}