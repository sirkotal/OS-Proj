#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <stdbool.h>
#include <time.h>

int main (int argc, char *argv[]) {
    /* check number of args */
    if(argc != 4) {
        printf("Couldn't execute the program\n");
        return EXIT_FAILURE;
    }

    char* pipe_name = malloc(sizeof(char)*50);
    int n_pipes = atoi(argv[1]);
    double chance = atof(argv[2]);
    int sleeper = atoi(argv[3]);

    int prob = chance*100;
    int msg = 0;


    for (int i = 1; i <= n_pipes; i++) {
        if(i == n_pipes)
            sprintf(pipe_name, "pipe%dto1", i);
        else
            sprintf(pipe_name, "pipe%dto%d", i, i+1);

        if ((mkfifo(pipe_name, 0666)) == -1) {
            fprintf(stderr, "mkfifo() Error %s", strerror(errno));
            exit(0);
        }
    }
    
    free(pipe_name);

    pid_t pids[n_pipes];

    char* write_pipe = malloc(sizeof(char)*50);
    char* read_pipe = malloc(sizeof(char)*50);

    for (int i = 1; i <= n_pipes; i++) {
        if ((pids[i-1] = fork()) < 0) {
            perror("fork");
            abort();
        } 
        else if (pids[i-1] == 0) {
            if (i == 1) {
                sprintf(write_pipe, "pipe%dto%d", i, i+1);
                sprintf(read_pipe, "pipe%dto1", n_pipes);
            } 
            else if (i == n_pipes) {
                sprintf(write_pipe, "pipe%dto1", i);
                sprintf(read_pipe, "pipe%dto%d", i-1, i);
            } 
            else {
                sprintf(write_pipe, "pipe%dto%d", i, i+1);
                sprintf(read_pipe, "pipe%dto%d", i-1, i);
            }

            srand(time(NULL) - (2*i));

            // array to store the pipes
            int pipeline[2];

            if (i == 1) {
                if ((pipeline[i] = open(write_pipe, O_WRONLY)) < 0) {
                    fprintf(stderr, "open() Error %s", strerror(errno));
                    exit(0);
                }

                msg++;

                if (write(pipeline[i], &msg, sizeof(int)) < 0) {
                    fprintf(stderr, "write() Error %s", strerror(errno));
                    exit(0);
                }

                close(pipeline[i]);
            }

            while(true) {

                // retrieves value from the previous process
                if ((pipeline[0] = open(read_pipe, O_RDONLY)) < 0) {
                    fprintf(stderr, "open() Error %s", strerror(errno));
                    exit(0);
                }

                if (read(pipeline[0], &msg, sizeof(int)) < 0) {
                    fprintf(stderr, "read() Error %s", strerror(errno));
                    exit(0);
                }

                close(pipeline[0]);

                msg++; 
                
                // randomizes chances for lock/unlock process
                int rand = random() % 100 - 1;

                if (rand == prob) {
                    printf("[p%d] lock on token (val = %d)\n", i, msg);
                    sleep(sleeper);
                    printf("[p%d] unlock token\n", i);
                }

                // writing the value of the message into the next process
                if((pipeline[1] = open(write_pipe, O_WRONLY)) < 0) {
                    fprintf(stderr, "open() Error %s", strerror(errno));
                    exit(0);
                }
                
                if(write(pipeline[1], &msg, sizeof(int)) < 0) {
                    fprintf(stderr, "write() Error %s", strerror(errno));
                    exit(0);
                }

                close(pipeline[1]);
            }

            exit(0);
        }
    }

    for (int i = 0; i < n_pipes; i++) {
        if(waitpid(pids[i], NULL, 0) < 0) {
            fprintf(stderr, "waitpid() Error %s", strerror(errno));
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}