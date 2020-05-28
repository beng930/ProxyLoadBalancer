#ifndef SERVER_C_
#define SERVER_C_

#include "ClientConnections.h"

/*
 * wrapper for the parameters we want to send to the client_handle function, as it's the
 * first function of the threads
 */
struct handle_client_args {
    int socket;
};

/*
 *
 */
static void* client_handle(void *args);

/*
 *
 */
static Servers findBestServer(Packet request);

/*
 *
 */
static int calculateCost(Servers server,char type,int init_cost);

void* ClientConnections(void *args) {
    struct sockaddr_in sa, ca;
    int server_socket, client_socket;
    pthread_t worker_thread;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        fprintf(stderr, "Socket creation error \n");
        pthread_exit(NULL);
    }

    sa.sin_port = htons(atoi(PORT));
    sa.sin_family = AF_INET;
    inet_pton(AF_INET, LB_SERVER_IP, &(sa.sin_addr));
    if (bind(server_socket, (struct sockaddr *)&sa,sizeof(sa)) < 0){
        close(server_socket);
        perror("Error: ");
        fprintf(stderr, "ERROR on binding\n");
        pthread_exit(NULL);
    }

    if (listen(server_socket, BACKLOG) < 0){
        close(server_socket);
        fprintf(stderr, "ERROR on listening");
        pthread_exit(NULL);
    }

    int cl = sizeof(ca);

    while(1){
        //server main loop
        client_socket = accept(server_socket, (struct sockaddr*) &ca, &cl);
        if (client_socket < 0){
            close(server_socket);
            fprintf(stderr, "ERROR on accept");
            pthread_exit(NULL);
        }

        struct handle_client_args* c_sock = malloc (sizeof(*c_sock));
        c_sock->socket = client_socket;

        if (pthread_create(&worker_thread, NULL, client_handle, c_sock) != 0) {
            close(client_socket);
            close(server_socket);
            fprintf(stderr, "ERROR on pthread");
            pthread_exit(NULL);
        }
    }
    pthread_exit(NULL);
    return NULL;//just to have no annoying warnings
}

static void* client_handle(void *args) {
    struct handle_client_args* c_sock = (struct handle_client_args*) args;
    int client_socket = c_sock->socket;
    free(c_sock);

    pthread_detach(pthread_self());

    char input[SIZE_OF_MESSAGE];
    recv(client_socket,input,SIZE_OF_MESSAGE*sizeof(char),MSG_WAITALL);
    Packet new_request = malloc(sizeof(*new_request));
    new_request->client_socket = client_socket;
    new_request->type = input[0];
    new_request->length = input[1];

    for(int i=0; i<NUM_OF_SERVERS; i++){
        pthread_mutex_lock(&mutexes[i]);
    }
    Servers best_fit_server = findBestServer(new_request);
    new_request->actual_time = calculateCost(best_fit_server,new_request->type,atoi(&new_request->length));
    new_request->priority = new_request->actual_time;
    queuePush(servers_queue[best_fit_server],new_request);
    for(int i=NUM_OF_SERVERS-1; i>=0; i--){
        pthread_mutex_unlock(&mutexes[i]);
    }

    pthread_exit(NULL);
    return NULL;//just to have no annoying warnings
}

static Servers findBestServer(Packet request) {
    Servers best_fit = Server1;
    int min_cost = calculateCost(Server1,request->type,atoi(&request->length));
    min_cost += queueGetTotalTime(servers_queue[best_fit]);
    //iterate all servers to find best fit, put the request in the queue
    for(int i=1; i<NUM_OF_SERVERS; i++) {
        int temp_cost = calculateCost(i,request->type,atoi(&request->length));
        temp_cost += queueGetTotalTime(servers_queue[i]);
        if(min_cost>temp_cost) {
            //this queue is more fit for this job
            best_fit = i;
            min_cost = temp_cost;
        }
    }
    return best_fit;
}

static int calculateCost(Servers server, char type, int init_cost) {
    if(getIndex(server) <= 6)
    {
        if(type == 'V' || type == 'P') return init_cost;
        if(type == 'M') return 2*init_cost;
    }
    else
    {
        if(type == 'M') return init_cost;
        if(type == 'V') return 3*init_cost;
        if(type == 'P') return 2*init_cost;
    }
    return -1;
}

Servers getEnumerator(int index)
{
    switch (index)
    {
        case 0: return Server1;
        case 1: return Server2;
        case 2: return Server3;
        case 3: return Server4;
        case 4: return Server5;
        case 5: return Server6;
        case 6: return Server7;
        case 7: return Server8;
        case 8: return Server9;
        case 9: return Server10;
    }
}
int getIndex(Servers server)
{
    switch (server)
    {
        case Server1: return 0;
        case Server2: return 1;
        case Server3: return 2;
        case Server4: return 3;
        case Server5: return 4;
        case Server6: return 5;
        case Server7: return 6;
        case Server8: return 7;
        case Server9: return 8;
        case Server10: return 9;
    }
}

#endif /* SERVER_C_ */
