#include "system_program.h"

#include <stdio.h>
#include <stdlib.h>





/*
 List all files matching the name in args[1] under current directory and subdirectories
*/
int execute(char **args)
{
    
    FILE *fp = popen("ps -efj | grep dspawn  | grep -Ev 'tty|pts' ", "r");
    if (!fp) {
        perror("ps file read");
        return 1;

    } else {

        char line[512];
        int appearances = 0;

        while (fgets(line, sizeof(line), fp)) {
            // printf("%s\n", line);
            appearances++;
        }

        printf("Live daemons: %d\n", appearances);


    }

    fclose(fp);


    return EXIT_SUCCESS;

}

int main(int argc, char **args)
{
    return execute(args);
}