#include <stdio.h>
#include <fcntl.h>      /* O_CREAT */
#include <semaphore.h>  /*sem_t*/
#include <stdlib.h>     /*exit()*/
#include <string.h>     /*strcmp*/
#include <assert.h>

#define PERMISSIONS (0666)
#define SEM_VAL (0)
#define BUFFER (15)

static void POSSemaphore(const char *name);

sem_t *POSSemCreate(const char *name);

int POSSemManipulate(sem_t *sem);

void UndoOps(sem_t *sem);

int glo_counter = 0;

int main(int argc, char *argv[])
{ 
    if(2 > argc)
    {
        printf("Wrong input. Input semaphore name in command line\n");
        return 1;
    }
    
    POSSemaphore(argv[1]);
    
    return 0;
}

static void POSSemaphore(const char *name)
{
    sem_t *sem = NULL;

    sem = POSSemCreate(name);

    while (0 == POSSemManipulate(sem));

    perror("SEMERR\n");
}

/*Open a named semaphore*/
sem_t *POSSemCreate(const char *name)
{
    return sem_open(name, O_CREAT, PERMISSIONS, SEM_VAL);
}

/*Sem_manipulate*/
int POSSemManipulate(sem_t *sem)
{
    char input[BUFFER] = {0};
    char command = 0;
    int value = 0;
    int success_flag = 0;
    int sem_value = 0;

    fgets(input, BUFFER, stdin);
    sscanf(input ,"%c %d %s", &command, &value, input);

    if(0 == strcmp("[undo]", input))
    {
        if('I' == command)
        {
            glo_counter += value;
        }
        else if('D' == command)
        {
            glo_counter -= value;
        }
    }

    switch (command)
    {
    case ('I'):
        do
        {
            success_flag = sem_post(sem);
        } while(0 < --value);
        break;
    
    case ('D'):
        do
        {
            success_flag = sem_wait(sem);
        } while(0 < --value);
        break;
    
    case ('V'):
        success_flag = sem_getvalue(sem, &sem_value);
        printf("Semaphore value: %d\n", sem_value);
        break;
    
    case ('X'):
        UndoOps(sem);
        sem_close(sem);
        exit(0);
    
    default:
        printf("Wrong input\n");
        break;
    }

    return success_flag;
}

void UndoOps(sem_t *sem)
{
    while(0 < glo_counter)
    {
        sem_wait(sem);
        --glo_counter;
    }

    while(0 > glo_counter)
    {
        sem_post(sem);
        ++glo_counter;
    }
}