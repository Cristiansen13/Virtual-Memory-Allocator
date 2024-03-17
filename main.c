#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "vma.h"

int main(void) {
    char command[100]; arena_t *arena = NULL;
    scanf("%s", command);
    while (strcmp(command, "DEALLOC_ARENA") != 0) {
        if (strcmp(command, "ALLOC_ARENA") == 0) {
            inter_alloc_arena(&arena);
        } else if (strcmp(command, "ALLOC_BLOCK") == 0) {
            inter_alloc_block(arena);
        } else if (strcmp(command, "FREE_BLOCK") == 0) {
            inter_free_block(arena);
        } else if (strcmp(command, "READ") == 0) {
            inter_read(arena);
        } else if (strcmp(command, "WRITE") == 0) {
            inter_write(arena);
        } else if (strcmp(command, "PMAP") == 0) {
            pmap(arena);
        } else {
            printf("Invalid command. Please try again.\n");
        }
        scanf("%s", command);
    }
    dealloc_arena(arena);
    return 0;
}
