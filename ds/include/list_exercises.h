#ifndef MY_PROJECT_LIST_EXERCISES_H__
#define MY_PROJECT_LIST_EXERCISES_H__

typedef struct node
{
    void *data;
    struct node *next;
}node_t;

node_t *Flip(node_t *head);

int HasLoop(const node_t *head);

node_t *FindInterSection(node_t *head_1, node_t *head_2);

void PrintList(node_t *node);


#endif
