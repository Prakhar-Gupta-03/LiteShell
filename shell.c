#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <limits.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <errno.h>
#include <sys/wait.h>
#include <pthread.h>


#define COMMAND_WORDS_MAX 100
#define WORD_LENGTH_MAX 100

void *threadingAndT(void *arg){
    char *filename = (char*)arg;
    system(filename);
    return NULL;
}
void shell_exit_help(){
    FILE *file;
    file = fopen("help_internals/exit.txt", "r");
    char c;
    c = fgetc(file);
    while (c != EOF){
        printf("%c", c);
        c = fgetc(file);
    }
    fclose(file);
}
void help(){
    FILE *file;
    file = fopen("help_internals/help.txt", "r");
    char c = fgetc(file);
    while (c != EOF){
        printf("%c", c);
        c = fgetc(file);
    }
    fclose(file);
}
void pwd(char *instruction[], int numberOfInstructions){
    if (numberOfInstructions==1 || numberOfInstructions==2){
        if (numberOfInstructions==2){
            if (strcmp(instruction[1],"--help")==0){
                FILE *file;
                file = fopen("help_internals/pwd.txt", "r");
                char c = fgetc(file);
                while (c != EOF){
                    printf("%c", c);
                    c = fgetc(file);
                }
                fclose(file);
            }
            else if (strcmp(instruction[1],"-p")==0){
                char cwd[PATH_MAX];
                if (getcwd(cwd, sizeof(cwd)) != NULL){
                    printf("Present Working Directory: %s\n", cwd);
                }
            }
            else{
                printf("Invalid command entered\n");
            }
        }
        else{
            char presentWorkingDirectory[PATH_MAX];
            getcwd(presentWorkingDirectory,sizeof(presentWorkingDirectory));
            printf("Present Working Directory: %s\n",presentWorkingDirectory);
        }
    }
    else{
        printf("Error: Invalid number of arguments\n");
    }
}
void echo(char *instruction[], int numberOfInstructions){
    if (numberOfInstructions==1){
        printf("Too few arguments\n");
    }
    else if (strcmp(instruction[1],"--help")==0 && numberOfInstructions!=2){
        printf("Too many arguments\n");
    }
    else if (numberOfInstructions==2 && (strcmp(instruction[1],"-e")==0 || strcmp(instruction[1],"-n")==0)){
        printf("Too few arguments\n");
    }
    else{
        if (strcmp(instruction[1],"--help")==0 && numberOfInstructions==2){
            FILE *file;
            file = fopen("help_internals/echo.txt", "r");
            char c = fgetc(file);
            while (c != EOF){
                printf("%c", c);
                c = fgetc(file);
            }
            fclose(file);
        }
        else if (strcmp(instruction[1],"-e")==0){
            char* string = (char*)malloc(COMMAND_WORDS_MAX*WORD_LENGTH_MAX*sizeof(char));
            for (int i = 2; i<numberOfInstructions; i++){
                strcat(string,instruction[i]);
                strcat(string," ");
            }
            for (int i = 0; i<100; i++){
                if (string[i]=='\\' && string[i+1]=='n'){
                    printf("\n");
                }
                else if (string[i]=='n' && string[i-1]=='\\'){
                    continue;
                }
                else if (string[i]=='\\' && string[i+1]=='t'){
                    printf("\t");
                }
                else if (string[i]=='t' && string[i-1]=='\\'){
                    continue;
                }
                else{
                    printf("%c",string[i]);
                }
            }
            printf("\n");
        }
        else if (strcmp(instruction[1],"-n")==0){
            char* string = (char*)malloc(COMMAND_WORDS_MAX*WORD_LENGTH_MAX*sizeof(char));
            for (int i = 2; i<numberOfInstructions; i++){
                strcat(string,instruction[i]);
                strcat(string," ");
            }
            printf("%s ",string);
        }
        else{
            char* string = (char*)malloc(COMMAND_WORDS_MAX*WORD_LENGTH_MAX*sizeof(char));
            for (int i = 1; i<numberOfInstructions; i++){
                strcat(string,instruction[i]);
                strcat(string," ");
            }
            printf("%s\n",string);
        }
    }
}
void cd(char *instruction[], int numberOfInstructions){
    if (numberOfInstructions==1){
        // change directory to home
        char *homeDirectory = getenv("HOME");
        chdir(homeDirectory);
        // printf("Too few arguments\n");
    }
    else if (numberOfInstructions==2 && strcmp(instruction[1],"--help")==0){
        FILE *file;
        file = fopen("help_internals/cd.txt", "r");
        char c = fgetc(file);
        while (c != EOF){
            printf("%c", c);
            c = fgetc(file);
        }
        fclose(file);
    }
    else if (numberOfInstructions==2 && strcmp(instruction[1],"~")==0){
        // change directory to home
        char *homeDirectory = getenv("HOME");
        chdir(homeDirectory);
        // printf("Too few arguments\n");
    }
    else if (numberOfInstructions==2 && strcmp(instruction[1],"..")==0){
        // change directory to parent
        chdir("..");
    }
    else if (numberOfInstructions==2 && strcmp(instruction[1],".")==0){
        // change directory to present
        char presentWorkingDirectory[PATH_MAX];
        getcwd(presentWorkingDirectory,sizeof(presentWorkingDirectory));
        chdir(presentWorkingDirectory);
    }
    else if (numberOfInstructions>=2){
        // generate string of sum of all strings in instruction
        char* string = (char*)malloc(COMMAND_WORDS_MAX*WORD_LENGTH_MAX*sizeof(char));
        for (int i = 1; i<numberOfInstructions; i++){
            strcat(string,instruction[i]);
            if (i!=numberOfInstructions-1){
                strcat(string," ");
            }
        }
        // check if string directory exists
        if (chdir(string)==0){
            // change directory to string
            chdir(string);
        }
        else{
            printf("No such directory exists\n");
        }
    }
    else{
        printf("Error: Invalid command\n");
    }
}
int main(){
    printf("***** LINUX SHELL IMPLEMENTATION IN C *****\n");
    char main_path[PATH_MAX];
    getcwd(main_path,sizeof(main_path));
    printf("Current working directory: %s\n",main_path);
    printf("Enter 'help' to see all the available commands\n");
    while(1){
        printf(">> ");
        char *line = (char *)malloc(COMMAND_WORDS_MAX*WORD_LENGTH_MAX*sizeof(char));
        fgets(line,COMMAND_WORDS_MAX*WORD_LENGTH_MAX,stdin);
        line = strtok(line,"\n");
        if (strcmp(line,"exit")==0){
            printf("Ending program execution...\n");
            printf("Exiting the shell...\n");
            exit(0);
        }
        else if(strcmp(line,"exit --help")==0){
            shell_exit_help();
            continue;
        }
        char *instruction[COMMAND_WORDS_MAX];
        int numberOfInstructions = 0;
        int LineCommandPointer = 0;
        while (LineCommandPointer < (int)(strlen(line))){
            int WordLength = 0;
            char* NextWordInCommand = (char*)(malloc(WORD_LENGTH_MAX*sizeof(char)));
            while (LineCommandPointer < (int)(strlen(line)) && line[LineCommandPointer] != 0x0 && line[LineCommandPointer] != 0x0a && line[LineCommandPointer] != 0x20){
                NextWordInCommand[WordLength++] = line[LineCommandPointer++];
                if (line[LineCommandPointer]==0x0a || line[LineCommandPointer]==0x0){
                    break;
                }
            }
            NextWordInCommand[WordLength] = '\0';
            instruction[numberOfInstructions] = (char*)malloc(WORD_LENGTH_MAX*sizeof(char));
            strcpy(instruction[numberOfInstructions++],NextWordInCommand);
            if (line[LineCommandPointer]==0x20){
                while (line[LineCommandPointer]==0x20 && LineCommandPointer < (int)(strlen(line))){
                    LineCommandPointer++;
                }
            }
            if (line[LineCommandPointer]==0x0a || line[LineCommandPointer]==0x0){
                break;
            }
        }
        int isInternal = 0;
        int isExternal = 0;
        int isAndT = 0;
        if (strcmp(instruction[0],"ls")==0 || strcmp(instruction[0],"mkdir")==0 || strcmp(instruction[0],"date")==0 || strcmp(instruction[0],"rm")==0 || strcmp(instruction[0],"cat")==0){
            isExternal = 1;
        }
        if (strcmp(instruction[0],"pwd")==0 || strcmp(instruction[0],"echo")==0 || strcmp(instruction[0],"cd")==0 || strcmp(instruction[0],"help")==0){
            isInternal = 1;
        }
        for (int i = 0; i<numberOfInstructions; i++){
            if (strcmp(instruction[i],"&t")==0){
                isAndT = 1;
            }
        }
        if (isInternal){
        // Calling the echo function
            if (strcmp(instruction[0],"echo")==0){
                echo(instruction,numberOfInstructions);
            }
            // Calling the help function
            else if (strcmp(instruction[0],"help")==0){
                help();
            }
            // Calling the pwd function
            else if (strcmp(instruction[0],"pwd")==0){
                pwd(instruction, numberOfInstructions);
            }
            // Calling the cd function
            else if (strcmp(instruction[0],"cd")==0){
                cd(instruction, numberOfInstructions);
            }
        }
        // check if instruction[0] is mkdir, ls, date, rm, cat
        else if (isExternal){
            // create a path for each command execution
            // generate the absolute path 
            char *path = (char*)malloc(WORD_LENGTH_MAX*sizeof(char));
            getcwd(path,WORD_LENGTH_MAX);
            strcpy(path,main_path);
            strcat(path,"/external_commands_outputs/");
            if (strcmp(instruction[0],"ls")==0){
                strcat(path,"ls");
            }
            else if (strcmp(instruction[0],"mkdir")==0){
                strcat(path,"mkdir");
            }
            else if (strcmp(instruction[0],"date")==0){
                strcat(path,"date");
            }
            else if (strcmp(instruction[0],"rm")==0){
                strcat(path,"rm");
            }
            else if (strcmp(instruction[0],"cat")==0){
                strcat(path,"cat");
            }
            if (!isAndT){
                // create a child process and run another file
                pid_t pid = fork();
                if (pid==0){
                    // child process
                    char* args[numberOfInstructions+1];
                    args[0] = path;
                    for (int i = 1; i<=numberOfInstructions; i++){
                        args[i] = instruction[i-1];
                    }
                    if (strcmp(instruction[0],"ls")==0){
                        char curr_path[PATH_MAX];
                        getcwd(curr_path,sizeof(curr_path));
                        // replace the main_path section of curr_path with a "."
                        char* new_path = (char*)malloc(PATH_MAX*sizeof(char));
                        strcat(new_path,".");
                        int i = 0;
                        while (i<strlen(main_path)){
                            if (main_path[i]==curr_path[i]){
                                i++;
                            }
                            else{
                                break;
                            }
                        }
                        int j = 1;
                        while (i<strlen(curr_path)){
                            new_path[j++] = curr_path[i++];
                        }
                        new_path[j] = '\0';
                        args[1] = new_path;
                    }
                    args[numberOfInstructions+1] = NULL;
                    // run the external command
                    execvp(args[0],args);
                    return 0;
                }
                else if (pid>0){
                    wait(NULL);
                }
            }
            else if (isAndT==1){
                // replace the main_path section of curr_path with a "."
                char* new_path = (char*)malloc(PATH_MAX*sizeof(char));
                strcat(new_path,".");
                int i = 0;
                while (i<strlen(main_path)){
                    if (main_path[i]==path[i]){
                        i++;
                    }
                    else{
                        break;
                    }
                }
                int j = 1;
                while (i<strlen(path)){
                    new_path[j++] = path[i++];
                }
                new_path[j] = '\0';
                pthread_t thread_1;
                char* filename = (char*)malloc(1000*sizeof(char));
                strcpy(filename,new_path);
                int k = 0;
                while (k<numberOfInstructions){
                    if (strcmp(instruction[k],"&t")==0){
                        k++;
                        continue;
                    }
                    strcat(filename," ");
                    strcat(filename,instruction[k++]);
                }
                pthread_create(&thread_1,NULL,threadingAndT,(void*)filename);
                pthread_join(thread_1,NULL);
            }
        }
        else{
            printf("Error: Invalid command\n");
        }        
    }
}