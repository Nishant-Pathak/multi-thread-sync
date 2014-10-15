#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#define MAX_THREAD 1000000

typedef struct DPKT {
   char *ch;
   int threadCount;
   int charCount;
   int index;
} dpkt;

pthread_mutex_t mutex[MAX_THREAD] = PTHREAD_MUTEX_INITIALIZER;

void *myPrinter(void * ptr) {
   dpkt *dp = (dpkt *) ptr;
   if(dp->index!=0) {
      pthread_mutex_lock(&mutex[dp->index]);
   }
   int printoff = dp->charCount * dp->index;
   for(;;) {
      pthread_mutex_lock(&mutex[dp->index]);

      printf("Thread%d: ",dp->index+1);
      int i ,k;
      for(i =0, k =0 ; i < dp->charCount; i ++) {
         int p = k + printoff;
         if(p >= strlen(dp->ch)) {k =0; p =0;} else {k++;};
         printf("%c",dp->ch[p]);
      }
      printf("\n");
      printoff = ( printoff  + dp->threadCount * dp->charCount) % strlen(dp->ch);
      
//    printf("Thread%d: %.*s\n",dp->index+1,dp->charCount,dp->ch + (dp->index * dp->charCount));
      pthread_mutex_unlock(&mutex[(dp->index+1)% (dp->threadCount)]);
   }
}

int main (int argc, char *argv[]) {
   if(argc != 4) {
      printf("Usage: [String] [Char Count] [Thread Count]\n");
      return 1;
   }
   int i;

   pthread_t *thread;
   char *ch = argv[1];
   unsigned long l = strlen(ch);
   int charCount = atoi(argv[2]);
   int threadCount = atoi(argv[3]);

   if(charCount< 0  ||
   threadCount < 0  ||
   threadCount > MAX_THREAD) {
      printf("Invalid value of Char Count or Thread Count\n");
      return 1;
   }
   thread = (pthread_t *) malloc(sizeof(pthread_t)*threadCount);
   if(thread == NULL) {
      printf("Memory Unavilable\n");
      return 1;
   }
   dpkt *dp =  malloc((sizeof(dpkt))*threadCount);
   if(dp == NULL) {
      printf("Memory Unavilable for dp\n");
      return 1;
   }
   for(i = 0; i < threadCount; i++) {
      dp[i].ch = ch;
      dp[i].index = i;
      dp[i].charCount = charCount;
      dp[i].threadCount = threadCount;
      pthread_create (&thread[i], NULL, myPrinter, (void *) &dp[i]);

   }
   for(i = 0; i < threadCount; i++) {
      pthread_join(thread[i],NULL);
   }
   return 0;
}

	


