
#include <stdio.h> /*fgets*/
#include <string.h> /*strcmp*/
#include <stdlib.h> /*system()*/
#include <unistd.h> /*fork()*/
#include <sys/types.h> /*pid_t*/
#include <wait.h> /*wait()*/

#define BUFFER (60)
#define ARGS (50)

void System(void);

void Fork(void);

int main(void)
{
    char choice[BUFFER] = {0};

    printf("\n\t\t\x1b[1;33mLaunching Avri's shell...\x1b[0m\n");
    
    printf("\n\x1b[3;36mChoose shell version (System/Fork):\x1b[0m ");

    fgets(choice, BUFFER, stdin);

    if(0 == strncmp(choice, "Fork", 4))
    {
        Fork();
    }
    else if (0 == strncmp(choice, "System", 6))
    {
        System();
    }
    else
    {
        printf("\x1B[1;31mWrong input\nExiting program\x1b[0m\n");
    }


    return 0;
}

void System(void)
{
    char command[BUFFER] = {0};

    while (0 != strncmp(command, "exit", 4))
    {
        printf("\n\x1b[1;32mAvri's-system\x1b[0m:\x1B[1;34m~\x1b[0m$ ");
        
        fgets(command, BUFFER, stdin);
        system(command);
    }
}

void Fork(void)
{
    char command[BUFFER] = {0};
    char *args[ARGS] = {NULL};
    char *runner = NULL;
    char *token = NULL;
    int status = 0;
    pid_t pid = 0;
    size_t i = 0;

    while (-1 != pid)
    {
        i = 0;
        printf("\n\x1b[1;32mAvri's-fork\x1b[0m:\x1B[1;34m~\x1b[0m$ ");

        fgets(command, BUFFER, stdin);

        runner = strchr(command, '\n');
        *runner = '\0';

        if(0 == strcmp(command, "exit"))
        {
            exit(1);
        }
        token = strtok(command, " ");

        while(NULL != token)
        {
            args[i++] = token; 
            token = strtok(NULL, " ");
        }
        args[i] = NULL;

        pid = fork();

        if(0 == pid)
        {
            status = execvp(command, args);
            if(-1 == status)
            {
                exit(1);
            }
        }
        else
        {
            wait(&status);
        }
    }
}