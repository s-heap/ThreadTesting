#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>

volatile int global_number = 0;
pthread_mutex_t muxlock = PTHREAD_MUTEX_INITIALIZER;
int threadAmount = 2;
struct Queue *q;
int running = 1;

struct QNode {
    int key;
    struct QNode *next;
};

struct Queue {
    struct QNode *front, *rear;
};

struct QNode* newNode(int k) {
    struct QNode *temp = (struct QNode*)malloc(sizeof(struct QNode));
    temp->key = k;
    temp->next = NULL;
    return temp;
}

struct Queue *createQueue() {
    struct Queue *q = (struct Queue*)malloc(sizeof(struct Queue));
    q->front = q->rear = NULL;
    return q;
}

void enQueue(struct Queue *q, int k) {
    // Create a new LL node
    struct QNode *temp = newNode(k);

    // If queue is empty, then new node is front and rear both
    if (q->rear == NULL)
    {
    q->front = q->rear = temp;
    return;
    }

    // Add the new node at the end of queue and change rear
    q->rear->next = temp;
    q->rear = temp;
}

struct QNode *deQueue(struct Queue *q) {
    // If queue is empty, return NULL.
    if (q->front == NULL)
    return NULL;

    // Store previous front and move front one node ahead
    struct QNode *temp = q->front;
    q->front = q->front->next;

    // If front becomes NULL, then change rear also as NULL
    if (q->front == NULL)
    q->rear = NULL;
    return temp;
}

struct threadData {
    unsigned int threadID;
};

void *thread_code_dequeuer(void *arg) {

    struct threadData * data = (struct threadData *) arg;
    struct QNode *value;
    while (running) {
        pthread_mutex_lock(&muxlock);
        value = deQueue(q);
        pthread_mutex_unlock(&muxlock);
        if (value != NULL) {
            printf("I am thread %d and I have dequeued %d from the queue.\n", data->threadID, value->key);
        } else {
            printf("I am thread %d and the queue appears empty.\n", data->threadID);
            sleep(1);
        }
    }

}

int main(void) {
    q = createQueue();
    pthread_t threads[threadAmount];
    int i;

    for (i = 0; i < threadAmount; i++) {
        struct threadData *data = malloc(sizeof(struct threadData));
        data->threadID = i;
        pthread_create(&threads[i], NULL, &thread_code_dequeuer, (void *) data);
    }

    sleep(3);

    for (i = 0; i < 10; i++) {
        pthread_mutex_lock(&muxlock);
        enQueue(q, i);
        pthread_mutex_unlock(&muxlock);
    }

    /*
    pthread_t threads[threadAmount];
    int i;
    for (i = 0; i < threadAmount; i++) {
        struct threadData *data = malloc(sizeof(struct threadData));
        data->threadID = i;
        pthread_create(&threads[i], NULL, &thread_code, (void *) data);
    }*/
    /*
    for (i = 0; i < threadAmount+1; ++i) {
        void *rv;
        pthread_join(threads[i], &rv);
    }*/
    sleep(3);
    //running = 0;
    printf("All threads finished executing\n");
}
