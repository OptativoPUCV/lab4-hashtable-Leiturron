#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) 
{
  long pos = hash(key, map->capacity);
  while(map->buckets[pos] != NULL && map->buckets[pos]->key != NULL)
    {
        pos++;
      if(pos == map->capacity) pos = 0;
    }
  map->buckets[pos] = createPair(key, value);
  map->size++;

  float verifi = (float)map->size/map->capacity;
  if(verifi > 0.7) enlarge(map);
}

void enlarge(HashMap * map) 
{
  enlarge_called = 1; //no borrar (testing purposes)
  Pair** old_buckets = map->buckets;
  long old_capacity = map->capacity;
  map->capacity *= 2;
  map->buckets = (Pair**)malloc(map->capacity * sizeof(Pair*));
  for(int i = 0; i < old_capacity; i++)
    {
      if(old_buckets[i] != NULL)
      {
        insertMap(map, old_buckets[i]->key, old_buckets[i]->value);
        free(old_buckets[i]);
      }
    }
  free(old_buckets);
}


HashMap * createMap(long capacity) 
{
  HashMap * map = (HashMap *)malloc(sizeof(HashMap));
  map->buckets = (Pair **)malloc(sizeof(Pair *) * capacity);
  map->size = 0;
  map->capacity = capacity;
  map->current = -1;
  return map;
}

void eraseMap(HashMap * map,  char * key) 
{    
  long pos = hash(key, map->capacity);
  while(map->buckets[pos] != NULL)
    {
      if(strcmp(map->buckets[pos]->key, key) == 0)
      {
        map->buckets[pos]->key = NULL;
        map->size--;
        return;
      }
      else
        pos++;
    }
}

Pair * searchMap(HashMap * map,  char * key) 
{   
  long pos = hash(key, map->capacity);
  while(map->buckets[pos] != NULL)
    {
      if(strcmp(map->buckets[pos]->key, key) == 0)
      {
        map->current = pos;
        return map->buckets[pos];
      }
      pos++;
    }
  return NULL;
}

Pair * firstMap(HashMap * map) {
  long pos = map->current;
  while(map->buckets[pos] == NULL || map->buckets[pos]->key == NULL)
    {
      pos++;
      if(pos > map->capacity) return NULL;
    }
  map->current = pos;
  return map->buckets[pos];
}

Pair * nextMap(HashMap * map) {
  long pos = map->current + 1;
  while(map->buckets[pos] == NULL || map->buckets[pos]->key == NULL)
  {
    pos++;
    if(pos == map->capacity) return NULL;
  }
  map->current = pos;
  return map->buckets[pos];
}
