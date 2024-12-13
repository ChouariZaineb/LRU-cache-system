#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct Node {
    int key;
    int value;
    struct Node* prev;
    struct Node* next;
} Node;

typedef struct {
    int capacity;       
    int size;           
    Node* head;          
    Node* tail;         
    Node** hash_table;   
    int hash_size;       
} LRUCache;

int hash_function(int key, int hash_size) {
    return key % hash_size;
}

Node* create_node(int key, int value) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->key = key;
    new_node->value = value;
    new_node->prev = NULL;
    new_node->next = NULL;
    return new_node;
}

LRUCache* lru_cache_create(int capacity) {
    LRUCache* cache = (LRUCache*)malloc(sizeof(LRUCache));
    cache->capacity = capacity;
    cache->size = 0;
    cache->head = NULL;
    cache->tail = NULL;
    cache->hash_size = capacity * 2; 
    cache->hash_table = (Node**)calloc(cache->hash_size, sizeof(Node*));
    return cache;
}

void move_to_head(LRUCache* cache, Node* node) {
    if (cache->head == node) return; 
    if (node->prev) node->prev->next = node->next;
    if (node->next) node->next->prev = node->prev;
    if (node == cache->tail) cache->tail = node->prev; 

    node->prev = NULL;
    node->next = cache->head;
    if (cache->head) cache->head->prev = node;
    cache->head = node;

    if (cache->tail == NULL) cache->tail = node;
}

void remove_tail(LRUCache* cache) {
    if (cache->tail == NULL) return;
    Node* old_tail = cache->tail;

    if (old_tail->prev) old_tail->prev->next = NULL;
    cache->tail = old_tail->prev;

    
    int index = hash_function(old_tail->key, cache->hash_size);
    cache->hash_table[index] = NULL;

    free(old_tail);
    cache->size--;
}

int lru_cache_get(LRUCache* cache, int key) {
    int index = hash_function(key, cache->hash_size);
    Node* node = cache->hash_table[index];

    if (node == NULL) return -1; 


    move_to_head(cache, node);
    return node->value;
}

void lru_cache_put(LRUCache* cache, int key, int value) {
    int index = hash_function(key, cache->hash_size);
    Node* node = cache->hash_table[index];

    if (node != NULL) {
        node->value = value;
        move_to_head(cache, node);
    } else {
        Node* new_node = create_node(key, value);

        if (cache->size == cache->capacity) {
            remove_tail(cache);
        }

       
        new_node->next = cache->head;
        if (cache->head) cache->head->prev = new_node;
        cache->head = new_node;

        if (cache->tail == NULL) cache->tail = new_node;
        cache->hash_table[index] = new_node;
        cache->size++;
    }
}


void lru_cache_free(LRUCache* cache) {
    Node* current = cache->head;
    while (current != NULL) {
        Node* next = current->next;
        free(current);
        current = next;
    }
    free(cache->hash_table);
    free(cache);
}


int main() {
    LRUCache* cache = lru_cache_create(2); 

    lru_cache_put(cache, 1, 1);
    lru_cache_put(cache, 2, 2);
    printf("Get 1: %d\n", lru_cache_get(cache, 1)); 

    lru_cache_put(cache, 3, 3); 
    printf("Get 2: %d\n", lru_cache_get(cache, 2)); 

    lru_cache_put(cache, 4, 4); 
    printf("Get 1: %d\n", lru_cache_get(cache, 1)); 
    printf("Get 3: %d\n", lru_cache_get(cache, 3)); 
    printf("Get 4: %d\n", lru_cache_get(cache, 4)); 

    lru_cache_free(cache);
    return 0;
}
