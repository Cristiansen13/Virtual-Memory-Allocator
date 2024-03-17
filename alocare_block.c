#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>

#include "vma.h"

void alloc_block_normal(arena_t *arena, const uint64_t address, const uint64_t size, uint64_t n)
{
    add_nth_node(arena->alloc_list, n);
    node_t *node = arena->alloc_list->head;
    while (n > 0) {
        node = node->next;
        n--;
    }
    block_t* block = node->data;
    block->size = size;
    block->start_address = address;
    block->miniblock_list = ll_create(sizeof(miniblock_t));
    list_t *lista_miniblocuri = block->miniblock_list;
    add_nth_node(lista_miniblocuri, 0);
    miniblock_t *miniblock = lista_miniblocuri->head->data;
    miniblock->size = size;
    miniblock->start_address = address;
    miniblock->perm = 6;
    miniblock->rw_buffer = (char*)malloc(sizeof(char) * size);
    char *zona = miniblock->rw_buffer;
    zona[0] = (char)1000;
}

void alocare_bloc_head(arena_t *arena, const uint64_t address, const uint64_t size)
{
    node_t *curr = arena->alloc_list->head;
    block_t *block = curr->data;
    if (block->start_address == address + size) {
        add_nth_node(block->miniblock_list, 0);
        block->size = block->size + size;
        block->start_address = address;
        list_t *lista_miniblocuri = block->miniblock_list;
        miniblock_t *miniblock = lista_miniblocuri->head->data;
        miniblock->size = size;
        miniblock->start_address = address;
        miniblock->perm = 6;
        miniblock->rw_buffer = (char*)malloc(sizeof(char) * size);
        char *zona = miniblock->rw_buffer;
        zona[0] = (char)1000;
    } else {
        alloc_block_normal(arena, address, size, 0);
    }
}

void alocare_bloc_tail(arena_t *arena, const uint64_t address, const uint64_t size)
{
    node_t *curr = arena->alloc_list->head;
    while (curr->next != NULL) {
        curr = curr->next;
    }
    block_t *block = curr->data;
    if (block->start_address + block->size == address) {
        add_nth_node(block->miniblock_list, get_size(block->miniblock_list));
        block->size = block->size + size;
        list_t *lista_miniblocuri = block->miniblock_list;
        node_t *lastminiblock = lista_miniblocuri->head;
        while (lastminiblock->next != NULL) {
            lastminiblock = lastminiblock->next;
        }
        miniblock_t *miniblock = lastminiblock->data;
        miniblock->size = size;
        miniblock->start_address = address;
        miniblock->perm = 6;
        miniblock->rw_buffer = (char*)malloc(sizeof(char) * size);
        char *zona = miniblock->rw_buffer;
        zona[0] = (char)1000;
    } else {
        alloc_block_normal(arena, address, size, get_size(arena->alloc_list));
    }
}

void alloc_block(arena_t *arena, const uint64_t address, const uint64_t size)
{
    uint64_t n = 0;
    node_t *curr = arena->alloc_list->head, *prev = NULL; 
    while (curr != NULL) {
        block_t *block = (block_t*)curr->data;
        if (address < block->start_address)
            break;
        n++;
        prev = curr;
        curr = curr->next;
    }
    if (prev == NULL && curr != NULL) {
        alocare_bloc_head(arena, address, size);
    } else if (curr == NULL && prev != NULL) {
        alocare_bloc_tail(arena, address, size);
    } else if (curr == NULL && prev == NULL) {
        alloc_block_normal(arena, address, size, n);
    } else {
        block_t *block_spate = prev->data, *block_fata = curr->data;
        if (block_spate->size + block_spate->start_address == address) {
            add_nth_node(block_spate->miniblock_list, get_size(block_spate->miniblock_list));
            block_spate->size = block_spate->size + size;
            list_t *lista_miniblocuri = block_spate->miniblock_list;
            node_t *lastminiblock = lista_miniblocuri->head;
            while (lastminiblock->next != NULL) {
                lastminiblock = lastminiblock->next;
            }
            miniblock_t *miniblock = lastminiblock->data;
            miniblock->size = size;
            miniblock->start_address = address;
            miniblock->perm = 6;
            miniblock->rw_buffer = (char*)malloc(sizeof(char) * size);
            char *zona = miniblock->rw_buffer;
            zona[0] = (char)1000;
        } else if (block_fata->start_address == address + size) {
            add_nth_node(block_fata->miniblock_list, 0);
            block_fata->size = block_fata->size + size;
            block_fata->start_address = address;
            list_t *lista_miniblocuri = block_fata->miniblock_list;
            miniblock_t *miniblock = lista_miniblocuri->head->data;
            miniblock->size = size;
            miniblock->start_address = address;
            miniblock->rw_buffer = (char*)malloc(sizeof(char) * size);
            char *zona = miniblock->rw_buffer;
            zona[0] = (char)1000;
        } else {
            alloc_block_normal(arena, address, size, n);
        }
        if (block_spate->start_address + block_spate->size == block_fata->start_address) {
            list_t *minilist_spate = block_spate->miniblock_list, *minilist_fata = block_fata->miniblock_list;
            node_t *ultminiblock_spate = minilist_spate->head, *primulminiblock_fata = minilist_fata->head;
            while (ultminiblock_spate->next != NULL)
                ultminiblock_spate = ultminiblock_spate->next;
            ultminiblock_spate->next = primulminiblock_fata;
            primulminiblock_fata->prev = ultminiblock_spate;
            block_spate->size = block_spate->size + block_fata->size;
            list_t *lista_block_spate = block_spate->miniblock_list, *lista_block_fata = block_fata->miniblock_list;
            lista_block_spate->size = lista_block_spate->size + lista_block_fata->size;
            node_t *removed = remove_nth_node(arena->alloc_list, n);
            block_t *blockremoved = removed->data;
            list_t *listremoved = blockremoved->miniblock_list;
            free(listremoved);
            free(blockremoved);
            free(removed);
        }
    }
}

void inter_alloc_block(arena_t *arena)
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
    if (adresa > arena->arena_size) {
        printf("The allocated address is outside the size of arena\n");
        return;
    }
    if (adresa + dimensiune - 1 >= arena->arena_size) {
        printf("The end address is past the size of the arena\n");
        return;
    }
    uint64_t cnt = 0;
    node_t *curr = arena->alloc_list->head;
    while (curr != NULL) {
        block_t *block = (block_t*)curr->data;
        if (block->start_address <= adresa && block->start_address + block->size > adresa)
            cnt++;
        if (block->start_address < adresa + dimensiune && block->start_address + block->size > adresa + dimensiune)
            cnt++;
        if (block->start_address >= adresa && block->start_address + block->size <= adresa + dimensiune)
            cnt++;
        curr = curr->next;
    }
    if (cnt != 0) {
        printf("This zone was already allocated.\n");
    } else {
        alloc_block(arena, adresa, dimensiune);
    }
}

void free_block_mijloc(arena_t *arena, const uint64_t address)
{
    node_t *nodblock = arena->alloc_list->head; block_t *block; uint64_t n = 0;
    list_t *listminiblock; node_t *nodeminiblock = NULL; miniblock_t *miniblock;
    while (nodblock != NULL) {
        block = nodblock->data;
        listminiblock = block->miniblock_list;
        nodeminiblock = listminiblock->head;
        while (nodeminiblock != NULL) {
            miniblock = nodeminiblock->data;
            if (miniblock->start_address == address)
                break; 
            nodeminiblock = nodeminiblock->next;
        }
        if (nodeminiblock != NULL)
            break;
        nodblock = nodblock->next;
        n++;
    }
    uint64_t adr, size = 0, nr = 0;
    list_t *list = block->miniblock_list;
    node_t *node = list->head; miniblock_t *mini;
    while (node != NULL) {
        mini = node->data;
        size = size + mini->size;
        nr++;
        if (node == nodeminiblock) {
            node = node->next;
            mini = node->data;
            adr = mini->start_address;
            break;
        }
        node = node->next;
    }
    uint64_t size2 = block->size - size;
    block->size = size - miniblock->size;
    add_nth_node(arena->alloc_list, n + 1);

    block_t *newblock = nodblock->next->data;
    newblock->size = size2;
    newblock->start_address = adr;
    
    newblock->miniblock_list = ll_create(sizeof(miniblock_t));
    list_t *mlist = newblock->miniblock_list;
    mlist->size = list->size - nr;
    mlist->head = node;
    mlist->head->prev = NULL;
    list->size = nr;
    nodeminiblock->next = NULL;
    node_t *removed = remove_nth_node(block->miniblock_list, nr);
    miniblock_t *minib = removed->data;
    free(minib->rw_buffer);
    free(minib);
    free(removed);
}

void free_block(arena_t *arena, const uint64_t address)
{
    node_t *nodblock = arena->alloc_list->head; block_t *block;
    list_t *listminiblock; node_t *nodeminiblock = NULL; miniblock_t *miniblock;
    while (nodblock != NULL) {
        block = nodblock->data;
        listminiblock = block->miniblock_list;
        nodeminiblock = listminiblock->head;
        while (nodeminiblock != NULL) {
            miniblock = nodeminiblock->data;
            if (miniblock->start_address == address)
                break; 
            nodeminiblock = nodeminiblock->next;
        }
        if (nodeminiblock != NULL)
            break;
        nodblock = nodblock->next;
    }
    if (nodeminiblock == NULL) {
        printf("Invalid address for read.\n");
        return;
    }
    if (nodeminiblock->prev == NULL) {
        node_t *remove = remove_nth_node(listminiblock, 0);
        miniblock_t *miniblock = remove->data;
        block->size = block->size - miniblock->size;
        block->start_address = miniblock->start_address + miniblock->size;
        free(miniblock->rw_buffer);
        free(miniblock);
        free(remove);
    } else if (nodeminiblock->next == NULL) {
        node_t *remove = remove_nth_node(listminiblock, get_size(listminiblock));
        miniblock_t *miniblock = remove->data;
        block->size = block->size - miniblock->size;
        free(miniblock->rw_buffer);
        free(miniblock);
        free(remove);
    } else
        free_block_mijloc(arena, address);
    if (get_size(listminiblock) == 0) {
        node_t *curr = arena->alloc_list->head; uint64_t i = 0;
        while (curr != nodblock) {
            curr = curr->next;
            i++;
        }
        node_t *removed = remove_nth_node(arena->alloc_list, i);
        block_t *blockremoved = removed->data;
        free(blockremoved->miniblock_list);
        free(blockremoved);
        free(removed);
    }
}

void inter_free_block(arena_t *arena)
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
    free_block(arena, adresa);
}
