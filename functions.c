#include <stdio.h>
#include <stdlib.h>
#include "hashtable.h"
#include "treemap.h"
#include "list.h"
#include "lectura_csv.c"

typedef struct anime anime;

struct anime
{
   int mal_id; //myanimelist_id
   char* name; //nombre
   char type[11]; //tipo de anime (movie,tv,web-series,special,ova)
   int episodes; //cantidad de episodios
   char* duration; //duracion
   char* rating; //rating (calificacion de contenido)
   int year; //año lanzamiento
   char* studio; //estudio animacion
   char* genre; //genero anime
};

//variables globales
List* catalogue;
List* most_faved;
List* most_hated;
HashMap* genreMap;
HashMap* yearMap;
HashMap* typeMap;

void init_var()
{
   catalogue =((List*)calloc(1,(sizeof(List))));
   most_faved =((List*)calloc(1,(sizeof(List))));
   most_hated =((List*)calloc(1,(sizeof(List))));
   catalogue =create_list();
   most_faved =create_list();
   most_hated =create_list();
   genreMap =((HashMap*)calloc(1,(sizeof(HashMap))));
   yearMap =((HashMap*)calloc(1,(sizeof(HashMap))));
   typeMap =((HashMap*)calloc(1,(sizeof(HashMap))));
   genreMap =createMap(100);
   yearMap =createMap(70);
   typeMap =createMap(6);
}

anime* create_anime()
{
    anime* newanime =((anime*)calloc(1,sizeof(anime)));
    newanime->duration =((char*)calloc(64,sizeof(char)));
    newanime->genre =((char*)calloc(120,sizeof(char)));
    newanime->name =((char*)calloc(120,sizeof(char)));
    newanime->rating =((char*)calloc(40,sizeof(char)));
    newanime->studio =((char*)calloc(120,sizeof(char)));
    return(newanime);
}

int addAnime(anime* toAdd)
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
   char line[512];
   int i=0;
   anime* toAdd;
   while(fgets(line,513,input) != NULL) 
   {

        if (i==0)
        {
            i=1;
            continue;
        }
        if(i!=0)
        {
            toAdd =create_anime();
            toAdd->mal_id =atol(get_csv_field(line,0));
            strcpy(toAdd->name,(char*)(get_csv_field(line,1)));
            strcpy(toAdd->type,(char*)(get_csv_field(line,2)));
            toAdd->episodes =atol(get_csv_field(line,3));
            strcpy(toAdd->duration,(char*)(get_csv_field(line,4)));
            strcpy(toAdd->rating,(char*)(get_csv_field(line,5)));
            toAdd->year =atol(get_csv_field(line,6));
            strcpy(toAdd->studio,(char*)(get_csv_field(line,7)));
            strcpy(toAdd->genre,(char*)(get_csv_field(line,8)));
            addAnime(toAdd);
            printf("%s\n",toAdd->name);
        }
   }
   fclose(input);
   //clear_Screen();
}

void print_Catalogue()
{
   anime* aux;
   aux =(first(catalogue));
   printf("%-5d│%-50.50s│%-11s│%-4d│%-15.15s│%-11.11s│%-4d│%-20.20s│%s\n",aux->mal_id,aux->name,aux->type,aux->episodes,aux->duration,aux->rating,aux->year,aux->studio,aux->genre);
   printf("⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼┼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼┼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼┼⎼⎼⎼⎼⎼┼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼┼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼┼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼┼⎼⎼⎼⎼⎼⎼⎼⎼\n");
   aux =next(catalogue);
   while(aux!=NULL) 
   {
        printf("%-5d│%-50.50s│%-11s│%-4d│%-15.15s│%-11.11s│%-4d│%-20.20s│%s\n",aux->mal_id,aux->name,aux->type,aux->episodes,aux->duration,aux->rating,aux->year,aux->studio,aux->genre);
        aux =next(catalogue);
   }
   free(aux);
}