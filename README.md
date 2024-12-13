# LRU Cache Implementation in C

## Functional Explanation

This project implements an efficient **Least Recently Used (LRU) Cache** algorithm in C. The LRU Cache is designed to store frequently accessed data while maintaining limited memory usage. When the cache reaches its capacity, it evicts the least recently accessed item to make space for new entries. This ensures optimal performance in memory-constrained environments.

The cache supports the following operations:
1. **Get(Key)**: Retrieves the value of a key if it exists in the cache; otherwise, returns `-1`.
2. **Put(Key, Value)**: Inserts or updates a key-value pair in the cache. If the cache is full, it removes the least recently used item before inserting the new one.

The implementation uses a **doubly linked list** and a **hash table** to achieve constant time complexity for these operations.

---

## Solution Description

### Design and Approach
1. **Doubly Linked List**: 
   - Maintains the order of usage, with the most recently used items at the front and the least recently used items at the back.
   - Provides `O(1)` insertion and deletion.

2. **Hash Table**:
   - Maps keys to their corresponding nodes in the doubly linked list.
   - Ensures `O(1)` access for retrieving or updating cache entries.

### Time and Space Complexity
- **Time Complexity**:
  - `Get(Key)`: `O(1)` - Direct access using the hash table and reordering via the doubly linked list.
  - `Put(Key, Value)`: `O(1)` - Involves updating the hash table and adjusting the doubly linked list.

- **Space Complexity**: `O(n)` for storing `n` key-value pairs in the hash table and doubly linked list.

### Why This Solution is Optimal
This design ensures that both operations—retrieval and insertion/updating—run in constant time, making it ideal for real-time applications where speed is critical. The combination of a hash table for quick lookups and a doubly linked list for maintaining order is the most efficient and commonly used approach for implementing an LRU Cache.

---

## Instructions for Cloning and Running the Code Locally


### Steps to Run
1. Clone the repository:
   ```bash
   git clone https://github.com/your-username/lru-cache-c.git
   cd lru-cache-c
   ```

2. Compile the code:
   ```bash
   gcc -o lru_cache lru_cache.c
   ```

3. Run the executable:
   ```bash
   ./lru_cache
   ```

### Example Usage
```c
LRUCache* cache = lRUCacheCreate(2); // Create a cache with capacity 2
lRUCachePut(cache, 1, 1);            // Insert key 1 with value 1
lRUCachePut(cache, 2, 2);            // Insert key 2 with value 2
printf("%d\n", lRUCacheGet(cache, 1)); // Returns 1
lRUCachePut(cache, 3, 3);            // Evicts key 2 and adds key 3
printf("%d\n", lRUCacheGet(cache, 2)); // Returns -1 (not found)
lRUCacheFree(cache);                 // Free the cache
```

---
