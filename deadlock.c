#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

pthread_mutex_t mut1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mut2 = PTHREAD_MUTEX_INITIALIZER;

void waitFirstMutex(){
    pthread_mutex_lock(&mut1);
    printf("waitFirstMutex\n");
    sleep(5);
    pthread_mutex_lock(&mut2);
    
    
    //for (int i = 0; i < 9999999; i++);
    pthread_mutex_unlock(&mut2);    
    pthread_mutex_unlock(&mut1);
}

void waitSecondMutex(){
    pthread_mutex_lock(&mut2);
    printf("waitSecondMutex\n");
    fflush(NULL);
    sleep(5);
    pthread_mutex_lock(&mut1);   
    
    
    //for (int i = 0; i < 9999999; i++);
    pthread_mutex_unlock(&mut1);
    pthread_mutex_unlock(&mut2); 
}

int main(){
    pthread_t thread1;
    pthread_t thread2;
    
    pthread_create(&thread1, NULL, (void*)waitSecondMutex, NULL);
    pthread_create(&thread2, NULL, (void*)waitFirstMutex, NULL);
    
    pthread_join(&thread1, NULL);
    pthread_join(&thread2, NULL);
    
    return 0;
}
