# LiteShell
## LINUX SHELL IMPLEMENTATION IN C:

A simple shell implementation in C done as an assignment for the
Operating Systems course at IIIT Delhi.

## Descriptions of the directories and files:

1. **help/internals** is a directory that contains 4 text files- **help.txt**, **echo.txt**, **pwd.txt** and
**cd.txt**.
2. **external_commands** is a directory that contatins 5 C files and 1 directory-
**external_commands** contains **cat.c**, **ls.c**, **rm.c**, **mkdir.c** and **date.c**.
**external_commands** contains a directory **help_externals**- which contains the help text files for 
the external files.
3.**help_external** contains the text files- **cat.txt**, **ls.txt**, **rm.txt**, **mkdir.txt** and **date.txt**.
4. **external_commands_outputs** is a directory that contains 5 binary files of all the external
commands- **ls**, **mkdir**, **date**, **rm** and **cat**.
5. **shell.c** is a C file which contains the entire shell program. 
6. **shell** is an executable binary file of the C file **shell.c**.

## Building and compiling the Linux shell:

1. Command to compile all external command files:
`make main`
2. Command to compile the Linux shell files:
`make run`
3. Command to run the Linux shell executable file:
`./shell`
4. Command to remove all the executable files after complete execution:
`make clean`

## Commands Handled:
### Internal Commands: 
*pwd, exit, help, echo, cd*

### External Commands: 
*ls, rm, mkdir, date, cat*

## Methods to call the external commands:
### 1. Fork() and execl(): 
Using fork(), we create a child process and then using execvp(), we execute the external command by overwriting the child process with the executable 
file of the external command. We also use the wait() system call to wait for the child process to complete its execution and then continuing with the 
shell execution.
### 2. pthreads_create() and system():
Using pthread_create(), we create a thread and create a function for the thread to execute. In this function, we use the system() call to execute the 
external command. To wait for the thread to complete its execution, we use the pthread_join() system call.

To use the second method, we need to write **&t** while entering the external command. 
For example, if we want to execute the **ls -i** command using the second method, we need to write **ls -i &t** in the shell.
