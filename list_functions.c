#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "vma.h"

#define MAX_STRING_SIZE 64

list_t *ll_create(unsigned int data_size)
{
    list_t* list;

    list = malloc(sizeof(*list));

    list->head = NULL;
    list->data_size = data_size;
    list->size = 0;

    return list;
}

void add_nth_node(list_t* list, unsigned int n)
{
    node_t *prev, *curr;
    node_t* new_node;

    if (!list) {
        return;
    }
    if (n > list->size) {
        n = list->size;
    }

    curr = list->head;
    prev = NULL;
    while (n > 0) {
        prev = curr;
        curr = curr->next;
        --n;
    }

    new_node = malloc(sizeof(*new_node));
    new_node->data = malloc(list->data_size);
    
    new_node->next = curr;
    new_node->prev = prev;
    if (prev == NULL) {
        list->head = new_node;
    } else {
        prev->next = new_node;
    }
    if (curr != NULL)
        curr->prev = new_node;

    list->size++;
}

node_t *remove_nth_node(list_t* list, unsigned int n)
{
    node_t *prev, *curr;

    if (!list || !list->head) {
        return NULL;
    }

    if (n > list->size - 1) {
        n = list->size - 1;
    }

    curr = list->head;
    prev = NULL;
    while (n > 0) {
        prev = curr;
        curr = curr->next;
        --n;
    }
    
    if (prev == NULL) {
        list->head = curr->next;
        if (curr->next != NULL)
            curr->next->prev = NULL;
    } else {
        prev->next = curr->next;
    }
    if (curr->next != NULL)
        curr->next->prev = prev;
    list->size--;

    return curr;
}

uint64_t get_size(list_t* list)
{
     if (!list) {
        return -1;
    }
    uint64_t n = 0;
    node_t *curr = list->head;
    while (curr != NULL) {
        n++;
        curr = curr->next;
    }
    return n;
}

void ll_free(list_t** list)
{
    node_t* currNode;

    if (!list || !*list) {
        return;
    }

    while (get_size(*list) > 0) {
        currNode = remove_nth_node(*list, 0);
        free(currNode->data);
        currNode->data = NULL;
        free(currNode);
        currNode = NULL;
    }

    free(*list);
    *list = NULL;
}
