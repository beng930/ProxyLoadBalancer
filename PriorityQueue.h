#ifndef PRIO_QUEUE_H_
#define PRIO_QUEUE_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct Queue_t* Queue;

typedef struct Packet_t {
	char type; // 'M' = music, 'V' = video, 'P' = picture
	char length; // number 0-9, as the time it supposed to take to process this media normally.
	int actual_time; // the time it will actually take to process this packet - depends on the server
	int client_socket; // the socket, that we got this packet request from, in the first place
	double priority; // The priority of this request. Starts with value = actual_time and is dynamic to prevent starvation
}* Packet;


Queue queueCreate();

int queueGetSize(Queue q);

// Returns the total time it will take to process the entire queue by the server.
int queueGetTotalTime(Queue q);

// Returns the first element in the queue.
Packet queueTop(Queue q);

// Removes the first element in the queue.
Packet queuePop(Queue q);

// Removes @param "to_remove" from the queue
void queueRemove(Queue q, Packet to_remove);

// Inserts new element to the queue, keeping the queue sorted (head is the shortest process).
void queuePush(Queue q, Packet p);

// Returns true if there are no elements in the queue.
bool queueIsEmpty(Queue q);

// Frees all elements memory and the queue's memory.
void queueDestroy(Queue q);

// Prints all the elements in the queue.
void printQueue(Queue q);


#endif /* PRIO_QUEUE_H_ */
