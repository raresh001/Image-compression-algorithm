/*TICUS Rares Andrei - Grupa 313CD*/

#ifndef _QUEUE_H
#define _QUEUE_H

#include "quaternarTree.h"

typedef QuaternarTreeNode *T;

typedef struct queueNode {
    T value;
    struct queueNode *next;
} QueueNode;

typedef struct {
    QueueNode *head;
    QueueNode *tail;
} Queue;

Queue *initialiseQueue();
void enqueue(Queue *queue, T value);
T dequeue(Queue *queue);
Queue *destroyQueue(Queue *queue);

#endif //  _QUEUE_H