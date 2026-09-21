
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main (void) {
    char program[1024];
    while (1){
        printf("Enter programs to run: \n");
        fflush(stdout);

        if (fgets(program,sizeof(program), stdin) == NULL)
            break;

        program[strcspn(program, "\n")] = '\0';

        pid_t pid = fork();

        if (pid < 0){
            perror("fork");
            return EXIT_FAILURE;
        }

        if (pid == 0){
            execl(program,program, (char *)NULL);

            printf("Exec failure\n");
            fflush(stdout);
            _exit(EXIT_FAILURE);
        }

        if (waitpid(pid,NULL,0) < 0){
            perror("waitpid");
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}