#include "system_program.h"

char output_file_path[PATH_MAX];


static int daemon_work()
{

    int num = 0;
    FILE *fptr;
    char *cwd;
    char buffer[1024];

    // write PID of daemon in the beginning
    fptr = fopen(output_file_path, "a");
    if (fptr == NULL)
    {
        return EXIT_FAILURE;
    }

    fprintf(fptr, "Daemon process running with PID: %d, PPID: %d, opening logfile with FD %d\n", getpid(), getppid(), fileno(fptr));

    // then write cwd
    cwd = getcwd(buffer, sizeof(buffer));
    if (cwd == NULL)
    {
        perror("getcwd() error");
        return 1;
    }

    fprintf(fptr, "Current working directory: %s\n", cwd);
    fclose(fptr);

    while (1)
    {

        // use appropriate location if you are using MacOS or Linux
        fptr = fopen(output_file_path, "a");

        if (fptr == NULL)
        {
            return EXIT_FAILURE;
        }

        fprintf(fptr, "PID %d Daemon writing line %d to the file.  \n", getpid(), num);
        num++;

        fclose(fptr);

        sleep(10);

        if (num == 10) // we just let this process terminate after 10 counts
            break;
    }

    return EXIT_SUCCESS;
}

int execute(char **args)
{
    

    pid_t pid = fork();

    if (pid < 0){ // fork failed

        fprintf(stderr, "Fork has failed. Exiting now.");
        printf("Failed to execute command");
        return 1; // exit error

    } else if (pid > 0) { // parent process
        
        exit(1);

    } else { // child process
        setsid(); // make child session leader - lose controlling TTY
    }

    // ignore SIGCHLD - child of intermediate process does not become zombie when it terminates
    signal(SIGCHLD, SIG_IGN);
    // ignore SIGHUP - daemon (child of intermediate) is reaped once it exits
    // current intermdiate is a session leader - SIGHUP signal will kill child of session leader
    // calling the next line prevents daemon from being killed
    signal(SIGHUP, SIG_IGN);

    // Second fork to create daemon process - allow intermediate process to terminate
    // and ensures that child is not the session leader
    pid_t pid2 = fork();

    if (pid2 < 0){ // fork failed

        fprintf(stderr, "Fork has failed. Exiting now.");
        printf("Failed to execute command");
        return 1; // exit error

    } else if (pid2 > 0) { // parent process
        exit(1); // terminate parent

    } else { // child process
        pid_t ppid1 = getppid();

        // ensures all new files created by daemon has 0777 perms
        // all newly created files have all perms set
        // needed since daemon cannot be controlled anymore
        umask(0); 


        pid_t ppid2 = getppid();


    }

    // return to root
    chdir("/"); 

    /* Close all open file descriptors */
    int x;
    for (x = sysconf(_SC_OPEN_MAX); x>=0; x--)
    {
        close (x);
    }

    /*
    * Attach file descriptors 0, 1, and 2 to /dev/null. */


    int fd0 = open("/dev/null", O_RDWR);
    int fd1 = dup(0);
    int fd2 = dup(0);
    
    daemon_work();

    return EXIT_SUCCESS;
}

int main(int argc, char **args)
{
    if (getcwd(output_file_path, sizeof(output_file_path)) == NULL)
    {
        perror("getcwd() error, exiting now.");
        return 1;
    }
    strcat(output_file_path, "/dspawn.log"); 
    
    return execute(args);
}