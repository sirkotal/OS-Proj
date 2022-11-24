#include <sys/wait.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

char* removeSubstr (char *string, char *sub) {
    char *match = string;
    int len = strlen(sub);
    char extension[] = ".epub";
    while ((match = strstr(match, sub))) {
        *match = '\0';
        strcat(string, match+len);
                match++;
    }
    strcat(string, extension);

    return string;
}

int main(int argc, char *argv[]) {
    if(argc < 1){
        printf("Couldn't execute the program\n\r");
        return EXIT_FAILURE;
    }

    pid_t pids[argc-1];
    int i;
    int n = argc-1;

    /* Start children. */
    for (i = 0; i < argc-1; i++) {
        if ((pids[i] = fork()) < 0) {
            perror("fork");
            abort();
        } 
        else if (pids[i] == 0) {
            //printf("%d\n", i);
            size_t len = strlen(argv[i+1]);
            //printf("%zu\n", len);
            char command[] = "/usr/bin/pandoc";
            char output[] = "-o";
            char text[len];
            strcpy(text, argv[i+1]);  // text = file.txt
            char* copyOfText = strdup(text);
            removeSubstr(copyOfText, ".txt");
            //puts(text);
            //puts(copyOfText);
            execlp(command, command, text, output, copyOfText, NULL);
            exit(0);
        }
    }

    /* Wait for children to exit. */
    int status;
    pid_t pid;
    while (n > 0) {
        pid = wait(&status);
        printf("Child with PID %ld exited.\n", (long)pid);
        n--;  
    }
    char* ext[argc-1];
    char * ( *ptr )[argc-1] = &ext;
    for (int i = 0; i < argc-1; i++) {
        size_t len = strlen(argv[i+1]);
        char txt[len];
        strcpy(txt, argv[i+1]);
        char* carbon = strdup(txt);
        (*ptr)[i] = removeSubstr(carbon, ".txt");
    }
    char* finalzip[argc+4];
    finalzip[0] = "/usr/bin/zip";
    finalzip[1] = "book_archive";

    for (int x = 0; x < argc-1; x++) {
        finalzip[x+2] = malloc((strlen(ext[x])+1)*sizeof(char));
        strcpy(finalzip[x+2], ext[x]);
    }
    
    finalzip[argc+2] = "--quiet";
    finalzip[argc+3] = NULL;

    execv(finalzip[0], finalzip);

    return EXIT_SUCCESS;
}