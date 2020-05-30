#include <stdio.h>
#include "PriorityQueue.h"

int main() {
    Queue q = queueCreate();
    Packet p = malloc(sizeof(*p));
    p->type = 'M';
    p->length = '3';
    p->priority = 6;
    p->actual_time = 6;
    p->client_socket = 4;

    Packet p3 = malloc(sizeof(*p));
    p3->type = 'M';
    p3->length = '3';
    p3->priority = 6;
    p3->actual_time = 6;
    p3->client_socket = 4;

    Packet p1 = malloc(sizeof(*p1));
    p1->type = 'M';
    p1->length = '3';
    p1->priority = 5;
    p1->actual_time = 5;
    p1->client_socket = 4;

    Packet p2 = malloc(sizeof(*p2));
    p2->type = 'M';
    p2->length = '3';
    p2->priority = 3;
    p2->actual_time = 3;
    p2->client_socket = 4;

    queuePush(q, p);
    printQueue(q);
    queuePush(q, p3);
    printQueue(q);
    queuePush(q, p1);
    printQueue(q);
    queuePush(q, p2);
    printQueue(q);

    queuePop(q);
    printQueue(q);
    queueRemove(q, p3);
    queueRemove(q, p);
    printQueue(q);
}
