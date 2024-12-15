# LRU Cache Implementation in C

## Functional Explanation

This project implements an efficient **Least Recently Used (LRU) Cache** algorithm in C. The LRU Cache is designed to store frequently accessed data while maintaining limited memory usage. When the cache reaches its capacity, it evicts the least recently accessed item to make space for new entries. This ensures optimal performance in memory-constrained environments.

The cache supports the following operations:
1. **Get(Key)**: Retrieves the value of a key if it exists in the cache; otherwise, returns `-1`.
2. **Put(Key, Value)**: Inserts or updates a key-value pair in the cache. If the cache is full, it removes the least recently used item before inserting the new one.

The implementation uses a **doubly linked list** and a **hash table** to achieve constant time complexity for these operations.

---
## Input/Output Analysis

### Inputs
- **Capacity**: The maximum number of key-value pairs the cache can hold.
- **Key**: An integer that uniquely identifies a cache entry.
- **Value**: An integer value associated with the given key.
- **Operations**:
  - `Get(Key)`: Retrieve the value of a specific key.
  - `Put(Key, Value)`: Insert or update a key-value pair.

### Outputs
- **Get(Key)**: Returns the value associated with the given key or `-1` if the key does not exist in the cache.
- **Put(Key, Value)**: Updates the state of the cache without producing output.

### Constraints
1. The cache capacity is a positive integer.
2. Both keys and values are integers.
3. Operations must be optimized for constant (`O(1)`) time complexity.

## Proposed Solutions

### 1. Array-Based Approach
- **Description**: Maintain an array to store cache entries in order of usage.
- **Time Complexity**:
  - `Put(Key, Value)`: `O(n)` (requires finding and updating the least recently used item).
  - `Get(Key)`: `O(n)` (requires searching the array).
- **Space Complexity**: `O(n)`.
- **Drawback**: Linear time operations make it inefficient for large-scale use.

### 2. Doubly Linked List with Hash Table (Chosen Solution)
- **Description**: Utilize a doubly linked list to maintain the order of access and a hash table for quick lookups.
- **Time Complexity**:
  - `Put(Key, Value)`: `O(1)` (constant time for insertion/removal operations).
  - `Get(Key)`: `O(1)` (constant time for lookup and reordering).
- **Space Complexity**: `O(n)` (to store `n` key-value pairs).
- **Advantage**: Optimal performance for real-time systems due to constant time complexity for both operations.
  
   so for that we chose the second one because this design ensures that both operations—retrieval and insertion/updating—run in constant time, making it ideal for real-time applications where speed is critical.     The combination of a hash table for quick lookups and a doubly linked list for maintaining order is the most efficient and commonly used approach for implementing an LRU Cache.

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

## Why HashMap Access is O(1) and When It Can Be O(n)

### Why HashMap Access is O(1)
HashMaps achieve `O(1)` average-time complexity for accessing elements by using a **hashing mechanism**. When a key is provided:
1. The hash function computes an index in constant time based on the key.
2. The value is retrieved directly from the computed index in the underlying array.

This direct addressing ensures that both insertion and retrieval are efficient and independent of the size of the HashMap.

### When HashMap Access Can Be O(n)
In certain scenarios, the time complexity can degrade to `O(n)`. This happens when there are **hash collisions**, where multiple keys are mapped to the same index due to limitations in the hash function. In such cases:
1. The HashMap stores all colliding keys at the same index, typically using a linked list or a balanced tree.
2. To retrieve the correct value, the HashMap must search through all keys stored at that index.
   - If a linked list is used, the search time is proportional to the number of colliding keys, resulting in `O(n)` complexity.
   - If a balanced tree is used, the complexity is reduced to `O(log n)` for retrieval.

### How to Mitigate Collisions
1. **Use a Good Hash Function**: A well-designed hash function minimizes collisions by distributing keys uniformly.
2. **Rehashing**: When the load factor (ratio of the number of elements to the size of the hash table) exceeds a threshold, the HashMap expands and redistributes elements to reduce collisions.

In summary, while HashMaps offer `O(1)` average-case performance, their efficiency depends on the quality of the hash function and how collisions are handled.


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
   gcc -o waywa waywa.c
   ```

3. Run the executable:
   ```bash
   ./waywa
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
## GitHub Actions Summary

This repository includes a custom **GitHub Action** workflow to automate code quality checks and testing for the LRU Cache implementation. The workflow is defined in the `.github/workflows/c-test.yml` file and performs the following tasks:

1. **Code Compilation**:
   - Verifies that the code compiles successfully using GCC.

2. **Unit Testing**:
   - Executes a suite of unit tests to validate the correctness of the LRU Cache implementation.

3. **Static Analysis**:
   - Runs tools like `cppcheck` to ensure code quality and identify potential issues such as memory leaks or undefined behavior.

4. **Cross-Platform Testing**:
   - Tests the implementation on multiple operating systems (Linux, macOS, and Windows) to ensure compatibility
