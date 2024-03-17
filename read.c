#include <inttypes.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "vma.h"

void read(arena_t *arena, uint64_t address, uint64_t size)
{
    node_t *nodblock = arena->alloc_list->head; block_t *block;
    while (nodblock != NULL) {
        block = nodblock->data;
        if (block->start_address <= address && address <= block->start_address + block->size)
            break; 
        nodblock = nodblock->next;
    }
    if (nodblock == NULL) {
        printf("Invalid address for read.\n");
        return;
    }
    if (block->size + block->start_address - address < size)
        printf("Warning: size was bigger than the block size. Reading %ld characters.\n", block->size + block->start_address - address);
    uint64_t i = 0;
    list_t *listminiblock = block->miniblock_list;
    node_t *nodeminiblock = listminiblock->head; miniblock_t *mini = nodeminiblock->data;
    while (nodeminiblock != NULL) {
        miniblock_t *mini = nodeminiblock->data;
        if (mini->size + mini->start_address > address && mini->start_address <= address)
            break;
        nodeminiblock = nodeminiblock->next;
    }
    while (i < mini->size - (address - mini->start_address)) {
        char *info = mini->rw_buffer;
        printf("%c", info[i - mini->start_address + address]);
        i++;
    }
    nodeminiblock = nodeminiblock->next;
    while (i < block->size + block->start_address - address && i < size) {
        miniblock_t *miniblock = nodeminiblock->data;
        char *info = miniblock->rw_buffer;
        if (info[0] == (char)1000)
            break;
        for (uint64_t j = 0; j < miniblock->size && (i + j) < size && (i + j) < block->size; j++)
            printf("%c", info[j]);
        i = i + miniblock->size;
        nodeminiblock = nodeminiblock->next;
    }
    printf("\n");
}

void inter_read(arena_t *arena)
{
    if (arena == NULL) {
        printf("Invalid command. Please try again.\n");
        return;
    }
    char com[100];
    scanf("%s", com);
    if (atoi(com) == 0 && com[0] != '0') {
        printf("Invalid command. Please try again.\n");
        return;
    }
    uint64_t adresa = atoi(com);
    scanf("%s", com);
    if (atoi(com) == 0 && com[0] != '0'){
        printf("Invalid command. Please try again.\n");
        return;
    }
    uint64_t dimensiune = atoi(com);
    read(arena, adresa, dimensiune);
}
