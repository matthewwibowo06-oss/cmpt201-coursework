#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

int main (void){
    char *buff = NULL;
    size_t size = 0;

    
    while (1) {
        printf("Please enter some text: ");

        if (getline(&buff,&size,stdin) == -1L)
            break;

        buff[strcspn(buff,"\n")] = '\0';
        
        char *input_str = buff; //store string
        char *delim = " "; //seperate token
        char *token = NULL; 
        char *saveptr = NULL; //tracking

        printf("Tokens: \n");

        while ((token = strtok_r(input_str, delim, &saveptr))){
            printf ("%s\n", token);
            input_str = NULL;
        }
    }

    free(buff);
    return 0;
}