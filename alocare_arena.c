#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vma.h"

arena_t *alloc_arena(const uint64_t size)
{
    arena_t *arena;
    arena = malloc(sizeof(*arena));

    arena->arena_size = size;
    arena->alloc_list = ll_create(sizeof(block_t));
    return arena;
}

void dealloc_arena(arena_t *arena)
{
    node_t *nodblock = arena->alloc_list->head;
    while (nodblock != NULL) {
        block_t *block = nodblock->data;
        list_t *lista_miniblocuri = block->miniblock_list;
        node_t *nodminiblock = lista_miniblocuri->head;
        while (nodminiblock != NULL) {
            miniblock_t *miniblock = nodminiblock->data;
            free(miniblock->rw_buffer);
            nodminiblock = nodminiblock->next;
        }
        list_t *list = block->miniblock_list;
        ll_free(&list);
        nodblock = nodblock->next;
    }
    list_t *list = arena->alloc_list;
    ll_free(&list);

    free(arena);
}

void inter_alloc_arena(arena_t **arena)
{
    char com[100];
    scanf("%s", com);
    if (atoi(com) == 0 && com[0] != '0') {
        printf("Invalid command. Please try again.\n");
        return;
    } else {
        uint64_t size = atoi(com);
        *arena = alloc_arena(size);
    }
    
}
