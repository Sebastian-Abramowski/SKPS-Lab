// SKPS 2022 Ex 4, demo code by WZab
// Sources of the data consumer
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>
#include <math.h>
#include "cw4a.h"

volatile int dummy=0; //Used to simulate processing delay

int main(int argc, char *argv[])
{
    int smpnum;
    unsigned long smptime,deliverytime;    
    int fd;
    double tdel;
    int ncli;
    int nsmp;
    int ndel;
    int fout;
    ncli = atoi(argv[1]);
    nsmp = atoi(argv[2]);    
    ndel = atoi(argv[3]);
    printf("Client: %d, nsmp=%d, del=%d\n",ncli,nsmp,ndel);
    //Create the report file
    char fname[20];
    sprintf(fname,"cli_%d.txt",ncli); //Watch out to not overflow fname!
    fout=open(fname,O_WRONLY | O_CREAT | O_TRUNC,S_IRUSR | S_IWUSR);
    assert(fout>=0);
    //Open shared memory
    fd = shm_open(SHM_CW4_NAME, O_RDWR, S_IRUSR | S_IWUSR);
    assert(fd>=0);
    //Map the shared memory
    void * mptr = mmap(NULL, SHM_LEN, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    assert(mptr != MAP_FAILED);
    struct ringbuf * rbuf = (struct ringbuf *) mptr;
    //Check if the memory is initialized
    assert(rbuf->magick == 0x12345678);
    //Now we can start receiving the data
    smpnum=0;
    while(smpnum<nsmp) {
        int j;
        char line[200];
        //pthread_rwlock_rdlock(&rbuf->buflock);
        //check if there is any the new data
        pthread_mutex_lock(&rbuf->cvar_lock);
        if(rbuf->head != rbuf->tail[ncli]) {
            struct timeval tv1,tv2;
            gettimeofday(&tv1,NULL);
            //memcpy(&tv2,&rbuf->buf[rbuf->tail].tstamp);
            tv2 = rbuf->buf[rbuf->tail[ncli]].tstamp;
            rbuf->tail[ncli]++;
            if (rbuf->tail[ncli] == BUF_LEN) rbuf->tail[ncli] = 0;
            pthread_mutex_unlock(&rbuf->cvar_lock);
            smptime = 1000000*tv2.tv_sec + tv2.tv_usec;
            deliverytime = 1000000*tv1.tv_sec + tv1.tv_usec;           
            int tdel = deliverytime - smptime;
            printf("Sample %d, client %d, delivery time: %d\n",smpnum,ncli,tdel);
            sprintf(line,"%d, %lu, %lu, %d\n",smpnum,smptime,deliverytime,tdel);
            //The next instruction is an example of incorrect implementation!
            //Now we only detect possible error, but we should also check the number of written bytes
            //and repeat writing if only part of the line was written?
            assert(write(fout,line,strlen(line))>0); 
            sync();
            smpnum++;
            //Here we simulate delay for data processing
            for(j=0;j<ndel;j++)
               dummy++;            
        } else {
            pthread_cond_wait(&rbuf->cvar,&rbuf->cvar_lock);
            pthread_mutex_unlock(&rbuf->cvar_lock);
        }
    }
    munmap(mptr,SHM_LEN);
    shm_unlink(SHM_CW4_NAME);
    close(fout);
    return 0;
}

