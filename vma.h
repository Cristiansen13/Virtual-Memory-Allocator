#pragma once
#include <inttypes.h>
#include <stddef.h>

typedef struct node_t
{
    void* data;
    struct node_t* next, *prev;
} node_t;

typedef struct list_t
{
    node_t* head;
    unsigned int data_size;
    unsigned int size;
} list_t;

typedef struct {
	uint64_t start_address;
	size_t size;
	void *miniblock_list;
} block_t;

typedef struct {
	uint64_t start_address;
	size_t size;
	uint8_t perm;
	void *rw_buffer;
} miniblock_t;

typedef struct {
	uint64_t arena_size;
	list_t *alloc_list;
} arena_t;

arena_t *alloc_arena(const uint64_t size);
void dealloc_arena(arena_t *arena);
void inter_alloc_arena(arena_t **arena);


void alloc_block(arena_t *arena, const uint64_t address, const uint64_t size);
void free_block(arena_t *arena, const uint64_t address);
void inter_alloc_block(arena_t *arena);
void inter_free_block(arena_t *arena);

void read(arena_t *arena, uint64_t address, uint64_t size);
void write(arena_t *arena, const uint64_t address,  const uint64_t size, int8_t *data);
void inter_read(arena_t *arena);
void inter_write(arena_t *arena);

void pmap(const arena_t *arena);

list_t *ll_create(unsigned int data_size);
void add_nth_node(list_t* list, unsigned int n);
uint64_t get_size(list_t* list);
node_t *remove_nth_node(list_t* list, unsigned int n);
void ll_free(list_t** list);
