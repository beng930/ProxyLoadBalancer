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

static void sendAndReceive(Packet request, int sockfd) {
    char send_data[SIZE_OF_MESSAGE];
    send_data[0] = request->type;
    send_data[1] = request->length;
    int clientSock = request->client_socket;

    /// Send waiting request from LB to server chosen
    int sent_bytes = 0;
    do {
        sent_bytes += send(sockfd, send_data+sent_bytes, SIZE_OF_MESSAGE-sent_bytes, 0);
    } while(sent_bytes<SIZE_OF_MESSAGE);

    /// Receive answer from server to LB
    char response[SIZE_OF_MESSAGE];
    int received_len = recv(sockfd, response, SIZE_OF_MESSAGE*sizeof(char), MSG_WAITALL);

    /// Send answer back to client
    sent_bytes = 0;
    do {
        sent_bytes += send(clientSock, response+sent_bytes, received_len-sent_bytes, 0);
    } while(sent_bytes<received_len);

    close(clientSock);
}

#endif /* CLIENT_C_ */
