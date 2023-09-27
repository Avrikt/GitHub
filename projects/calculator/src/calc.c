/********************************************
* Developer: Avri Kehat						*
* Reviewed by: Omri Shalev					*
* Date: 10.04.23							*
* Description: Source file for Calculator	*
*********************************************/

/************************************LIBRARIES**************************************************/
#include<stdlib.h> /*malloc, free*/
#include<assert.h> /*assert*/
#include<math.h> /*pow*/

#include "calc.h"
#include "stack.h"
#include "utilities.h"

/********************************FORWARD DECLERATIONS*******************************************/
#define ASCII 256

enum stack_type
{
    OPERAND = 0,
    OPERATOR = 1,
    NUM_OF_STACKS = 2
} stack_type_t;

struct calculator
{
	stack_t *stack[NUM_OF_STACKS];
};

typedef enum state
{
    INIT_S = 0,
    NUMBER_S = 1,
    OPERATOR_S = 2,
    ERROR_S = 3,
    NUM_OF_STATES = 4
} state_t;

typedef double (*arithmetic_operation_t)(double, double);
typedef calc_status_t (*state_op_t)(calc_t *, char **, state_t *);

state_op_t LUT_states[NUM_OF_STATES][ASCII] = {NULL};
arithmetic_operation_t LUT_arith_op[ASCII] = {NULL};
int LUT_precedence[ASCII] = {0};


static void InitLUT(void);

static calc_status_t PushOperand(calc_t *calc, char **input, state_t *state);

static calc_status_t PushOperator(calc_t *calc, char **input, state_t *state);

static calc_status_t CloseParen(calc_t *calc, char **input, state_t *state);

static calc_status_t Error(calc_t *calc, char **input, state_t *state);

static calc_status_t MakeSpace(calc_t *calc, char **input, state_t *state);

static double OperandStackPeekPop(stack_t *stack);

static char OperatorStackPeekPop(stack_t *stack);

static calc_status_t ExecuteMath(calc_t *calc);

static void EmptyStack(stack_t *stack);

static double Add(double x, double y);

static double Subtract(double x, double y);

static double Multiply(double x, double y);

static double Devide(double x, double y);

/************************************Functions***************************************************/
calc_t *CalcCreate(size_t max_exp)
{
    calc_t *new_calc = (calc_t *) malloc(sizeof(calc_t));
    if(NULL == new_calc)
    {
        return NULL;
    }

    new_calc->stack[OPERAND] = StackCreate(max_exp, sizeof(double));
    if(NULL == new_calc->stack[OPERAND])
    {
        free(new_calc);
        return NULL;
    }

    new_calc->stack[OPERATOR] = StackCreate(max_exp, sizeof(char));
    if (NULL == new_calc->stack[OPERATOR])
    {
        StackDestroy(new_calc->stack[OPERAND]);
        free(new_calc);
        return NULL;
    }
    
    InitLUT();

    return new_calc;
}

void CalcDestroy(calc_t *calc)
{
    size_t i = 0;

    for(i = 0; i < NUM_OF_STACKS; ++i)
    {
        StackDestroy(calc->stack[i]);
    }

    free(calc);
}

calc_status_t Calculator(calc_t *calc, const char *expression, double *result)
{
    char *ch = (char *)expression;
    state_t state = INIT_S;
    calc_status_t calc_status = SUCCESS;

    assert(NULL != calc);
    assert(NULL != expression);
    assert(NULL != result);

    *result = 0;

    while ('\0' != *ch && ERROR_S != state && SUCCESS == calc_status)
    {
        calc_status = LUT_states[state][(int)*ch](calc, &ch, &state);
    }
    
    if (NUMBER_S != state)
    {
        return SYNTAX_ERROR;
    }
    
    while(!StackIsEmpty(calc->stack[OPERATOR]) && SUCCESS == calc_status)
    {
        if ('(' == *(char *)StackPeek(calc->stack[OPERATOR]))
        {
            calc_status = UNBALANCED_PARANTHESIS;
            break;
        }
        calc_status = ExecuteMath(calc);
    }

    if(SUCCESS == calc_status)
    {
        *result = OperandStackPeekPop(calc->stack[OPERAND]);
    }

    EmptyStack(calc->stack[OPERAND]);
    EmptyStack(calc->stack[OPERATOR]);

    return calc_status;
}

static void InitLUT(void)
{
    size_t i = 0, j = 0;

    for(i = 0; i < NUM_OF_STATES; ++i)
    {
        for(j = 0; j < ASCII; ++j)
        {
            LUT_states[i][j] = &Error;
        }
    }

    for (i = '0'; i <= '9'; ++i)
    {
        LUT_states[INIT_S][i] = &PushOperand;
        LUT_states[OPERATOR_S][i] = &PushOperand;
    }
    
    LUT_states[NUMBER_S]['+'] = &PushOperator;
    LUT_states[NUMBER_S]['-'] = &PushOperator;
    LUT_states[NUMBER_S]['*'] = &PushOperator;
    LUT_states[NUMBER_S]['/'] = &PushOperator;
    LUT_states[NUMBER_S]['^'] = &PushOperator;
    LUT_states[NUMBER_S]['('] = &PushOperator;
    LUT_states[NUMBER_S][')'] = &CloseParen;

    LUT_states[INIT_S]['+'] = &PushOperand;
    LUT_states[INIT_S]['-'] = &PushOperand;
    
    LUT_states[OPERATOR_S]['+'] = &PushOperand;
    LUT_states[OPERATOR_S]['-'] = &PushOperand;

    LUT_states[INIT_S]['('] = &PushOperator;
    LUT_states[OPERATOR_S]['('] = &PushOperator;

    LUT_states[INIT_S][' '] = &MakeSpace;
    LUT_states[NUMBER_S][' '] = &MakeSpace;
    LUT_states[OPERATOR_S][' '] = &MakeSpace;

    LUT_arith_op['+'] = Add;
    LUT_arith_op['-'] = Subtract;
    LUT_arith_op['*'] = Multiply;
    LUT_arith_op['/'] = Devide;
    LUT_arith_op['^'] = pow;

    LUT_precedence['('] = 0;
    LUT_precedence['+'] = 1;
    LUT_precedence['-'] = 1;
    LUT_precedence['*'] = 2;
    LUT_precedence['/'] = 2;
    LUT_precedence['^'] = 3;

}

static calc_status_t PushOperand(calc_t *calc, char **input, state_t *state)
{
    double num = 0;
    num = strtod(*input, input);

    if(StackSize(calc->stack[OPERAND]) == StackCapacity(calc->stack[OPERAND]))
    {
        return EXPRESSION_EXCEEDED;
    }

    StackPush(calc->stack[OPERAND], &num);
    *state = NUMBER_S;

    return SUCCESS;
}

static calc_status_t PushOperator(calc_t *calc, char **input, state_t *state)
{
    char ch = **input;

    if(StackSize(calc->stack[OPERATOR]) == StackCapacity(calc->stack[OPERATOR]))
    {
        return EXPRESSION_EXCEEDED;
    }

    while (!StackIsEmpty(calc->stack[OPERATOR]) && 
          (LUT_precedence[(int)*(char *)StackPeek(calc->stack[OPERATOR])] >= LUT_precedence[(int)ch]) &&
          '(' != ch && '^' != ch)
    {
        ExecuteMath(calc);
    }

    StackPush(calc->stack[OPERATOR], &ch);
    ++(*input);
    *state = OPERATOR_S;

    return SUCCESS;
}

static calc_status_t MakeSpace(calc_t *calc, char **input, state_t *state)
{
    (void)calc;
    (void)state;
    ++(*input);

    return SUCCESS;
}

static calc_status_t CloseParen(calc_t *calc, char **input, state_t *state)
{
    while(!StackIsEmpty(calc->stack[OPERATOR]) && '(' != *(char *)StackPeek(calc->stack[OPERATOR]))
    {
        ExecuteMath(calc);
    }

    if (StackIsEmpty(calc->stack[OPERATOR]))
    {
        return UNBALANCED_PARANTHESIS;
    }
    
    StackPop(calc->stack[OPERATOR]);
    ++(*input);
    *state = NUMBER_S;

    return SUCCESS;
}

static calc_status_t Error(calc_t *calc, char **input, state_t *state)
{
    (void)calc;
    (void)input;
    *state = ERROR_S;

    return SYNTAX_ERROR;
}

static double OperandStackPeekPop(stack_t *stack)
{
    double result = *(double *)StackPeek(stack);
    StackPop(stack);

    return result;
}

static char OperatorStackPeekPop(stack_t *stack)
{
    char result = *(char *)StackPeek(stack);
    StackPop(stack);

    return result;
}

static calc_status_t ExecuteMath(calc_t *calc)
{
    double num1 = 0, num2 = 0;
    calc_status_t status = SUCCESS;
    char op = OperatorStackPeekPop(calc->stack[OPERATOR]);

    num1 = OperandStackPeekPop(calc->stack[OPERAND]);
    num2 = OperandStackPeekPop(calc->stack[OPERAND]);

    if ('/' == op && 0 == num1)
    {
        status = DIVISION_BY_ZERO;
    }

    num1 = LUT_arith_op[(int)op](num2, num1);
    StackPush(calc->stack[OPERAND], &num1);

    return status;
}

static void EmptyStack(stack_t *stack)
{
    while(!StackIsEmpty(stack))
    {
        StackPop(stack);
    }
}

static double Add(double x, double y){return x + y;}

static double Subtract(double x, double y){return x - y;}

static double Multiply(double x, double y){return x * y;}

static double Devide(double x, double y){return x / y;}