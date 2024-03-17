CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = -lm
FILES = main.c alocare_arena.c alocare_block.c read.c write.c pmap.c list_functions.c
TARGET = vma

.PHONY: clean

build: $(TARGET)

$(TARGET): $(FILES)
	$(CC) $(CFLAGS) $(LDFLAGS) -o vma $(FILES)

run_vma: $(TARGET)
	valgrind --leak-check=full ./$(TARGET)

clean:
	rm -f $(TARGET) *.o