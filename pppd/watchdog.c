#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <signal.h>
#include "watchdog.h"
#include "pppd.h"

int Watchdog_cnt;
int Watchdog_stop;
int Watchdog_time;

void *thread_watchdog(void *arg);

void watchdog_reset()
 {Watchdog_cnt = 0;
  info("### WATCHDOG: Reset!\n",Watchdog_time);
 }

void watchdog_stop()
 {Watchdog_stop=(Watchdog_stop == 2)?2:1; //Thread ist schon beendet
  info("### WATCHDOG: Stop!\n",Watchdog_time);
  while(Watchdog_stop != 2)
   {sleep(1);
   }
  info("### WATCHDOG: Stopped!\n",Watchdog_time);
 }

void watchdog_start(int time)
 {pthread_t a_thread;
  int res;

  Watchdog_time = time;
  Watchdog_stop = 0;

  info("### WATCHDOG: Start %d seconds\n",Watchdog_time);

  res = pthread_create(&a_thread, NULL, thread_watchdog, (void*)&Watchdog_time);

  if(res != 0)
   {info("### WATCHDOG: Thread creation failed!\n");
   }
 }

void *thread_watchdog(void *arg)
 {pid_t pid = getpid();

  Watchdog_cnt = 0;

  info("### WATCHDOG: Thread started! pid watchdog: %d , watchdog_time: %d\n",pid,*((int*)arg));

  while((Watchdog_cnt < Watchdog_time) && (Watchdog_stop == 0))
   {sleep(1);
    Watchdog_cnt++;
   }

  if(Watchdog_stop == 0)
    {info("### WATCHDOG: KILL SIGINT ###\n");
     kill(pid,SIGINT);
//        sleep(10);
//        info("### WATCHDOG: SIGTERM ###\n");
//        kill(pid,SIGTERM);
//        sleep(10);
//        info("### WATCHDOG: SIGKILL ###\n");
//        kill(pid,SIGKILL);
//        sleep(10);

    }
  info("### WATCHDOG: Thread end!\n");
  Watchdog_stop=2;
 }
