#ifndef SERVERCONNECTIONS_H_
#define SERVERCONNECTIONS_H_

#include "ClientConnections.h"

/*
 * wrapper of parameters to send to client as a thread's first function
 */
struct ServerArgWrapper {
    char* serverIP; //The IP address of the server which we will connect to
    Servers server; //The server that we will connect to
};

#define SERV1_IP "192.168.0.101"
#define SERV2_IP "192.168.0.102"
#define SERV3_IP "192.168.0.103"
#define SERV4_IP "192.168.0.104"
#define SERV5_IP "192.168.0.105"
#define SERV6_IP "192.168.0.106"
#define SERV7_IP "192.168.0.107"
#define SERV8_IP "192.168.0.108"
#define SERV9_IP "192.168.0.109"
#define SERV10_IP "192.168.0.100"

/*
 *	@param args - assumed to be struct client_to_servers_args
 *	Runs one thread, and handles the connection with one back end server. It takes the first request of
 *	the priority queue and send it to the server. When it receives a response, it sends it back to the
 *	client that sent this request in the first place.
 */
void* ServerConnections(void *args);

#endif /* SERVERCONNECTIONS_H_ */
