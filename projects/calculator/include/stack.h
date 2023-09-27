#ifndef MY_PROJECT_STACK_H__
#define MY_PROJECT_STACK_H__

typedef struct stack stack_t;

/* 
* StackCreate Description:
* Create a stack according to user's input of capacity and size of the item to store 
*
* @param:
*	capacity - the total size of the stack. Item size * number of elements required.	 
*	item_size - size of the item to be stored in the stack.
* 	The program cannot deal with a variety of data types.
*
* @Return
*	returns a pointer to the beginning of the stack
*
* @Complexity
*	Time: O(1)
*/

stack_t *StackCreate(size_t capacity, size_t item_size);

/* 
* StackDestroy Description:
* Dismantle the stack from memory.
*
* @param:
*	*stack - pointer to the stack.
*
* @Return
*	void
*
* @Complexity
*	Time: O(1)
*/
void StackDestroy(stack_t *stack);

/* 
* StackPush Description:
* Pushes the item from user input to top of stack 
* Function does not protect user from pushing items to a full stack.
*
* @param:
*	*stack - pointer to the stack.
*	item - pointer to the item to store.
*
* @Return
*	void
*
* @Complexity
*	Time: O(1)
*/
void StackPush(stack_t *stack, const void *item);

/* 
* StackPop Description:
* Pops the last item from the top of the stack 
* Function does not protect user from popping items of an empty stack.
* 
* @param:
*	*stack - pointer to the stack.
*
* @Return
*	void
*
* @Complexity
*	Time: O(1)
*/
void StackPop(stack_t *stack);

/* 
* StackPeak Description:
* Return pointer to the top element of the stack 
* Function does not protect user from peaking at an empty stack.
* 
* @param:
*	*stack - pointer to the stack.
*
* @Return
*	void pointer to the top element of the stack
*
* @Complexity
*	Time: O(1)
*/
void *StackPeek(const stack_t *stack);

/* 
* StackIsEmpty Description:
* Checks if stack is empty
* 
* @param:
*	*stack - pointer to the stack.
*
* @Return
*	Returns 1 if stack is empty and 0 if stack is full
*
* @Complexity
*	Time: O(1)
*/
int StackIsEmpty(const stack_t *stack);

/* 
* StackSize Description:
* Checks number of elements currently stored in the stack
* 
* @param:
*	*stack - pointer to the stack.
*
* @Return
*	size_t of size of elements in the stack 
*
* @Complexity
*	Time: O(1)
*/
size_t StackSize(const stack_t *stack);

/* 
* StackCapacity Description:
* Checks the capacity of the stack 
* 
* @param:
*	*stack - pointer to the stack.
*
* @Return
*	size_t of total capacity of the stack 
*
* @Complexity
*	Time: O(1)
*/
size_t StackCapacity(const stack_t *stack);

#endif
