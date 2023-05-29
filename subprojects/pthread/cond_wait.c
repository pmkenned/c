#include <stdio.h>
#include <stddef.h>
#include <pthread.h>
#include <assert.h>

pthread_mutex_t mutex;
pthread_cond_t cond;

enum { OWNER_A='A', OWNER_B='B' } mutex_owner;

static void
initialize() {
    pthread_mutex_init(&mutex, NULL) ;
    pthread_cond_init(&cond, NULL) ;
    mutex_owner = OWNER_A;
}

static void
waitForOwnership(int owner){
    pthread_mutex_lock(&mutex);
    while (owner != mutex_owner) {
        pthread_cond_wait(&cond, &mutex);
    }
}

static void
dropOwnership(int owner){
    assert(owner == mutex_owner);
    switch (owner) {
        case OWNER_A: mutex_owner = OWNER_B; break ;
        case OWNER_B: mutex_owner = OWNER_A; break ;
        default: assert(0);
    }
    pthread_mutex_unlock(&mutex);
    pthread_cond_signal(&cond);
}

static void *
thread_A_proc(void * arg)
{
    for (int i = 0; i < 10; i++) {
        waitForOwnership(OWNER_A);
        fprintf(stderr, "thread A\n");
        dropOwnership(OWNER_A);
    }
    return NULL;
}

static void *
thread_B_proc(void * arg)
{
    for (int i = 0; i < 10; i++) {
        waitForOwnership(OWNER_B);
        fprintf(stderr, "thread B\n");
        dropOwnership(OWNER_B);
    }
    return NULL;
}

int main()
{
    initialize();
    pthread_t thread_A, thread_B;
    pthread_create(&thread_A, NULL, thread_A_proc, NULL);
    pthread_create(&thread_B, NULL, thread_B_proc, NULL);
    pthread_join(thread_A, NULL);
    pthread_join(thread_B, NULL);
}
