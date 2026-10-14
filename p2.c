#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MSGSZ 128

// Elementos de un mensaje
typedef struct msgbuf {
  long mtype;
  char mtext[MSGSZ];
} message_buf;

int main(){
  int msqid;
  int msgflg =  IPC_CREAT | 0666;
  key_t key;
  message_buf rbuf, sbuf;
  size_t buf_lenght;
  
  key = 1234;
  
  if ((msqid = msgget(key, msgflg)) < 0){
    perror("msgget()");
    exit(1);
  }
  
  // Proceso 2 espera el primer mensaje del proceso 1 (tipo 1)
  if (msgrcv(msqid, &rbuf, MSGSZ, 1, 0) < 0){
    perror("msgrcv");
    exit(1);
  }
  printf("%s\n", rbuf.mtext);

  // Proceso 2 envia la respuesta (tipo 2)
  sbuf.mtype = 2;
  (void)strcpy(sbuf.mtext, "Loud and clear");
  buf_lenght = strlen(sbuf.mtext) + 1;
  
  if (msgsnd(msqid, &sbuf, buf_lenght, 0) < 0){
    perror("msgsnd");
    exit(1);
  }

  // Proceso 2 espera el segundo mensaje del proceso 1 (tipo 1)
  if (msgrcv(msqid, &rbuf, MSGSZ, 1, 0) < 0){
    perror("msgrcv");
    exit(1);
  }
  printf("%s\n", rbuf.mtext);

  exit(0);
}

