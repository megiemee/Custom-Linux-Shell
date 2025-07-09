#include <limits.h> // For PATH_MAX
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>


#define MAX_LINE 1024
#define MAX_ARGS 64
#define BIN_PATH "./bin/"
#define LANG_FILE "./lang.txt"

#define SHELL_NAME "MegShell"

#define RED "\x1b[1;31;40m"
#define WHITE "\x1b[37;40m"
#define BLUE "\x1b[1;34;40m"
#define CYAN "\x1b[36;40m"
#define GREEN "\x1b[32m"