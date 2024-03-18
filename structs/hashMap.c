#include "hashMap.h"

HashMap create_hashMap() {
    HashMap hm;
    hm.size = 0;
    hm.capacity = 256;
    hm.treshold = 0.6;
    hm.table = malloc(sizeof(HashNode*)*hm.capacity);
    for(int i=0; i<hm.capacity; i++) 
         hm.table[i] = NULL;
    
    return hm;
}

HashNode *create_hashNode(char *key, int value) {
    HashNode *node = malloc(sizeof(HashNode));
    node->value = value;
    strncpy(node->key, key, strlen(key)+1);
    node->next = NULL;

    return node;
}

void insert_hashMap(HashMap *hm, char *key, int value) {
    u32 idx = hash_func(key, hm->capacity);
    HashNode *node = create_hashNode(key, value);

    if(hm->table[idx] == NULL) {
        hm->table[idx] = node;
        printf("%s %d\n", hm->table[idx]->key, hm->table[idx]->value);
    } else {
        HashNode *ptr = hm->table[idx]; 
        while(ptr->next)
            ptr = ptr->next;

        ptr->next = node;
    }
}

int get_hashMap_value(HashMap *hm, char *key) {
    u32 idx = hash_func(key, hm->capacity);

    if(hm->table[idx] == NULL) {
        return -1;
    } else {
        HashNode *ptr = hm->table[idx]; 
        while(ptr && strncmp(key, ptr->key, strlen(ptr->key)) != 0)
            ptr = ptr->next;
            
        if(ptr)
            return ptr->value;
    }

    return -1;
}

void rehash(HashMap *hm) {
    hm->capacity *= 2;
    HashNode **new_table = malloc(sizeof(HashNode*)*hm->capacity);
    for(int i=0; i<hm->capacity; i++) 
         new_table[i] = NULL;
        
    HashNode **tmp = hm->table;
    hm->table = new_table;

    for(int i=0; i<hm->capacity/2; i++) {
        if(tmp[i] == NULL) continue;
        HashNode *ptr = tmp[i];
        while(ptr != NULL) {
            insert_hashMap(hm, ptr->key, ptr->value);
            ptr = ptr->next;   
        }
    }

    for(int i=0; i<hm->capacity/2; i++)
        free(tmp[i]);
    free(tmp);
}

void free_hashMap(HashMap *hm) {
    for(int i=0; i<hm->capacity; i++) {
        free(hm->table[i]);
    }
    free(hm->table);
}

void print_hashMap(HashMap *hm) {
    for(int i=0; i<hm->capacity; i++) {
        if(hm->table[i]) {
            HashNode *ptr = hm->table[i];
            printf("[%d] => {", i);
            while(ptr != NULL) {
                printf("(%s: %d) -> ", ptr->key, ptr->value);
                break;
                ptr = ptr->next;
            }
            printf("NULL}\n");
        }
    }
}

u32 hash_func(char *key, u32 capacity) {
    int len = strlen(key);
    u32 h = 5381;
    while(len--) {
        h += (h << 5);
        h ^= *key++;
    }

    u32 res = h % capacity;
    printf("%u %u %u\n", h, capacity, res);
    return res;
}