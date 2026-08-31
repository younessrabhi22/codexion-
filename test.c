
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include <sys/time.h>
#include "codexion.h"


int i = 10;
pthread_mutex_t lock;


void *function(void *str)
{

    for (int j = 0; j < 1000000; j++)
    {
       pthread_mutex_lock(&lock);
       i++;
       pthread_mutex_unlock(&lock);
    }

    printf("%s", (char *)str);
    return NULL;

}

void *test1()
{
    printf("thrad3\n");
}

void *test2()
{
    printf("thrad1\n");
}

void *test3()
{
    printf("thrad2\n");
}



pthread_mutex_t mutex1;

int var = 0;

void *function1()
{
    pthread_mutex_lock(&mutex1);
    while (var > 1)
    {
        printf("function executing...\n");
        var+=1;
        sleep(1);
    }
    pthread_mutex_unlock(&mutex1);
}


void    main(void)
{

    pthread_t th1, th2;

    pthread_mutex_init(&mutex1, NULL);

    pthread_create(&th1, NULL, &function1, NULL);
    pthread_create(&th2, NULL, &function1, NULL);


    pthread_join(th1, NULL);
    pthread_join(th2, NULL);

    pthread_mutex_destroy(&mutex1);



    printf("%d\n", i);
    printf("exit the program\n");


    // pthread_t t1, t2, t3, t4;

    // // test();
    // pthread_mutex_init(&lock, NULL);
    // // pthread_create(&t1, NULL, &function, "thread1\n");
    // // // sleep(1);
    // // pthread_create(&t2, NULL, &function, "thread2\n");
    // // pthread_create(&t3, NULL, &function, "thread3\n");
    // // pthread_create(&t4, NULL, &function, "thread4\n");

    // pthread_create(&t1, NULL, &test1, NULL);
    // pthread_create(&t2, NULL, &test2, NULL);
    // pthread_create(&t3, NULL, &test3, NULL);

    // // usleep(200000);
    // pthread_join(t1, NULL);
    // pthread_join(t2, NULL);
    // pthread_join(t3, NULL);
    // // pthread_join(t4, NULL);


    // pthread_mutex_destroy(&lock);

    // struct timeval tv;

    // gettimeofday(&tv, NULL);

    // printf("%ld", tv.tv_sec);
    // printf("\n");
    // sleep(2);
    // printf("%ld", tv.tv_sec);

    // printf("%d", i);
    // pthread_cond_broadcast()
    // pthread_cond_wait()
    // pthread_cond_init()
    // pthread_cond_destroy()
    // pthread_cond_signal()
}
