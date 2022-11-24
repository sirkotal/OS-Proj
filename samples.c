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
#include <time.h>
#include <stdbool.h>

int main(int argc, char *argv[]) {
    if(argc < 4 || argc > 4) {
        printf("Couldn't execute the program\n\r");
        return EXIT_FAILURE;
    }

    FILE *fp;
    fp = fopen(argv[1],"r+");
    
    //char* p; -- used for strtoI
    //char* q;

    srand(time(0));
    int count = atoi(argv[2]);  //better than strtoI
    int c_count = atoi(argv[3]);
    char chr;

    int nums[count];

    for (int i = 0; i < count; i++) {
        int temp = rand() % sizeof(*fp) - c_count;
        bool exists = false;
        for (int j = 0; j < i; j++) {
            if (nums[j] == temp) {
                exists = true;
                break;
            }
        }

        /*if (temp < 0) {
            i--;
            continue;
        }*/

        if (!exists) {
            nums[i] = temp;
        }
        else {
            i--;
        }
    }

    /*for(int i=0; i<count; i++) {
     printf("%d ", nums[i]);
    }*/
    char starter = '>';
    char ender = '<';
    char nl = '\n';

    for (int i = 0; i < count; i++) {
        //char frag[c_count] ;
        //printf("%d", nums[i]);
        printf("%c", starter);
        fseek(fp, nums[i], 0);
        for (int j = 0; j < c_count; j++) {
            chr = fgetc(fp);
            if(feof(fp)) {
                break;
            }
            if (chr == '\r') {   // Windows specific
                j--;
                continue;
            }
            if (chr == '\n') {
                j--;
                continue;
            }
            printf("%c", chr);
        }
        printf("%c", ender);
        printf("%c", nl);
    }
}