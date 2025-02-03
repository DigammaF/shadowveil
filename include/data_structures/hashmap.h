
#ifndef HASHMAP_H
#define HASHMAP_H

#define HASHMAP_GROWTH_RATE 1.1

/**
 * 
 *  pas une vraie hashmap au sens où il n'y a pas de hash,
 *  mais se manipule comme tel
 * 
 */
typedef struct {
    void** elements;
    unsigned capacity;
} hashmap_t;

void initHashmap(hashmap_t* hashmap);
void dropHashmap(hashmap_t* hashmap);

void* hashmapGet(hashmap_t* hashmap, unsigned key);
void hashmapSet(hashmap_t* hashmap, unsigned key, void* value);

unsigned hashmapLocateUnusedKey(hashmap_t* hashmap);

void increaseHashmapCapacity(hashmap_t* hashmap);

void dumpHashmap(hashmap_t* hashmap);

#endif
