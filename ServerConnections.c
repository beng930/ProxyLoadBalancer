#ifndef CLIENT_C_
#define CLIENT_C_

#include "ServerConnections.h"

/*
 *
 */
static void sendAndReceive(Packet request, int client_lb_fd);

void* ServerConnections(void *args) {
    struct ServerArgWrapper* s_args = (struct ServerArgWrapper*) args;
    char* serverIP = s_args->serverIP;
    Servers server = s_args->server;
    free(s_args);

    int sockfd, portno;
    struct sockaddr_in servAddr;

    portno = atoi(PORT);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        fprintf (stderr, "ERROR opening socket");
        pthread_exit(NULL);
    }

    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(portno);
    inet_pton(AF_INET, serverIP, &(servAddr.sin_addr));

    if (connect(sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
    {
        fprintf(stderr, "ERROR connecting to server %s", serverIP);
        pthread_exit(NULL);
    }

    /// Access  relevant server queue
    Queue waitingRequests = serversQueue[server];

    /// Do until connection is terminated
    while(1)
    {
        pthread_mutex_lock(&mutexes[server]);
        if (!queueIsEmpty(waitingRequests))
        {
            /// Top for error handling - we wish to save the data until work is done
            Packet request = queueTop(waitingRequests);
            pthread_mutex_unlock(&mutexes[server]);
            sendAndReceive(request, sockfd);
            pthread_mutex_lock(&mutexes[server]);
            queueRemove(waitingRequests, request);
            free(request);
        }
        pthread_mutex_unlock(&mutexes[server]);
    }

    pthread_exit(NULL);
    return NULL;//just to have no annoying warnings
}

static void sendAndReceive(Packet request, int sockFd) {
    char DataToSend[SIZE_OF_MESSAGE];
    DataToSend[0] = request->type;
    DataToSend[1] = request->length;
    int clientSock = request->client_socket;

    /// Send waiting request from LB to server chosen
    int sentBytes = 0;
    do {
        sentBytes += send(sockFd, DataToSend+sentBytes, SIZE_OF_MESSAGE-sentBytes, 0);
    } while(sentBytes<SIZE_OF_MESSAGE);

    /// Receive answer from server to LB - blocking call
    char response[SIZE_OF_MESSAGE];
    int MessageReceivedLength = recv(sockFd, response, SIZE_OF_MESSAGE*sizeof(char), MSG_WAITALL);

    /// Send answer back to client
    sentBytes = 0;
    do {
        sentBytes += send(clientSock, response+sentBytes, MessageReceivedLength-sentBytes, 0);
    } while (sentBytes < MessageReceivedLength);

    close(clientSock);
}

#endif /* CLIENT_C_ */
