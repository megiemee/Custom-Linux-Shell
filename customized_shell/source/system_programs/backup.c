#include "system_program.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>

// to be changed according to root path
#define ROOT_DIR "/home/meg/customized_shell/" 



/*
 List all files matching the name in args[1] under current directory and subdirectories
*/
int execute(char **args)
{
    char output_file_path[PATH_MAX];
    char archive_file_path[PATH_MAX];
    const char *name = "BACKUP_DIR";
    const char *env_p = getenv(name);

    char buffer[1024];

    

    if (env_p){
        // get current date & time
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        char timestamp[20];
        


        // format date time
        sprintf(timestamp, "%02d%02d%02d%02d%02d%02d", tm.tm_year+1900, tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec); // for ref on how to get date
        
        // format output file path
        char output_full_path[100];
        sprintf(output_full_path, "%sarchive/backup-%s.zip", ROOT_DIR, timestamp);
        

        // create archive folder if it does not exist
        struct stat info;
        // format archive file path
        sprintf(archive_file_path, "%sarchive", ROOT_DIR);

        if (stat(archive_file_path, &info) == 0 && S_ISDIR(info.st_mode)){
            printf("Archive folder already exists.\n");
        } else {
        if (mkdir(archive_file_path, 0755) == 0) {
            printf("Archive folder created successfully.\n");
        } else {
            printf("Archive folder creation failed");
            perror("Archive folder creation failed");
            return 1;
            }
        }
        

        // output file
        printf("Creating backup of '%s' at '%s'\n", env_p, output_full_path);

        // format sys command
        char syscmd[1024];
        sprintf(syscmd, "zip -r %s %s", output_full_path, env_p);

        // run zip shell command
        if (system(syscmd) == 0){
            printf("Backup created successfully ദ്ദി ˉ͈̀꒳ˉ͈́ )✧\n");
        } else {
            printf("Backup failed (っ- ‸ - ς)\nPlease retry.\n"); // change to red
            return 1;
        }


    } else {
        // no path for BACKUP_DIR found
        printf("BACKUP_DIR environment variable is not set. (っ- ‸ - ς)\n"); // change to red
        printf("setenv BACKUP_DIR=<file/folder_name> to set BACKUP_DIR environment variable.\n");
        return 1;
    }

    return EXIT_SUCCESS;

}

int main(int argc, char **args)
{
    
    return execute(args);
}