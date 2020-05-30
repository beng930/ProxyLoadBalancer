#ifndef ClientConnections_H_
#define ClientConnections_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>
#include "PriorityQueue.h"

#define PORT "80"
#define BACKLOG 10
#define LB_SERVER_IP "10.0.0.1"
#define LB_CLIENT_IP "192.168.0.1"
#define SIZE_OF_MESSAGE 2 // messages are expected to be type and length, one char each.

/*
 *  Server 1-6 = Video type = x2 Music
 *  Server 7-10 = Music type = x3 Video, x2 Picture
 */
typedef enum servers_t {Server1, Server2, Server3, Server4, Server5,
                        Server6, Server7, Server8, Server9, Server10, NUM_OF_SERVERS} Servers;

Servers getEnumerator(int index);
int getIndex(Servers server);

/*
 * Priority queues for servers
 */
Queue serversQueue[NUM_OF_SERVERS];

/*
 * Mutexes for each server queue
 */
pthread_mutex_t mutexes[NUM_OF_SERVERS];

/*
 *	@param args = NULL
 *	Runs the client thread for handling requests according to power 2 implementation
 */
void* ClientConnections(void *args);

#endif /* ClientConnections_H_ */
