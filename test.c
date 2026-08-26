
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

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

void test()
{
    printf("test\n");
}

void    main(void)
{
    pthread_t t1, t2, t3, t4;

    // test();

    pthread_create(&t1, NULL, &function, "thread1\n");
    // sleep(1);
    pthread_create(&t2, NULL, &function, "thread2\n");
    pthread_create(&t3, NULL, &function, "thread3\n");
    pthread_create(&t4, NULL, &function, "thread4\n");

    usleep(200000);
    // pthread_join(t1, NULL);
    // pthread_join(t2, NULL);
    // pthread_join(t3, NULL);
    // pthread_join(t4, NULL);

    printf("%d", i);
}
