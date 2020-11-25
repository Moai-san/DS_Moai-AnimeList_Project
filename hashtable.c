//Leonardo Gonzalez Catalan - September/29/2020
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct Pair Pair;
typedef struct HashMap HashMap;

struct Pair
{
  char* key;
  void* value;
};

struct HashMap
{
  Pair** buckets;
  long size;
  long capacity;
  long current;
};

Pair* createPair(void* key, void* value)
{
  Pair* newP =((Pair*)calloc(1,sizeof(Pair)));
  newP->key =(key);
  newP->value =(value);
  return (newP);
}

long hash( void* key, long capacity)
{
  unsigned long hash =(0);
  char* ptr;
  for (ptr =(key);(*ptr!='\0');ptr =(ptr+1))
  {
    hash = (hash+(hash*32)+(tolower(*ptr)));
  }
  return (hash%capacity);
}

int is_equal(void* key1, void* key2)
{
  if((key1==(NULL))||(key2==(NULL)))
  {
    return (0);
  }
  if ((strcmp((char*)key1,(char*)key2))==(0))
  {
    return (1);
  }
  return (0);
}

void insertReHashed(HashMap* map, void* key, void* value) 
{
  long position;
  Pair* a =(createPair(key,value));
  position = hash(key,map->capacity);
  while (map->buckets[position]!=NULL)
  {
    position =(position+1);
    if (position>=(map->capacity))
    {
      position =(position-(map->capacity));
    }
  }
  map->buckets[position] =(a);
}

void enlarge(HashMap * map)
{
  Pair** pivot;
  map->capacity =((map->capacity)*2);  
  pivot =(map->buckets);
  map->buckets =((Pair**)calloc(map->capacity,(sizeof(Pair*))));
  Pair* test;
  for (int i=0;(i<(map->capacity)/2);i =(i+1))
  {
    test =(pivot[i]);
    if (test!=(NULL))
    {
      insertReHashed(map,test->key,test->value);
    }
  }
  //free(test);
  //free(pivot);
}

void insertMap(HashMap* map, void* key, void* value) 
{
  long position;
  Pair* a =(createPair(key,value));
  position =(hash(key,map->capacity));
  while ((map->buckets[position])!=(NULL))
  {
    position =(position+1);
    if (position>=(map->capacity))
    {
      position =(position-(map->capacity));
    }
  }
  map->buckets[position] =(a);
  map->size =((map->size)+1);
  if ((map->size)>((map->capacity)*0.7))
  {
    enlarge(map);
  }
}

HashMap* createMap(long capacity)
{
  HashMap* map =((HashMap*)calloc(1,(sizeof(HashMap))));
  map->size =(0);
  map->buckets =((Pair**)calloc(capacity,(sizeof(Pair *))));
  map->capacity =(capacity);
  map->current =(-1);
  return map;
}

void* searchMap(HashMap* map, void* key)
{   
  if ((map->size)==(0))
  {
    return (NULL);
  }
  long position;
  position =(hash(key,(map->capacity)));
  long count =(0);
  while (1)
  {
    if ((map->buckets[position])!=(NULL))
    {
      if ((is_equal(key,(map->buckets[position]->key)))==(0))
      {
        position =(position+1);
        count =(count+1);
        while(1)
        {
          if ((map->buckets[position])==(NULL))
          {
            count =(count+1);
            position =(position+1);
            if (position>=(map->capacity))
            {
              position =(position-(map->capacity));
            }
            if (count>=(map->capacity)-1)
            {
              map->current =(-1);
              return (NULL);
            }
          }
          else
          {
            break;
          }
        }
      }
      else
      {
        break;
      }
    }
    else
    {
      position =(position+1);
      count =(count+1);
      if (position>=(map->capacity))
      {
        position =(position-(map->capacity));
      }
      if (count>=(map->capacity)-1)
      {
        map->current =(-1);
        return (NULL);
      }
    }
        
  }
  void* output;
  //output =((void*)calloc(1,(sizeof(void))));
  output =(map->buckets[position]->value);
  map->current =(position);
  return (output);
}

void eraseMap(HashMap* map, void* key)
{    
  searchMap(map,key);
  if((map->current)==(-1))
  {
    printf ("Elemento inexistente!");
    return;
  }
  map->buckets[map->current]->key =(NULL);
  map->size =((map->size)-1);
}

void* firstMap(HashMap* map)
{
  if ((map->size)==(0))
  {
    printf ("Mapa Vacio!");
    return (NULL);
  }
  map->current =(0);
  while (1)
  {
    if (((map->buckets[map->current])==(NULL))||((map->buckets[map->current]->key)==(NULL)))
    {
      map->current =((map->current)+1);
    }
    else
    {
      break;
    }
  }
  return ((map->buckets[map->current]->value));
}

void* nextMap(HashMap* map)
{
  if ((map->buckets)==(NULL))
  {
    return (NULL);
  }
  map->current =((map->current)+1);
  while (1)
  {
    if (map->current>=map->capacity)
    {
      map->current =0;
      return (NULL);
    }
    if (((map->buckets[map->current])==(NULL))||((map->buckets[map->current]->key)==(NULL)))
    {
      map->current =((map->current)+1);
    }
    else
    {
      break;
    }
  }
  return (map->buckets[map->current]->value);
}
