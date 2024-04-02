#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

int koniec = 0;
pthread_t pid1, pid2;


//Funkcje drukujące komunikat co pewien czas
void * drukuj1(void * arg)
{
    while(1) {
        printf("Komunikat z wątku A\n");
        usleep(1110000);
    }
}

void * drukuj2(void * arg)
{
    while(1) {
        printf("Komunikat z wątku B\n");
        usleep(723000);
    }
}


int main(int argc, char *argv[])
{
    printf("dzien dobry\n");
    pthread_create(&pid1,NULL,drukuj1,NULL);
    pthread_create(&pid2,NULL,drukuj2,NULL);
    pthread_join(pid1,NULL);
    pthread_join(pid2,NULL);
    return 0;
}

