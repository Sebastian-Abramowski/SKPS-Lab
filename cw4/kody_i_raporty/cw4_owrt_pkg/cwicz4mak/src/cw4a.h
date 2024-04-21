#ifndef __CW4_INCLUDE__
#define __CW4_INCLIDE__
#define MSG_LEN 16
#define MAX_CLIENTS 16

#define SHM_CW4_NAME "/skps4shm"
struct msg{
  struct timeval tstamp;
  uint32_t data[MSG_LEN];
};

#define BUF_LEN 2048

struct ringbuf{
  int magick;
  //Mutex for data producer and consumers
  //pthread_rwlock_t buflock;
  //Condition variable for notifying the consumers.
  pthread_mutex_t cvar_lock;
  pthread_cond_t cvar;
  int head;
  int tail[MAX_CLIENTS];
  struct msg buf[BUF_LEN];
};

#define SHM_LEN sizeof(struct ringbuf)
#endif

