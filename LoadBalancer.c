#include "ServerConnections.h"

int main(void) {
    for(int i=0; i<NUM_OF_SERVERS; i++){
        // priority queue of requests for each server
        serversQueue[i] = queueCreate();
        // locks to handle reading and writing to the priority queues
        pthread_mutex_init(&mutexes[i], NULL);
    }

    char* serversIPs[10] = {SERV1_IP, SERV2_IP, SERV3_IP, SERV4_IP, SERV5_IP, SERV6_IP, SERV7_IP, SERV8_IP, SERV9_IP, SERV10_IP};
    pthread_t serverThread[10];

    for (int i=0; i<NUM_OF_SERVERS; i++)
    {
        struct ServerArgWrapper* serverArgWrapper = malloc(sizeof(*serverArgWrapper));
        serverArgWrapper->server = getEnumerator(i);
        serverArgWrapper->serverIP = serversIPs[i];

        if (pthread_create(&serverThread[i], NULL, ServerConnections, serverArgWrapper) != 0)
        {
            fprintf(stderr, "ERROR - couldn't open server thread");
            free(serverArgWrapper);
            return -1;
        }
    }

    pthread_t clientThread;
    if (pthread_create(&clientThread, NULL, ClientConnections, NULL) != 0)
    {
        fprintf(stderr, "ERROR - couldn't open client thread");
        return -1;
    }

    for (int i=0; i<NUM_OF_SERVERS; i++)
    {
        pthread_join(serverThread[i], NULL);
    }
    pthread_join(clientThread,NULL);

    // destroy all mutexes and queues
    for(int i=0; i<NUM_OF_SERVERS; i++){
        queueDestroy(serversQueue[i]);
        pthread_mutex_destroy(&mutexes[i]);
    }

    return 0;
}
