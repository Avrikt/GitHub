#ifndef MY_PROJECT_FSQ_H__
#define MY_PROJECT_FSQ_H__

#include <stddef.h>

#define PEEK_ERROR (-1)

typedef struct fsq fsq_t;


fsq_t *FSQCreate(size_t size);
void FSQDestroy(fsq_t *q);
int FSQEnqueue(fsq_t *q, int input);
int FSQDequeue(fsq_t *q);
int FSQPeek(fsq_t *q);
int FSQIsEmpty(fsq_t *q);
int FSQIsFull(fsq_t *q);

#endif