# Custom-Shell
A custom shell coded with C for use in Unix-based systems designed to provide an interface for executing system programs.

## Directory Structure

The project is organized as follows:

- `bin/` - Contains compiled executables for system programs.
  - `find` - Program to find files.
  - `ld` - Program for listing the contents of the current directory.
  - `ldr` - Program for listing the contents of the current directory recursively.
  - `sys.c` - Program for listing the system information.
  - `backup.c` - Program for zipping the file/folder at the specified specified path and saving it in the archive folder, naming based on date and time of creation.
  - `dspawn.c` - Program for spawning a daemon process.
  - `dcheck.c` - Program for showing the number of running daemon processes.
- `cseshell` - The main executable for the shell.
- `files/` - Contains various test files used with the shell and system programs.
  - `combined.txt`, `file1.txt`, `file2.txt`, ... - Test text files.
  - `notes.pdf` - A PDF file for testing.
  - `ss.png` - An image file.
- `makefile` - Makefile for building the shell and system programs.
- `source/` - Source code for the shell and system programs.
  - `shell.c` and `shell.h` - Source and header files for the shell.
  - `system_programs/` - Source code and header for the system programs.



## Building the Project

To build the shell and system programs, run the following command in the root directory:

```bash
make
```

This will compile the source code and place the executable files in the appropriate directories.

## Running shell

After building, you can start the shell by running:

```bash
chmod +x cseshell
```
then

```bash
./cseshell
```

From there, you can execute built-in commands and any of the included system programs (e.g., `find`, `ld`, `ldr`).

## System Programs

- `find.c` - Searches for files in a directory.
- `ld.c` - List the contents of the curent directory.
- `ldr.c` - List the contents of the current directory recursively.
- `sys.c` - List the system information.
- `backup.c` - Zips the file/folder specified and saves it in the archive folder, naming based on date and time of creation.
    *Please note that the ROOT_DIR has to be changed accordingly before use.*
- `dspawn.c` - Spawn a daemon process.
- `dcheck.c` - Show the number of running daemon processes.

Each program can be executed from the shell once it is running. This starter code only allows the shell to execute a command once before exiting because `execv` replace the entire process' address space. Students need to fix this and allow the shell to prompt for more commands in Programming Assignment 1.

## Builtin commands
-`cd`:  Changes the current directory of the shell to the specified path. 
e.g. cd files | navigates to files subdirectory if it exists

-`help`:   List all builtin commands in the shell

-`exit`:    Exits the shell

-`usage`:  Provides a brief usage guide for the shell and its built-in command
e.g. usage env | displays the usage guide for env

-`env`:  Lists all the environment variables currently set in the shell

-`setenv`: Sets or modifies an environment variable for this shell session
e.g. setenv TEST="value" | adds TEST="value" to the list of this process' environment variables

-`unsetenv`:  Removes an environment variable which key matches the input from the shell
e.g. unsetenv TEST | removes TEST="value" that was previously added from the list of this process' environment variables

-`lang`: Changes the language of the descriptions and error message used in the built-in commands of the shell 
e.g. lang chinese 
Current supported languages:
1. English
2. Chinese

-`history`: view the 5 most recently used commands (ignoring empty inputs) entered 

## Sustanability
1. After typing a command, shell will display user CPU time, system CPU time, memory usage and disk I/O to promote awareness of resource consumption.

## Inclusitivity
1. Added support for multiple languages using the "lang" command so that people who may not be so familiar with english can utilize the shell more easily as well. Currently supports chinese and english, but could be easily expanded to support more languages.
Language chosen is saved, so when the shell exited and reopened later, the last set language will be the one loaded.

2. Simple error messaging done in plain language with added descriptions on how some in-built commands that require additional arguments can be used are provided so that people who are not familiar with using the shell can use it more easily. 
Messages are also color coded with error messages being light red to highlight that the command did not succeed so that it is easy for those who are not familiar with the shell to know when something is wrong.

## Makefile

The Makefile contains rules for compiling the shell and system programs. You can clean the build by running:

```bash
make clean
```

## Source Directory

Contains all the necessary source code for the shell and system programs. It is divided into the shell implementation (`shell.c`, `shell.h`) and system programs (`system_programs/`).

