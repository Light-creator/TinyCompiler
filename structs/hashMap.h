#ifndef _HASH_MAP_H
#define _HASH_MAP_H

#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#include "../utils/types.h"

typedef struct _hashNode {
    int value;
    char *key;
    struct _hashNode *next;
} HashNode;

typedef struct _hashMap {
    u32 size;
    u32 capacity;
    double treshold;
    HashNode **table;
} HashMap;

HashMap create_hashMap();
HashNode *create_hashNode(char *key, int value);

void insert_hashMap(HashMap *hm, char *key, int value);
int get_hashMap_value(HashMap *hm, char *key);
void rehash(HashMap *hm);
void free_hashMap(HashMap *hm);

void print_hashMap(HashMap *hm);

u32 hash_func(char *key, u32 capacity);

#endif