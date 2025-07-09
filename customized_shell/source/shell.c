// Include the shell header file for necessary constants and function declarations
#include "shell.h"
#include <sys/resource.h>
#define history_log 5

char *history[history_log];
int history_count = 0;
// to control language used
int lang_id = 0;

char language[MAX_LINE];
char langpath[PATH_MAX];

// Used to formulate the full path of the command to be executed
char full_path[PATH_MAX]; 

char root_directory[1024];


// supported languages
const char *supported_languages[] = {
"english",    
"chinese"
};

// builtin commands
const char *builtin_commands[] = {
"cd",    // Changes the current directory of the shell to the specified path. If no path is given, it defaults to the user's home directory.
"help",  //  List all builtin commands in the shell
"exit",  // Exits the shell
"usage", // Provides a brief usage guide for the shell and its built-in command
"env", // Lists all the environment variables currently set in the shell
"setenv", // Sets or modifies an environment variable for this shell session
"unsetenv", // Removes an environment variable from the shell
"lang", // Changes language - additional builtin_command
"history"
};

// builtin command descriptions
const char *builtin_desc[] = {
"cd <directory_name> to change the current working directory of the shell",    // Changes the current directory of the shell to the specified path. If no path is given, it defaults to the user's home directory.
"help for supported commands",  //  List all builtin commands in the shell
"exit to terminate the cell gracefully",  // Exits the shell
"usage <built-in command> to get brief usage guide of built-in command", // Provides a brief usage guide for the shell and its built-in command
"env to list all registered env variables", // Lists all the environment variables currently set in the shell
"setenv ENV=VALUE to set a new env variable", // Sets or modifies an environment variable for this shell session
"unsetenv ENV to remove this env from the list of env variables", // Removes an environment variable from the shell
"lang <language> to change language used for shell prompts", // change language used for shell prompts
"history to view the 5 most recently used commands"
};

// builtin command descriptions
const char *builtin_desc_chinese[] = {
"cd <目录名字> 换当前工作目录",    // Changes the current directory of the shell to the specified path. If no path is given, it defaults to the user's home directory.
"help 查看支持的内置命令",  //  List all builtin commands in the shell
"exit 优雅地终止进程",  // Exits the shell
"usage <内置命令> 看内置命令的用处", // Provides a brief usage guide for the shell and its built-in command
"env 查看所有已注册的环境变量", // Lists all the environment variables currently set in the shell
"setenv ENV=<VALUE> 设置新的环境变量", // Sets or modifies an environment variable for this shell session
"unsetenv ENV 清除环境变量", // Removes an environment variable from the shell
"lang <语言> 换语言",
"history 查看最近使用的 5 条命令" 
};



// This program will list all the environment variables available to it at runtime
#include <stdio.h>

// Declaration of the external variable 'environ'
// environ is actually defined in the C standard library
extern char **environ; 

// 2 = success for built-in

/*
Handler of each shell builtin function
*/
int shell_cd(char **args){ // to do
  if (args[1] != NULL){
    if (chdir(args[1]) == 0){
      return 2;
    }
  }
  // if no/non-valid dir/file given 
  printf("%s", RED);
  if (lang_id == 1){
    printf("❌ 文件或目录不存在。\n");
    printf("提示:\n"); 
    printf("用 cd .. 换去上级目录.\n"); 
    printf("用 ls 查看这目录的内容.\n"); 
  }
  else {
    printf("❌ No such file or directory\n"); 
    printf("Tips:\n"); // light red
    printf("Use cd .. to go to parent directory.\n"); 
    printf("Use ls to check contents of current directory.\n"); 
  }
  printf("%s", WHITE);

  return 2;
};
int shell_help(char **args){
  if (lang_id == 1){
    printf("%s使用方法: command arguments\n 此 shell 支持以下命令:\n", WHITE); 
  } else {
    printf("%sUsage: command arguments\n The following commands are implemented within the shell:\n", WHITE); 
  }
  
  for (int command_index = 0; command_index < sizeof(builtin_commands) / sizeof(char *); command_index++)
      {
          printf(" %s\n", builtin_commands[command_index]); 
      }
  
  return 2;
};

int shell_history(char **args) {
    if (history_count == 0){
      if (lang_id == 1){
        printf("%s❌ 还没用任何命令.%s\n", RED, WHITE);
      } else {
        printf("%s❌ No commands have been used yet.%s\n", RED, WHITE);
      }
    }
    for (int i = 0; i < history_count; i++) {
        printf("%d %s\n", i + 1, history[i]);  
    }
    return 2;
}

int shell_exit(char **args){ 
  return 0;
};

int shell_usage(char **args){ 

  // check if command name was given in second word of cmd
  if (args[1] == NULL){
    if (lang_id == 1){
      printf("%s❌ 未输入指令.\n要查看某个指令的作用，请输入：usage <指令>\n%s", RED, WHITE); 
    } else {
      printf("%s❌ Command not given.\nTo check what a command does, type usage <command>%s", RED, WHITE); 
    }
    
  }
  else
  { 
    // print according to command - todo: create funciton for this
    for (int command_index = 0; command_index < sizeof(builtin_commands) / sizeof(char *); command_index++)
      {
          if (strcmp(args[1], builtin_commands[command_index]) == 0){ // cmd[1] contains the second word of the command
            if (lang_id == 1){
              printf("\x1b[37;40m输入: %s\n", builtin_desc_chinese[command_index]); 
            } else{
              printf("\x1b[37;40mType: %s\n", builtin_desc[command_index]); 
            }
            
            return 2;
          }
       
      }
    // else print not present message
    
    if (lang_id == 1){
      printf("%s❌ 你输入的指令: %s, 不是 %s 的内建指令.\n", RED, args[1], SHELL_NAME);
      printf("要查看内建指令列表，请输入 <help>.%s\n", WHITE);
    } else{
      printf("%s❌ The command you gave: %s, is not part of %s's builtin commands.\n", RED, args[1], SHELL_NAME);
      printf("For list of builtin commands, type <help>.%s\n", WHITE);
    }
    
  }

  return 2;

};

int list_env(char **args){ 
  char **env = environ; // Pointer to the array of environment strings
    
  while (*env) { // Loop until NULL pointer is encountered
      printf("%s\n", *env); // Print the current environment variable
      env++; // Move to the next environment variable
  }

  return 2;

};
int set_env_var(char **args){ 


  if (args[1] != NULL){
    // Temporarily split into key and value

    // Expecting "KEY=VALUE", so split it
    char *equal_sign = strchr(args[1], '=');

    if (!equal_sign) { // do not set if incorrect format
      
      return 2;
    }

    *equal_sign = '\0';
    char *key = args[1];
    char *value = equal_sign + 1;

    setenv(key, value, 1);

    
  }

  return 2;
};
int unset_env_var(char **args){ // to do

  if (args[1] != NULL){
    unsetenv(args[1]);
  }

  return 2;
};

int set_language(char **args){ 

  // read saved language
  if (args[1] == NULL){

    if (lang_id == 1){
      printf("%s❌ 未指定语言. 要更改语言，请输入 <language>.%s\n", RED, WHITE);
    } else{
      printf("%s❌ Language not given. To change language, type lang <language>.%s\n", RED, WHITE);
    }
    
  } else {
    for (int lang_index = 0; lang_index < sizeof(supported_languages) / sizeof(char *); lang_index++)
    {
      if (strcmp(args[1], supported_languages[lang_index]) == 0){ // update language if its valid
        // update current language id
        lang_id = lang_index;
        // write to file
        FILE *lang_filew;
        snprintf(langpath, sizeof(langpath), LANG_FILE); // lang_file defined in header
        lang_filew = fopen(langpath, "w");
        if (!lang_filew) {
          perror("failed to update lang file");
        } else {
          fputs(supported_languages[lang_index],lang_filew);
        }
        fclose(lang_filew);

        return 2;
      }
      
    }
    if (lang_id == 1){
      printf("%s❌ 不支持 %s 语言.\n当前支持的语言有:\n1. english\n 2. chinese\n%s", RED, args[1], WHITE); 
    } else{
      printf("%s❌ Language %s not supported.\nCurrent supported languages:\n1. english\n 2. chinese\n%s", RED, args[1], WHITE); 
    }
    
    return 2;

  }
}
  

/*** This is array of functions, with argument char ***/
      int (*builtin_command_func[])(char **) = {
          &shell_cd,     // builtin_command_func[0]: cd 
          &shell_help,   // builtin_command_func[1]: help
          &shell_exit,   // builtin_command_func[2]: exit
          &shell_usage,  // builtin_command_func[3]: usage
          &list_env,     // builtin_command_func[4]: env
          &set_env_var,  // builtin_command_func[5]: setenv
          &unset_env_var, // builtin_command_func[6]: unsetenv
          &set_language, // builtin_command_func[7]: lang
          &shell_history
      };

// function to check if cmd is a builtin func
// input: word being considered
// output: return 0 if it is, 1 if it is not
int is_builtin(char *arg){
  for (int command_index = 0; command_index < sizeof(builtin_commands) / sizeof(char *); command_index++)
    {
      if (strcmp(arg, builtin_commands[command_index]) == 0){ // args contains the word being considerd
        return 0;
      }
      
    }
  return 1;
}

// Helper function to figure out how many builtin commands are supported by the shell
int num_builtin_functions()
{
    return sizeof(builtin_commands) / sizeof(char *);
};

// helper function to run builtin AFTER checking if command entered is a builtin command
int execute_builtin(char **cmd)
{
    for (int command_index = 0; command_index < num_builtin_functions(); command_index++)
        {
          if (strcmp(cmd[0], builtin_commands[command_index]) == 0) // cmd[0] contains the first word of the command
          {
            if ((*builtin_command_func[command_index])(cmd) == 0){
              return 0; // exit shell if req
            }
          } 
          
      }

    return 1;
};

// Function to read a command from the user input
void read_command(char **cmd) {
  // Define a character array to store the command line input
  char line[MAX_LINE];
  // Initialize count to keep track of the number of characters read
  int count = 0, i = 0;
  // Array to hold pointers to the parsed command arguments
  char *array[MAX_ARGS], *command_token;

  // Infinite loop to read characters until a newline or maximum line length is
  // reached
  for (;;) {
    // Read a single character from standard input
    int current_char = fgetc(stdin);
    // Store the character in the line array and increment count
    line[count++] = (char)current_char;
    // If a newline character is encountered, break out of the loop
    if (current_char == '\n')
      break;
    // If the command exceeds the maximum length, print an error and exit
    if (count >= MAX_LINE) {
      if (lang_id == 1){
        printf("%s❌ 指令过长, 无法处理. \n%s", RED, WHITE); 
      } else{
        printf("%s❌ Command is too long, unable to process%s\n", RED, WHITE);  
      }
      
      exit(1);
    }
  }
  // Null-terminate the command line string
  line[count] = '\0';

  // If only the newline character was entered, return without processing
  if (count == 1)
    return;

  // Use strtok to parse the first token (word) of the command
  command_token = strtok(line, " \n");

  // Continue parsing the line into words and store them in the array
  while (command_token != NULL) {
    array[i++] = strdup(command_token);  // Duplicate the token and store it
    command_token = strtok(NULL, " \n"); // Get the next token
  }

  // Copy the parsed command and its parameters to the cmd array
  for (int j = 0; j < i; j++) {
    cmd[j] = array[j];
  }
  // Null-terminate the cmd array to mark the end of arguments
  cmd[i] = NULL;

  // history recording
  if (cmd[0] && strcmp(cmd[0], "history") != 0) {
    if (history_count == history_log) {
      free(history[0]);

      for (int i = 1; i < history_log; i++) {
        history[i - 1] = history[i];
      }

    history_count--;  
}

history[history_count++] = strdup(line);
}
}


// parameters: pointer to the line of cmd, empty array of strings to store words in cmd
void parse_line(char *line, char **args) {
    char *token = strtok(line, " \t"); //split the words in the line in-place 
    int i = 0;
    while (token != NULL && i < MAX_ARGS) {
        args[i++] = token; // add token to args
        token = strtok(NULL, " \t\n"); // continue parsing line
    }
    args[i] = NULL; // tells exec func where the cmd cmd ends
}


// Execute a command using fork + execvp - my ver
int execute_command(char **args) { 
  int child_status;
  
  if (args[0] == NULL)
  {
    ;
  } // built in function
  else if (is_builtin(args[0]) == 0)
  {
    if (execute_builtin(args) == 0){
      return 0; // exit shell if req
    }
  }
  else
  {
    pid_t pid = fork();
    if (pid < 0){ // fork failed

      perror("Fork has failed. Exiting now.");

      printf("%s🤖 Failed to execute command. Please retry.\n", RED); // exit error

    }
    else if (pid == 0){ // child process
      snprintf(full_path, sizeof(full_path), "%s/bin/%s", root_directory, args[0]);
      execv(full_path, args); // try running what's in the original path first
      execvp(args[0], args); // check saved path
      
      // If execv returns, command execution has failed
      printf("%s(つ◉益◉)つ Command '%s' not found.\n%s", RED, args[0], WHITE); 
      
      _exit(0);

    }
    else { // parent process


      // wait for child and get child pid
      pid_t child_pid = waitpid(pid, &child_status, 0); // get child status
      
      if (child_pid == -1){
        perror("waitpid failed"); 
        ; // continue if child failed, don't exit
      }

      // inspect child status
      if (WIFEXITED(child_status)) { // can comment out as not needed
          int exit_status = WEXITSTATUS(child_status);
          // printf("Child exited normally with status %d\n", exit_status); // for testing
      } else if (WIFSIGNALED(child_status)) {
          int term_signal = WTERMSIG(child_status);
          // printf("Child was terminated by signal %d\n", term_signal); // for testing
      } else {
          perror("Child terminated abnormally\n"); // for testing
      }


    }

  }
}

void load_language(){ 

  // read saved language
  snprintf(langpath, sizeof(langpath), LANG_FILE); // lang_file defined in header
  FILE *langfile = fopen(langpath, "r");
  if (!langfile){
    printf("%s🤖 Failed to load saved language. Default to english.\n%s", RED, WHITE);
  }

  else {
    
    if (fgets(language, sizeof(language), langfile)){
      language[strcspn(language, "\r\n")] = '\0';
      if (strcmp(language, "english") == 0){
        lang_id = 0;
      } else if (strcmp(language, "chinese") == 0){
        lang_id = 1;
      } else{
        printf("%s🤖 Failed to load saved language. Default to english.%s\n", RED, WHITE);
      }
    } else{
      printf("%s🤖 Failed to load saved language. Default to english.\n%s", RED, WHITE);
    }
    
    
  } 

  fclose(langfile);
};

void process_rc_file() {
    // read from ./cseshellrc
    char path[PATH_MAX];

    snprintf(path, sizeof(path), "./.cseshellrc");

    FILE *file = fopen(path, "r");
    if (!file) {
        perror("Could not open .cseshellrc");
        return;
    }

    // read line by line
    char line[MAX_LINE];
    while (fgets(line, sizeof(line), file)) { // read each line - line is the pointer to the buffer where that line string is stored

        
        char *line_pointer = line; // trimmed is a pointer to line

        // Remove leading whitespace
        while (*line_pointer == ' ' || *line_pointer == '\t') 
          line_pointer++;

        // Remove trailing newline and carriage return 
        line_pointer[strcspn(line_pointer, "\r\n")] = '\0';

        if (strncmp(line_pointer, "PATH", 4) == 0) { // add to env var if it is PATH by checking if it starts with PATH
            char *value = line_pointer + 5;
            setenv("PATH", value, 1);  // Overwrite PATH

        } else { // execute it if its a cmd

            char *args[MAX_ARGS];
            
            parse_line(line_pointer, args);
            
            execute_command(args);

        }
    }

    fclose(file);
}


// Function to display the shell prompt
void type_prompt() {
  fflush(stdout); // Flush the output buffer

  // get the cwd
  char cwd[1024];
  getcwd(cwd, sizeof(cwd));
  printf("%s>> %s >> %s \n", BLUE, cwd, SHELL_NAME); 
  printf("%s(ง'̀-'́)ง %s", CYAN, WHITE);  // Print the shell prompt // cyan
}



// The main function where the shell's execution begins - v4
int main(void) {
  

  int child_status;
  pid_t pid;

  // Define an array to hold the command and its arguments
  char *cmd[MAX_ARGS];

  // Define attributes to hold the current working directory and root directory
  getcwd(root_directory, sizeof(root_directory));

  // clear screen before starting
  #ifdef _WIN32
      system("cls"); // Windows command to clear screen
  #else
      system("clear"); // UNIX/Linux command to clear screen
  #endif
      
  

  // One-time startup config from rc file - in proj
  process_rc_file();
  load_language();

  printf("PATH = %s\n", getenv("PATH"));

  for(;;){

    type_prompt();     // Display the prompt
    read_command(cmd); // Read a command from the user

    // If the command is "empty", skip execution
    if (cmd[0] == NULL || strcmp(cmd[0], "") == 0){
      continue;

    }
    else
    {
      
      // built in function
      if (is_builtin(cmd[0]) == 0){
        
        if (execute_builtin(cmd) == 0){
          return 0; // exit shell if req
        }

      }
      else // not built in function
      {

      

      char cwd[1024]; 

      if (getcwd(cwd, sizeof(cwd)) != NULL) { 
        pid_t pid = fork();

        snprintf(full_path, sizeof(full_path), "%s/bin/%s", root_directory, cmd[0]); 

        if (pid < 0){ // fork failed

          fprintf(stderr, "Fork has failed. Exiting now.");
          printf("%s🤖 Failed to execute command. Please retry.\n", RED);
          return 1; // exit error

        }
        else if (pid == 0){ // child process

          execv(full_path, cmd); // try running what's in the original path first
          execvp(cmd[0], cmd); // check path env if execv fails 

          // If execv returns, command execution has failed
          if (lang_id == 1){
            printf("%s❌ 没有 '%s' 的指示.%s\n", RED, cmd[0], WHITE); 
          } else {
            printf("%s❌ Command '%s' not found.%s\n", RED, cmd[0], WHITE); 
          }

          _exit(0);

        }
        else { // parent process

          struct rusage usage;

          // wait for child and get child pid
          pid_t child_pid = wait4(pid, &child_status, WUNTRACED, &usage); // get child status
          
          if (WIFEXITED(child_status))
          {
            double user_time = usage.ru_utime.tv_sec + usage.ru_utime.tv_usec / 1e6;
            double sys_time = usage.ru_stime.tv_sec + usage.ru_stime.tv_usec / 1e6;
            long max_rss_kb = usage.ru_maxrss;
            long io_reads = usage.ru_inblock;
            long io_writes = usage.ru_oublock;

            printf("%s", GREEN);

            printf("Resource Usage:\n");
            printf("  User CPU time: %.6f sec\n", user_time);
            printf("  System CPU time: %.6f sec\n", sys_time);
            printf("  Memory usage: %ld KB\n", max_rss_kb);
            printf("  Disk I/O: %ld reads, %ld writes\n", io_reads, io_writes);

            printf("%s", WHITE);
          }

          if (child_pid == -1){
            //perror("waitpid failed"); 
            continue; // continue if child failed, don't exit
          }

          // inspect child status
          if (WIFEXITED(child_status)) { // can comment out as not needed
              int exit_status = WEXITSTATUS(child_status);
              // printf("Child exited normally with status %d\n", exit_status); // for testing
          } else if (WIFSIGNALED(child_status)) {
              int term_signal = WTERMSIG(child_status);
              // printf("Child was terminated by signal %d\n", term_signal); // for testing
          } else {
              perror("Child terminated abnormally\n"); // for testing
          }


        }
      }
      }

      // Free the allocated memory for the command arguments before exiting
      for (int i = 0; cmd[i] != NULL; i++)
      {
        free(cmd[i]);
        cmd[i] = NULL;
      }
  }

  }
}