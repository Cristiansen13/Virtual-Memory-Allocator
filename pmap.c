#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "vma.h"

void pmap(const arena_t *arena)
{
    printf("Total memory: 0x%lx bytes\n", arena->arena_size);
    uint64_t nrblockuri = 0, nrminiblockuri = 0, spatiu_ocupat = 0;
    list_t *listblock = arena->alloc_list;
    node_t *curr = listblock->head;
    while (curr != NULL) {
        block_t *block = curr->data;
        spatiu_ocupat = spatiu_ocupat + block->size;
        list_t *listminiblock = block->miniblock_list;
        node_t *miniblock_curr = listminiblock->head;
        while (miniblock_curr != NULL) {
            nrminiblockuri++;
            miniblock_curr = miniblock_curr->next;
        }
        nrblockuri++;
        curr = curr->next;
    }
    printf("Free memory: 0x%lx bytes\n", arena->arena_size - spatiu_ocupat);
    printf("Number of allocated blocks: %ld\n", nrblockuri);
    printf("Number of allocated miniblocks: %ld\n", nrminiblockuri);
    uint64_t nr = 1; curr = listblock->head;
    while (curr != NULL) {
        block_t *block = curr->data;
        printf("Block %ld begin\n", nr);
        printf("Zone: 0x%lx - 0x%lx\n", block->start_address, block->start_address + block->size);
        list_t *listminiblock = block->miniblock_list;
        node_t *miniblock_curr = listminiblock->head;
        uint64_t nrminiblock = 1;
        while (miniblock_curr != NULL) {
            miniblock_t *miniblock = miniblock_curr->data;
            printf("Miniblock %ld:\t0x%lx\t-\t0x%lx\t| RW-\n", nrminiblock, miniblock->start_address, miniblock->start_address + miniblock->size);        
            nrminiblock++;
            miniblock_curr = miniblock_curr->next;
        }
        printf("Block %ld end\n", nr);
        nr++;
        curr = curr->next;
    }
}

