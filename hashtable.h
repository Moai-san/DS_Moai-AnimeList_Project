//Leonardo Gonzalez Catalan - September/29/2020
#ifndef HashMap_h
#define HashMap_h
#include "hashtable.c"

typedef struct HashMap HashMap;
typedef struct Pair Pair;

HashMap* createMap(long capacity);
Pair* createPair(void* key, void* value);
long hash( void* key, long capacity);
void insertMap(HashMap* map, void* key, void* value);
void insertReHashed(HashMap* map, void* key, void* value);
void eraseMap(HashMap* map, void* key);
void enlarge(HashMap* map);
void* searchMap(HashMap* map, void* key);
void* firstMap(HashMap* map);
void* nextMap(HashMap* map);

#endif
