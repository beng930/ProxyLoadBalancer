#ifndef SERVERCONNECTIONS_H_
#define SERVERCONNECTIONS_H_

#include "ClientConnections.h"

/*
 * Wrapper of server parameters
 */
struct ServerArgWrapper {
    char* serverIP;
    Servers server;
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
 *	@param args = struct ServerArgWrapper
 *	Runs the server thread for handling requests coming through the LB from clients
 */
void* ServerConnections(void *args);

#endif /* SERVERCONNECTIONS_H_ */
