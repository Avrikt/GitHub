/********************************************
* Developer: Avri Kehat						*
* Reviewed by: Egor Markov					*
* Date: 10.05.23							*
* Description: File for Sys V Semaphore		*
*********************************************/

/****************************************LIBRARIES**************************************************/

#include <stdio.h>
#include <sys/sem.h> /*semget, semctl, semop*/
#include <string.h> /*strcmp*/
#include <stdlib.h> /*exit()*/

#define BUFFER (15)
#define ERROR (-1)
#define ID (132)
#define PERMISSIONS (0666)
#define INIT_VAL (0)

/************************************ Foward Declarations*******************************************/
static void SysSemaphore(const char *name);

int SysVSemCreate(const char *name);

int SysSemManipulate(int semid);

int glo_counter = 0;

int main(int argc, char const *argv[])
{
    
    if (2 > argc)  
    {
        perror("Invalid input\n");
        return ERROR;
    }
    
    SysSemaphore(argv[1]);

    return 0;
}

/************************************ Functions *******************************************/
static void SysSemaphore(const char *name)
{
    int semid = SysVSemCreate(name);
    if(ERROR == semid)
    {
        perror("Error creating semaphore\n");
        return;
    }

    while(0 == SysSemManipulate(semid));

    perror("SEMERR\n");
}

int SysVSemCreate(const char *name)
{
    key_t key = ftok("/home/avri/git/system_programming/semaphores/sem_sysv.c", *(int *)name);
    int semid = 0;

    if(-1 == key)
    {
        perror("ftok\n");
    }

    semid = semget(key, 1, PERMISSIONS | IPC_CREAT | IPC_EXCL);
    if(ERROR == semid)
    {
        semid = semget(key, 1, PERMISSIONS | IPC_CREAT);
    }        
    else
    {
        semctl(semid, 0, SETVAL, INIT_VAL);
    }

    return semid;
}

int SysSemManipulate(int semid)
{
    char input[BUFFER] = {0};
    struct sembuf sb; 
    char command = 0;
    int value = 1;
    int success_flag = 0;
    int exit_status = 0;

    fgets(input, BUFFER, stdin);
    sscanf(input ,"%c %d %s", &command, &value, input);

    if(0 == strcmp("[undo]", input))
    {
        sb.sem_flg = SEM_UNDO;
        exit_status = 1;
    }

    switch (command)
    {
    case ('I'):
            sb.sem_op = value;
            success_flag = semop(semid, &sb, 1);
        break;
    
    case ('D'):
            sb.sem_op = (-value);
            success_flag = semop(semid, &sb, 1);
        break;
    
    case ('V'):
        printf("Semaphore value: %d\n", semctl(semid, 0, GETVAL));
        break;
    
    case ('X'):
        exit(exit_status);
    
    default:
        printf("Wrong input\n");
        break;
    }

    return success_flag;
}
