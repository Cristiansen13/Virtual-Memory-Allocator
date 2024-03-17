#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "vma.h"

void write(arena_t *arena, const uint64_t address, const uint64_t size, int8_t *data)
{
    node_t *nodblock = arena->alloc_list->head; block_t *block;
    while (nodblock != NULL) {
        block = nodblock->data;
        if (block->start_address <= address && address <= block->start_address + block->size)
            break; 
        nodblock = nodblock->next;
    }
    if (nodblock == NULL) {
        printf("Invalid address for write.\n");
        return;
    }
    if (block->size + block->start_address - address < size) {
        printf("Warning: size was bigger than the block size. Writing %ld characters.\n", block->size + block->start_address - address);
    }
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
        info[i - mini->start_address + address] = data[i];
        i++;
    }
    nodeminiblock = nodeminiblock->next;
    while (i < block->size + block->start_address - address && i < size) {
        miniblock_t *miniblock = nodeminiblock->data;
        char *info = miniblock->rw_buffer;
        for (uint64_t j = 0; j < miniblock->size && (i + j) < size && (i + j) < block->size; j++)
            info[j] = data[i + j];
        i = i + miniblock->size;
        nodeminiblock = nodeminiblock->next;
    }
}

void inter_write(arena_t *arena)
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
    int8_t s; 
    scanf("%c", &s);
    int8_t *data = malloc(dimensiune); 
    fread(data, dimensiune, 1, stdin);   
    write(arena, adresa, dimensiune, data);
    free(data);
}
