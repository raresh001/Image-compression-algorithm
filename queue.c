/*TICUS Rares Andrei - Grupa 313CD*/

#include <stdlib.h>
#include "quaternarTree.h"

#include "queue.h"

Queue *initialiseQueue() {
    return calloc(1, sizeof(Queue));
}

static QueueNode *initialiseQueueNode(T value) {
    QueueNode *queuenode = malloc(sizeof(QueueNode));
    if (queuenode == NULL) {
        fprintf(stderr, "Cannot create queue node\n");
        exit(1);
    }

    queuenode->value = value;
    queuenode->next = NULL;

    return queuenode;
}

void enqueue(Queue *queue, T value) {
    if (queue == NULL) {
        fprintf(stderr, "Queue is not initialised\n");
        return;
    }

    if (queue->head == NULL) {
        queue->head = queue->tail = initialiseQueueNode(value);
        return;
    }

    queue->tail->next = initialiseQueueNode(value);
    queue->tail = queue->tail->next;
}

T dequeue(Queue *queue) {
    if (queue == NULL || queue->head == NULL) {
        fprintf(stderr, "Trying to dequeue from an empty queue\n");
        exit(1);
    }

    QueueNode *aux = queue->head;
    queue->head = aux->next;
    if (queue->head == NULL) {
        queue->tail = NULL;
    }

    T value = aux->value;
    free(aux);
    return value;
}

Queue *destroyQueue(Queue *queue) {
    while (queue->head) {
        QueueNode *aux = queue->head;
        queue->head = aux->next;
        free(aux);
    }

    free(queue);
    return NULL;
}