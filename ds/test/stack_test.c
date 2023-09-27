/********************
Developer: Avri Kehat
Reviewed by: Lilach Rabinovich
Date: 30.01.23
Description: Implementation of a stack
*********************/

#include <stdio.h>	/* printf */
#include <stdlib.h> /* exit*/
#include "stack.h"

int main(void)
{
	stack_t *stack = StackCreate((size_t)10, sizeof(int));
	int *top_item = NULL;
	int item = 0;
	int operation = 0;
	
	while(1)
	{
		printf("\nChoose operation on stack from the following:\n");
		printf("1: Stack Push\t2: Stack Pop\t3: Stack Peek\t4: Check if stack is empty\t\n5: Stack Size\
		6: Stack Capacity\t 7: Stack Destroy\n");
		if (1 != scanf("%d", &operation))
		{
			printf("Wrong input\n");
			break;
		} 
				
		switch(operation)
		{
			case 1:
				printf("Enter member to push: ");
				scanf("%d", &item);
				StackPush(stack, &item);
				break;
			
			case 2:
				StackPop(stack);
				break;
				
			case 3:
				top_item = StackPeek(stack);
				printf("Top member in stack: %d\n", *top_item);
				break;	
			
			case 4:
				if(StackIsEmpty(stack))
				{
					printf("Stack is empty\n");
				}
				else
				{
					printf("Stack is not empty\n");
				}
				break;
				
			case 5:
				printf("Stack size is: %ld\n", StackSize(stack));
				break;
				
			case 6:
				printf("Stack capacity: %ld\n", StackCapacity(stack));
				break;
				
			case 7:
				StackDestroy(stack);
				printf("Stack Destroyed\n");
				exit(0);
				
			default: 
				printf("Input incorrect\n"); 
				break;
		}
	} 	

	return 0;
}
