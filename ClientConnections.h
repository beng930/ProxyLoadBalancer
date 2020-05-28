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
 *  Serv 1 & 2 = Video type = x2 Music
 *  Serv3 = Music type = x3 Video, x2 Picture
 */
typedef enum servers_t {Server1, Server2, Server3, Server4, Server5,
                        Server6, Server7, Server8, Server9, Server10, NUM_OF_SERVERS} Servers;

Servers getEnumerator(int index);

/*
 * priority queues - one for each server, to make it take the decision of what is the next packet to send
 */
Queue servers_queue[NUM_OF_SERVERS];

/*
 * all mutexes, one for each priority queue (meaning one for each server's queue)
 */
pthread_mutex_t mutexes[NUM_OF_SERVERS];

/*
 *	@param args - assumed to be NULL
 *	Runs a thread that acts as a front end of the Load Balancer. It handles all the connections with the
 *	outer world, i.e. receives all the requests' messages from clients, and send it to the back end servers
 *	according to a specific balancing policy.
 */
void* ClientConnections(void *args);

#endif /* ClientConnections_H_ */
