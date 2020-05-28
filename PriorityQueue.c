#ifndef PRIOQUEUE_C_
#define PRIOQUEUE_C_

#include "PriorityQueue.h"

typedef struct Node_t {
	Packet data;
	struct Node_t* prev;
	struct Node_t* next;
}* Node;

typedef struct Queue_t {
	Node head;
	Node tail;
	int size;
	int queueTotalTime;
}* Queue;

Queue queueCreate() {
	Queue q = malloc(sizeof(*q));
	q->head=NULL;
	q->tail=NULL;
	q->size=0;
	q->queueTotalTime=0;
	return q;
}

int queueGetSize(Queue q)
{
    return q->size;
}

// Returns the total time it will take to process the entire queue by the server.
int queueGetTotalTime(Queue q)
{
    return q->queueTotalTime;
}

// Returns the first element in the queue.
Packet queueTop(Queue q)
{
    if (queueIsEmpty(q))
    {
        return NULL;
    }
    return q->head->data;
}

// Removes the first element in the queue.
Packet queuePop(Queue q)
{
    Packet dataToReturn = NULL;
    if (queueIsEmpty(q))
    {
        return dataToReturn;
    }
    Node headToRemove = q->head;
    dataToReturn = headToRemove->data;
    if (q->size == 1)
    {
        q->tail = NULL;
    }
    q->head = q->head->next;
    q->queueTotalTime-= dataToReturn->actual_time;
    q->size = q->size-1;
    free(headToRemove);
    Node temp = q->head;
    while (temp)
    {
        temp->data->priority = (temp->data->priority-= 0.5*dataToReturn->actual_time) > 0? temp->data->priority-= 0.5*dataToReturn->actual_time : 0;
        temp = temp->next;
    }
    return dataToReturn;
}

// Removes @param "to_remove" from the queue
void queueRemove(Queue q, Packet toRemove)
{
    if (q->head->data == toRemove)
    {
        queuePop(q);
        return;
    }
    Node temp = q->head->next;
    while (temp)
    {
        if (temp->data == toRemove)
        {
            temp->prev->next = temp->next;
            if (temp == q->tail)
            {
                q->tail = temp->prev;
            }
            else
            {
                temp->next->prev = temp->prev;
            }
            free(temp);
            q->queueTotalTime-= toRemove->actual_time;
            q->size = q->size - 1;
            Node temp = q->head;
            while (temp)
            {
                temp->data->priority = (temp->data->priority-= 0.5*toRemove->actual_time) > 0? temp->data->priority-= 0.5*toRemove->actual_time : 0;
                temp = temp->next;
            }
            return;
        }
        temp = temp->next;
    }
}

// Inserts new element to the queue, keeping the queue sorted (head is the shortest process).
void queuePush(Queue q, Packet data)
{
    Node inserted = malloc(sizeof(*inserted));
    inserted->prev = NULL;
    inserted->next = NULL;
    inserted->data = data;
    if (queueIsEmpty(q))
    {
        q->head = inserted;
        q->tail = inserted;
    }
    else
    {
        ///search for where we should insert
        Node temp = q->head;
        while (temp && temp->data->priority < inserted->data->priority)
        {
            temp = temp->next;
        }
        if (!temp)
        {
            q->tail->next = inserted;
            inserted->prev = q->tail;
            q->tail = inserted;
        }
        else
        {
            if (temp != q->head)
            {
                temp->prev->next = inserted;
                inserted->prev = temp->prev;
            }
            else
            {
                q->head = inserted;
            }
            temp->prev = inserted;
            inserted->next = temp;
        }
    }
    q->size = q->size + 1;
    q->queueTotalTime+= data->actual_time;
}

// Returns true if there are no elements in the queue.
bool queueIsEmpty(Queue q)
{
    return q->size == 0;
}

// Frees all elements memory and the queue's memory.
void queueDestroy(Queue q)
{
    while (!queueIsEmpty(q))
    {
        free (queuePop(q));
    }
    free(q);
}

// Prints all the elements in the queue.
void printQueue(Queue q)
{
    Node temp = q->head;
    Packet data = temp->data;
    while(temp) {
        data = temp->data;
        printf("(%c%c|%d, %f)", data->type,data->length,data->actual_time, data->priority);
        temp = temp->next;
        if(temp) printf(", ");
        else printf(".");
    }
    printf("\n");
}

void UpdateQueuePriority(Node head, Packet p)
{
    while (head)
    {
        head->data->priority = (head->data->priority-= 0.5*p->actual_time) > 0? head->data->priority-= 0.5*p->actual_time : 0;
        head = head->next;
    }
}

#endif /* PRIOQUEUE_C_ */
