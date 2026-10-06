#define _DEFAULT_SOURCE
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define EXTRA_SIZE 256
#define BLOCK_SIZE 128
#define BUF_SIZE 128

struct header {
    uint64_t size;
    struct header *next;
};

static void handle_error(const char *message) {
    perror(message);
    exit(EXIT_FAILURE);
}

void print_out(char *format, void *data, size_t data_size){
    char buf[BUF_SIZE];
    ssize_t len = snprintf(buf,BUF_SIZE,format, data_size == sizeof(uint64_t) ? *(uint64_t *)data : *(void ** ) data);

    if (len < 0) 
        handle_error("snprintf");
    
    write(STDOUT_FILENO, buf, len);
}

static void print_byte(unsigned char b) {
    char buf[BUF_SIZE];
    int len = snprintf(buf, BUF_SIZE, "%d\n", b);
    if (len < 0)
        handle_error("snprintf");
    
    write(STDOUT_FILENO,buf,len);
}

static void *increase_heap_size(size_t bytes) {
    void *start = sbrk(bytes);
    if (start == (void *)-1)
        handle_error("sbrk");
    
    return start;
}

static void initialize_block(struct header *block, uint64_t size, struct header *next, int value) {
    block->size = size;
    block->next = next;
    memset((char *)(block + 1), value, size - sizeof(struct header));
}

static void print_block_data(struct header *block) {
    unsigned char *data = (unsigned char *)(block + 1);
    for(uint64_t i = 0; i < block->size - sizeof(struct header); i++) {
        print_byte(data[i]);
    }
}

int main(void) {
    struct header *first = increase_heap_size(EXTRA_SIZE);
    struct header *second = (struct header *) ((char *)first + BLOCK_SIZE);

    initialize_block(first, BLOCK_SIZE, NULL,0);
    initialize_block(second, BLOCK_SIZE, first, 1);

    print_out("first block:       %p\n", &first, sizeof(&first));
    print_out("second block:      %p\n", &second, sizeof(&second));
    print_out("first block size:  %lu\n", &first->size, sizeof(first->size));
    print_out("first block next:  %p\n", &first->next, sizeof(first->next));
    print_out("second block size: %lu\n", &second->size, sizeof(second->size));
    print_out("second block next: %p\n", &second->next, sizeof(second->next));

    print_block_data(first);
    print_block_data(second);

    return 0;
}

