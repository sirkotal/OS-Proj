#include <sys/stat.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

int main (int argc, char *argv[]) {
    // check number of args (must be 4)
    if(argc != 4) {
        printf("Couldn't execute the program\n");
        return EXIT_FAILURE;
    }

    char* pipe_name = malloc(60);   // allocates memory for the pipe names
    int n_pipes = atoi(argv[1]);                 // number of pipes
    double chance = atof(argv[2]);               // chance of locking
    int sleeper = atoi(argv[3]);                 // time it takes to unlock

    int prob = chance * 100;                     // 1 <= prob <= 100

    int msg = 0;                                 // token value


    for (int i = 1; i <= n_pipes; i++) {         // names every pipe
        if(i == n_pipes)
            sprintf(pipe_name, "pipe%dto1", i);
        else
            sprintf(pipe_name, "pipe%dto%d", i, i+1);

        if ((mkfifo(pipe_name, 0666)) == -1) {     // creates the FIFO special files (pipes)
            perror("mkfifo");
            exit(0);
        }
    }
    
    free(pipe_name);                               // free pipe name memory (making sure we have enough memory for read_pipe/write_pipe allocations)

    pid_t pids[n_pipes];                           // array of processes

    char* read_pipe = malloc(60);
    char* write_pipe = malloc(60);

    for (int i = 1; i <= n_pipes; i++) {                  // creates processes
        if ((pids[i-1] = fork()) < 0) {
            perror("fork");
            abort();
        } 
        else if (pids[i-1] == 0) {                       // identifies the write and read pipes
            if (i == 1) {
                sprintf(write_pipe, "pipe%dto%d", i, i+1);
                sprintf(read_pipe, "pipe%dto1", n_pipes);
            } 
            else if (i == n_pipes) {
                sprintf(read_pipe, "pipe%dto%d", i-1, i);
                sprintf(write_pipe, "pipe%dto1", i);
            } 
            else {
                sprintf(read_pipe, "pipe%dto%d", i-1, i);
                sprintf(write_pipe, "pipe%dto%d", i, i+1);
            }


            // array to store the pipes
            int pipeline[2];

            if (i == 1) {
                if ((pipeline[i] = open(write_pipe, O_WRONLY)) < 0) {
                    perror("open");
                    exit(0);
                }

                if (write(pipeline[i], &msg, sizeof(int)) < 0) {
                    perror("write");
                    exit(0);
                }

                close(pipeline[i]);
            }

            srand(time(NULL) - i);      // seed for random

            while(true) {

                // retrieves value from the previous process
                if ((pipeline[0] = open(read_pipe, O_RDONLY)) < 0) {
                    perror("open");
                    exit(0);
                }

                if (read(pipeline[0], &msg, sizeof(int)) < 0) {
                    perror("read");
                    exit(0);
                }

                close(pipeline[0]);

                msg++; 
                
                // randomizes chances for lock/unlock process
                int num = rand() % 100 + 1;

                if (num <= prob) {
                    printf("[p%d] lock on token (val = %d)\n", i, msg);
                    sleep(sleeper);
                    printf("[p%d] unlock token\n", i);
                }

                // writing the value of the message into the next process
                if((pipeline[1] = open(write_pipe, O_WRONLY)) < 0) {
                    perror("open");
                    exit(0);
                }
                
                if(write(pipeline[1], &msg, sizeof(int)) < 0) {
                    perror("write");
                    exit(0);
                }

                close(pipeline[1]);
            }

            exit(0);
        }
    }

    for (int i = 0; i < n_pipes; i++) {
        if(waitpid(pids[i], NULL, 0) < 0) {
            perror("waitpid");
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}