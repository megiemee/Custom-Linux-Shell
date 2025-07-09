#include "system_program.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/utsname.h>
#include <time.h>
#include <sys/resource.h>
#include <sys/sysinfo.h>
#include <pwd.h> 


const char *months[] = {
    "January", "February", "March", "April", "May", "June",
    "July", "August", "September", "October", "November", "December"
}; 


/*
 List all files matching the name in args[1] under current directory and subdirectories
*/
int execute(char **args)
{
    // get sys info
    struct utsname sys_info;
    
    // get current date and time
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    // get uptime
    struct sysinfo sys_info2;

    // get resource usaeg
    struct rusage usage;

    uid_t uid = getuid();
    struct passwd *pw = getpwuid(uid);
    if (pw) {
        printf("໒(⊙ᴗ⊙)७✎▤ %s\n", pw->pw_name);
    } else {
        perror("getpwuid");
        return 1;
    }

    

    // date & time
    printf("Date: %d %s %02d\n", tm.tm_mday, months[tm.tm_mon], tm.tm_year + 1900);
    if (tm.tm_hour > 12){
        printf("Time: %d:%02d:%02d PM\n", tm.tm_hour%12, tm.tm_min, tm.tm_sec);
    }
    else{
        printf("Time: %02d:%02d:%02d AM\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
    }

    // sys info
    if (uname(&sys_info) == 0) {
        printf("OS: %s ", sys_info.sysname);
        if (strncmp(sys_info.sysname, "Linux", 5) == 0){
            printf("🐧\n");
        } else if (strncmp(sys_info.sysname, "Apple", 5) == 0){
            printf("🍎\n");
        } else {
            printf("\n");
        }

        const char *home = getenv("HOME");
        printf("Home: %s 🎍\n", home ? home : "unknown");

        printf("Host: %s\n", sys_info.nodename);
        printf("Kernel: %s\n", sys_info.release);
        printf("Version: %s\n", sys_info.version);
        printf("Machine: %s\n", sys_info.machine);
          
    } else {
        perror("uname");
        return 1;
    }


    // uptime, free mem, mem usage
    if (sysinfo(&sys_info2) == 0) {
        unsigned int days = sys_info2.uptime / 86400;
        unsigned int hours = (sys_info2.uptime % 86400) / 3600;
        unsigned int minutes = (sys_info2.uptime % 3600) /60;
        printf("Uptime: %u days, %u hours, %u minutes\n", days, hours, minutes);
        


    } else {
        perror("sysinfo");
        return 1;
    }

    // mem

    
    FILE *fp = fopen("/proc/meminfo", "r");
    if (!fp) {
        perror("mem file read");
        return 1;

    } else {
        unsigned long mem_total = 0;
        unsigned long mem_free = 0;
        unsigned long mem_available = 0;
        unsigned long buffers = 0;
        unsigned long cached = 0;
        unsigned long shmem = 0;
        unsigned long sreclaimable = 0;

        typedef struct {
            const char *key;
            unsigned long *value;
        } MemInfoField;

        MemInfoField fields[] = {
        {"MemTotal:", &mem_total},
        {"MemFree:", &mem_free},
        {"MemAvailable:", &mem_available},
        {"Buffers:", &buffers},
        {"Cached:", &cached},
        {"Shmem:", &shmem},
        {"SReclaimable:", &sreclaimable}
        };

        void read_meminfo(FILE *fp, MemInfoField fields[], size_t n_fields) {
            char line[256];

            // read lines and match to variable
            while (fgets(line, sizeof(line), fp)) {
                for (size_t i = 0; i < n_fields; i++) {
                    size_t key_len = strlen(fields[i].key);
                    if (strncmp(line, fields[i].key, key_len) == 0) {
                        sscanf(line + key_len, " %lu kB", fields[i].value);
                        break;
                    }
                }
            }
        }

        read_meminfo(fp, fields, sizeof(fields)/sizeof(fields[0]));

        fclose(fp);


        printf("Memory Used: %ld / %ld kB\n", mem_total + shmem - mem_free - buffers - cached - sreclaimable, mem_total);
    }

    FILE *fp2 = fopen("/proc/cpuinfo", "r");
    if (!fp2) {
        perror("cpu file read");
        return 1;

    } else {

        char line[512];

        while (fgets(line, sizeof(line), fp2)) {
            if (strncmp(line, "model name", 10) == 0) {
                char *cpu_name = line + 17; // 17 is hardcoded :P

                printf("CPU: %s\n", cpu_name);
                break;
            }
        }


    }

    fclose(fp2);


    return EXIT_SUCCESS;

}

int main(int argc, char **args)
{
    return execute(args);
}