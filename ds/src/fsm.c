
#include <stdio.h>

#define MAX_INPUTS 2

typedef enum input {ZERO, ONE} input_t;

typedef enum state
{
    START = 0,
    STATE_1 = 1,
    SUCCESS = 2,
    ERROR = 3
}state_t;

typedef struct state_change
{
    const input_t input;
    const state_t state;
} state_change_t;

typedef struct fsm
{
    const state_t state;
    state_change_t table[MAX_INPUTS];
}fsm_t;

const fsm_t fsm[]=
{
    {START,   {{ZERO, STATE_1}, {ONE, ERROR}}},
    {STATE_1, {{ZERO, SUCCESS}, {ONE, STATE_1}}},
    {SUCCESS, {{ZERO, SUCCESS}, {ONE, STATE_1}}}
};

static void FSMBinaryDigit(void);

int main(void)
{
    FSMBinaryDigit();

    return 0;
}

static void FSMBinaryDigit(void)
{
    int input = 0;
    int current_state = START;

    printf("Input 0 or 1:\n");

    while(ERROR != current_state)
    {
        scanf("%d",&input);
        if(1 < input)
        {
            break;
        }
        current_state = fsm[current_state].table[input].state;
    }

    if(SUCCESS == current_state)
    {
        printf("String accepted\n");
    }
    else
    {
        printf("String not accepted\n");
    }
}
