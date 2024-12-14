#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Définition de la structure de chaque nœud de la liste doublement chaînée
typedef struct Node {
    int key;
    int value;
    struct Node* prev;
    struct Node* next;
} Node;

// Définition de la structure LRU Cache
typedef struct {
    int capacity;        // Capacité maximale du cache
    int size;            // Taille actuelle du cache
    Node* head;          // Pointeur vers la tête de la liste
    Node* tail;          // Pointeur vers la fin de la liste
    Node** hash_table;   // Table de hachage pour un accès rapide
    int hash_size;       // Taille de la table de hachage
} LRUCache;

// Fonction de hachage simple pour mapper une clé à un index
int hash_function(int key, int hash_size) {
    return key % hash_size;
}

// Fonction pour créer un nouveau nœud
Node* create_node(int key, int value) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->key = key;
    new_node->value = value;
    new_node->prev = NULL;
    new_node->next = NULL;
    return new_node;
}

// Initialisation de la structure LRU Cache
LRUCache* lru_cache_create(int capacity) {
    LRUCache* cache = (LRUCache*)malloc(sizeof(LRUCache));
    cache->capacity = capacity;
    cache->size = 0;
    cache->head = NULL;
    cache->tail = NULL;
    cache->hash_size = capacity * 2;  // Taille de la table de hachage
    cache->hash_table = (Node**)calloc(cache->hash_size, sizeof(Node*));
    return cache;
}

// Déplacement d'un nœud en tête de la liste
void move_to_head(LRUCache* cache, Node* node) {
    if (cache->head == node) return; // Déjà en tête
    // Déconnexion du nœud
    if (node->prev) node->prev->next = node->next;
    if (node->next) node->next->prev = node->prev;
    if (node == cache->tail) cache->tail = node->prev; // Mettre à jour la fin si nécessaire
    // Insérer en tête
    node->prev = NULL;
    node->next = cache->head;
    if (cache->head) cache->head->prev = node;
    cache->head = node;
    // Si la liste n'avait qu'un élément, mettre à jour la queue
    if (cache->tail == NULL) cache->tail = node;
}

// Suppression du nœud de la fin de la liste
void remove_tail(LRUCache* cache) {
    if (cache->tail == NULL) return;
    Node* old_tail = cache->tail;
    // Déconnexion du nœud
    if (old_tail->prev) old_tail->prev->next = NULL;
    cache->tail = old_tail->prev;
    // Suppression de la table de hachage
    int index = hash_function(old_tail->key, cache->hash_size);
    cache->hash_table[index] = NULL;
    free(old_tail);
    cache->size--;
}

// Récupération de la valeur associée à une clé
int lru_cache_get(LRUCache* cache, int key) {
    int index = hash_function(key, cache->hash_size);
    Node* node = cache->hash_table[index];
    if (node == NULL) return -1; // Clé non trouvée
    // Déplacer le nœud en tête de la liste
    move_to_head(cache, node);
    return node->value;
}

// Ajout ou mise à jour d'une paire clé-valeur
void lru_cache_put(LRUCache* cache, int key, int value) {
    int index = hash_function(key, cache->hash_size);
    Node* node = cache->hash_table[index];
    if (node != NULL) {
        // Mise à jour de la valeur existante
        node->value = value;
        move_to_head(cache, node);
    } else {
        // Ajouter un nouveau nœud
        Node* new_node = create_node(key, value);
        if (cache->size == cache->capacity) {
            // Supprimer l'élément le moins récemment utilisé
            remove_tail(cache);
        }
        // Ajouter en tête de la liste
        new_node->next = cache->head;
        if (cache->head) cache->head->prev = new_node;
        cache->head = new_node;
        // Mettre à jour la queue si nécessaire
        if (cache->tail == NULL) cache->tail = new_node;
        // Ajouter à la table de hachage
        cache->hash_table[index] = new_node;
        cache->size++;
    }
}

// Libération de la mémoire utilisée par le cache
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

// Fonction pour exécuter les commandes à partir d'un fichier
void execute_commands(LRUCache* cache, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    char command[10];
    int key, value;
    while (fscanf(file, "%s", command) != EOF) {
        if (strcmp(command, "put") == 0) {
            fscanf(file, "%d %d", &key, &value);
            lru_cache_put(cache, key, value);
        } else if (strcmp(command, "get") == 0) {
            fscanf(file, "%d", &key);
            int result = lru_cache_get(cache, key);
            if (result != -1) {
                printf("Get %d: %d\n", key, result);
            } else {
                printf("Get %d: -1 (non trouvé)\n", key);
            }
        }
    }
    fclose(file);
}

int main() {
    LRUCache* cache = lru_cache_create(2); // Capacité : 2

    // Exécute les commandes à partir du fichier
    execute_commands(cache, "test_data.txt");

    lru_cache_free(cache); // Libération de la mémoire
    return 0;
}