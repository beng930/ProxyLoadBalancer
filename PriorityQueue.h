#ifndef PRIO_QUEUE_H_
#define PRIO_QUEUE_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct Queue_t* Queue;

typedef struct Packet_t {
	char type; // 'M' = music, 'V' = video, 'P' = picture.
	char length; // Normal process time of packet.
	int actual_time; // Actual process time of packet - according to server.
	int client_socket; // The client socket the packet came from.
	double priority; // The priority of this request.
}* Packet;


Queue queueCreate();

int queueGetSize(Queue q);

// Returns the total time it will take to process the entire queue.
int queueGetTotalTime(Queue q);

// Returns the first element in the queue.
Packet queueTop(Queue q);

// Removes the first element in the queue.
Packet queuePop(Queue q);

// Removes @param "to_remove" from the queue
void queueRemove(Queue q, Packet to_remove);

// Inserts @param "p" to the queue, keeping the queue sorted.
void queuePush(Queue q, Packet p);

// Returns true if there are no elements in the queue.
bool queueIsEmpty(Queue q);

void queueDestroy(Queue q);

void printQueue(Queue q);


#endif /* PRIO_QUEUE_H_ */
