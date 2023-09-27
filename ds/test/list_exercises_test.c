/********************
Developer: Avri Kehat
Reviewed by: Yossi Matzliah
Date: 09.02.2023
*********************/

#include <stdio.h> /*printf*/
#include "list_exercises.h"

int main(void)
{
    node_t *test;
    
    node_t head1;
    node_t two;
    node_t three;
    node_t four;
    node_t five;
    node_t tail;
    
    node_t head2;
    node_t alpha;
    node_t beta;
    
    int num1 = 1;
    int num2 = 2;
    int num3 = 3;
    int num4 = 4;
    int num5 = 5;
    int num6 = 6;
 
    head1.data = (void *)&num1;
    head1.next = &two;
    
    two.data = (void *)&num2;
    two.next = &three;
    
    three.data = (void *)&num3;
    three.next = &four;
    
    four.data = (void *)&num4;
    four.next = &five;
    
    five.data = (void *)&num5;
    five.next = &tail;
    
    tail.data = (void *)&num6;
    tail.next = NULL;
    
    head2.data = (void *)&num3;
    head2.next = &alpha;
    
    alpha.data = (void *)&num2;
    alpha.next = &beta;
    
    beta.data = (void *)&num1;
    beta.next = &three;
    
    test = FindInterSection(&head1, &head2);

    if (*(int *)test->data == num3)
    {
    	printf("FindeInterSection test: success\n");
    }
    if (HasLoop(&head1))
    {
        printf("Has Loop\n");
    }
    else
    {
        printf("Doesnt have Loop\n");
    }
    
    tail.next = &four;
    
    if (HasLoop(&head1))
    {
        printf("Has Loop\n");
    }
    else
    {
        printf("Doesnt have Loop\n");
    }
	   
	tail.next = NULL;
    printf("\nOriginal list:\n");
    PrintList(&head1);
	
	printf("\nFlipped list:\n");
    PrintList(Flip(&head1));
    
    return 0;    
}
