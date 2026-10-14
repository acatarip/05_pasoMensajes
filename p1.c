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
  key_t key;
  message_buf rbuf, sbuf;
  size_t buf_lenght;
  
  key = 1234;
  
  // (Se agrega IPC_CREAT para que la cola se cree si aun no existe)
  if ((msqid = msgget(key, IPC_CREAT | 0666)) < 0){
    perror("msgget");
    exit(1);
  }

  // Proceso 1 envia el primer mensaje al proceso 2
  sbuf.mtype = 1;
  (void)strcpy(sbuf.mtext, "Are you hearing me?");
  buf_lenght = strlen(sbuf.mtext) + 1;

  if (msgsnd(msqid, &sbuf, buf_lenght, 0) < 0){
    perror("msgsnd");
    exit(1);
  }

  // Proceso 1 espera la respuesta del proceso 2 (tipo 2)
  if (msgrcv(msqid, &rbuf, MSGSZ, 2, 0) < 0){
    perror("msgrcv");
    exit(1);
  }
  printf("%s\n", rbuf.mtext);

  // Proceso 1 envia el segundo mensaje al proceso 2
  sbuf.mtype = 1;
  (void)strcpy(sbuf.mtext, "I can hear you too");
  buf_lenght = strlen(sbuf.mtext) + 1;

  if (msgsnd(msqid, &sbuf, buf_lenght, 0) < 0){
    perror("msgsnd");
    exit(1);
  }

  exit(0);
}

