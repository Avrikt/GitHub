#ifndef MY_PROJECT_FSQ__
#define MY_PROJECT_FSQ__

#include <stddef.h>

#define PEEK_ERROR ((size_t)-1)

enum status
{
    SUCCESS = 0,
    QUEUE_IS_FULL = -1,
    QUEUE_IS_EMPTY = -2
};

typedef struct fsq fsq_t;


fsq_t *FSQCreate(size_t size);
void FSQDestroy(fsq_t *q);
void FSQEnqueue(fsq_t *q, int input);
void FSQDequeue(fsq_t *q);
int FSQPeek(fsq_t *q);
int FSQIsEmpty(fsq_t *q);

#endif