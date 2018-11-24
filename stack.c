#include "stack.h"

#include <string.h>
#include <stdio.h>
#include <assert.h>

Stack* stackAlloc(size_t elementSize)
{
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    if (stack == NULL)
    {
        return NULL;
    }
    stack->_top = NULL;
    stack->_elementSize = elementSize;
    return stack;
}

void freeStack(Stack** stack)
{
    Node* p1;
    Node* p2;
    if (*stack != NULL)
    {
        p1= (*stack)->_top;
        while(p1)
        {
            p2= p1;
            p1= p1->_next;
            free(p2->_data);
            free(p2);
        }
        free(*stack);
        *stack = NULL;
    }
}

int push(Stack* stack, void *data)
{
    Node* node = (Node*)malloc(sizeof(Node));
    if (node == NULL)
    {
        return 1;
    }
    node->_data = data;
    node->_next = stack->_top;
    stack->_top = node;
    return 0;
}

void * pop(Stack* stack)
{
    if(stack->_top == NULL)
    {
        fprintf(stderr, "The stack is empty\n");
        return NULL;
    }
    Node *node = stack->_top;
    stack->_top = node->_next;
    void * data = node->_data;
    free(node);
    return data;
}

int isEmptyStack(Stack* stack)
{
    return stack->_top == NULL;
}
