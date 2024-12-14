#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the structure for each node in the doubly linked list
typedef struct Node {
    int key;                 // Key associated with the data
    int value;               // Value associated with the data
    struct Node* prev;       // Pointer to the previous node
    struct Node* next;       // Pointer to the next node
} Node;

// Define the structure for the LRU Cache
typedef struct {
    int capacity;            // Maximum number of items the cache can hold
    int size;                // Current number of items in the cache
    Node* head;              // Pointer to the most recently used item
    Node* tail;              // Pointer to the least recently used item
    Node** hash_table;       // Hash table for fast key lookup
    int hash_size;           // Size of the hash table
} LRUCache;

// Simple hash function to map a key to an index in the hash table
int hash_function(int key, int hash_size) {
    return key % hash_size; // Compute index using modulo operator
}

// Function to create a new node for the doubly linked list
Node* create_node(int key, int value) {
    Node* new_node = (Node*)malloc(sizeof(Node)); // Allocate memory for the new node
    new_node->key = key;                          // Set the key
    new_node->value = value;                      // Set the value
    new_node->prev = NULL;                        // Initialize previous pointer to NULL
    new_node->next = NULL;                        // Initialize next pointer to NULL
    return new_node;                              // Return the created node
}

// Function to initialize the LRU Cache
LRUCache* lru_cache_create(int capacity) {
    LRUCache* cache = (LRUCache*)malloc(sizeof(LRUCache)); // Allocate memory for the cache
    cache->capacity = capacity;                           // Set the capacity
    cache->size = 0;                                      // Initialize size to 0
    cache->head = NULL;                                   // Initialize head pointer to NULL
    cache->tail = NULL;                                   // Initialize tail pointer to NULL
    cache->hash_size = capacity * 2;                      // Set hash table size to twice the capacity
    cache->hash_table = (Node**)calloc(cache->hash_size, sizeof(Node*)); // Allocate and initialize the hash table
    return cache;                                         // Return the initialized cache
}

// Function to move a node to the head of the doubly linked list
void move_to_head(LRUCache* cache, Node* node) {
    if (cache->head == node) return; // If the node is already the head, do nothing

    // Disconnect the node from its current position
    if (node->prev) node->prev->next = node->next; // Update the previous node's next pointer
    if (node->next) node->next->prev = node->prev; // Update the next node's previous pointer

    // If the node is the tail, update the tail pointer
    if (node == cache->tail) cache->tail = node->prev;

    // Insert the node at the head of the list
    node->prev = NULL;          // Set the previous pointer to NULL
    node->next = cache->head;   // Set the next pointer to the current head
    if (cache->head) cache->head->prev = node; // Update the old head's previous pointer
    cache->head = node;         // Update the head pointer to the new node

    // If the list was empty, update the tail pointer
    if (cache->tail == NULL) cache->tail = node;
}

// Function to remove the node at the tail of the doubly linked list
void remove_tail(LRUCache* cache) {
    if (cache->tail == NULL) return; // If the list is empty, do nothing

    Node* old_tail = cache->tail;    // Get the current tail node

    // Disconnect the tail node
    if (old_tail->prev) old_tail->prev->next = NULL; // Update the previous node's next pointer
    cache->tail = old_tail->prev;     // Update the tail pointer

    // Remove the node from the hash table
    int index = hash_function(old_tail->key, cache->hash_size); // Compute the hash index
    cache->hash_table[index] = NULL; // Remove the node from the hash table

    free(old_tail);                   // Free the memory for the old tail node
    cache->size--;                    // Decrease the cache size
}

// Function to get the value associated with a key from the cache
int lru_cache_get(LRUCache* cache, int key) {
    int index = hash_function(key, cache->hash_size); // Compute the hash index for the key
    Node* node = cache->hash_table[index];            // Retrieve the node from the hash table

    if (node == NULL) return -1; // If the node is not found, return -1

    // Move the accessed node to the head of the list (marking it as recently used)
    move_to_head(cache, node);
    return node->value;         // Return the value associated with the key
}

// Function to add or update a key-value pair in the cache
void lru_cache_put(LRUCache* cache, int key, int value) {
    int index = hash_function(key, cache->hash_size); // Compute the hash index for the key
    Node* node = cache->hash_table[index];            // Retrieve the node from the hash table

    if (node != NULL) {
        // If the key already exists, update its value
        node->value = value; // Update the value
        move_to_head(cache, node); // Move the node to the head of the list
    } else {
        // If the key does not exist, create a new node
        Node* new_node = create_node(key, value); // Create a new node

        if (cache->size == cache->capacity) {
            // If the cache is full, remove the least recently used item
            remove_tail(cache);
        }

        // Add the new node to the head of the list
        new_node->next = cache->head;           // Set the new node's next pointer to the current head
        if (cache->head) cache->head->prev = new_node; // Update the old head's previous pointer
        cache->head = new_node;                // Update the head pointer to the new node

        // If the list was empty, update the tail pointer
        if (cache->tail == NULL) cache->tail = new_node;

        // Add the new node to the hash table
        cache->hash_table[index] = new_node;
        cache->size++; // Increase the cache size
    }
}

// Function to free the memory used by the cache
void lru_cache_free(LRUCache* cache) {
    Node* current = cache->head;

    // Free all nodes in the doubly linked list
    while (current != NULL) {
        Node* next = current->next; // Get the next node
        free(current);              // Free the current node
        current = next;             // Move to the next node
    }

    free(cache->hash_table); // Free the hash table
    free(cache);             // Free the cache structure
}

// Function to execute commands from a file
void execute_commands(LRUCache* cache, const char* filename) {
    FILE* file = fopen(filename, "r"); // Open the file in read mode
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n"); // Error opening file
        return;
    }

    char command[10]; // Buffer to store the command
    int key, value;

    // Read and execute commands from the file
    while (fscanf(file, "%s", command) != EOF) {
        if (strcmp(command, "put") == 0) {
            // If the command is "put", read the key and value and add them to the cache
            fscanf(file, "%d %d", &key, &value);
            lru_cache_put(cache, key, value);
        } else if (strcmp(command, "get") == 0) {
            // If the command is "get", read the key and retrieve its value from the cache
            fscanf(file, "%d", &key);
            int result = lru_cache_get(cache, key);

            // Print the result
            if (result != -1) {
                printf("Get %d: %d\n", key, result);
            } else {
                printf("Get %d: -1 (non trouv\u00e9)\n", key);
            }
        }
    }

    fclose(file); // Close the file
}

int main() {
    LRUCache* cache = lru_cache_create(2); // Create a cache with capacity 2

    // Execute commands from the file "test_data.txt"
    execute_commands(cache, "test_data.txt");

    lru_cache_free(cache); // Free the memory used by the cache
    return 0;
}
