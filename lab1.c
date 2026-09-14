#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

int main (void){
    char *buff = NULL;
    size_t size = 0;
    
    while (1) {
        if (getline(&buff,&size,stdin) != -1L){
            char *input_str = buff;
            char *delim = " ";
            char *token = NULL;
            char *saveptr = NULL;

        while ((token = strtok_r(input_str, delim, &saveptr))){
            printf ("Token : '%s'\n", token);

            input_str = NULL;
            }
        } else {
            printf ("Getline failure.\n");
        }
    }
    free(buff);
    return 0;
}