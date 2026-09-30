#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HISTORY_SIZE 5

//add copt od line to history buffer
static int add_history(char *history[], size_t *next, size_t *count, const char *line){
    char *copy = strdup(line);
    if (copy == NULL){
        return 0;
    }

    free(history[*next]);
    history[*next] = copy;

    *next = (*next + 1) % HISTORY_SIZE;
    if (*count < HISTORY_SIZE){
        ++(*count);
    }
    return 1;
}

static void print_history(char *history[], size_t next, size_t count){
    size_t first = (next + HISTORY_SIZE - count) % HISTORY_SIZE;

    for (size_t i = 0; i < count; i++){
        size_t index = (first + i) % HISTORY_SIZE;
        fputs(history[index],stdout);

        size_t length = strlen(history[index]);
        if (length == 0 || history[index][length-1] != '\n')
            putchar('\n');
    }
}

int main(void){
    char *history[HISTORY_SIZE] = {NULL};
    size_t next = 0;
    size_t count = 0;
    char *line = NULL;
    size_t line_capacity = 0;
    ssize_t line_length;

    while (1) {
        printf("Enter input: ");
        fflush(stdout);

        line_length = getline(&line, &line_capacity,stdin);
        if(line_length == -1)
            break;

        size_t commandlength = (size_t) line_length;
        if (commandlength > 0 && line[commandlength-1] == '\n')
            --commandlength;

        int is_print_command = commandlength == 5 && memcmp(line,"print",5) == 0;

        if (!add_history(history, &next, &count, line)){
            fprintf(stderr, "Error: could not allocate memory for history\n");
            free(line);

            for (size_t i = 0; i < HISTORY_SIZE; i++){
                free(history[i]);
            }
            return EXIT_FAILURE;
        }

        if (is_print_command)
            print_history(history,next,count);
    }

    free(line);
    for(size_t i = 0; i < HISTORY_SIZE;i++){
        free(history[i]);
    }

    return EXIT_SUCCESS;
}