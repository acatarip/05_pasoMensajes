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

  if ((msqid = msgget(key, IPC_CREAT | 0666)) < 0){
    perror("msgget");
    exit(1);
  }

  printf("Servidor iniciado. Esperando mensajes...\n");

  // Atender a dos clientes
  for (int i = 0; i < 2; i++){

    if (msgrcv(msqid, &rbuf, MSGSZ, 1, 0) < 0){
      perror("msgrcv");
      exit(1);
    }

    printf("Mensaje recibido: %s\n", rbuf.mtext);

    if (strncmp(rbuf.mtext, "CLIENTE1:", 9) == 0){
      sbuf.mtype = 2;
      strcpy(sbuf.mtext, "Respuesta privada para el cliente 1");
    }
    else if (strncmp(rbuf.mtext, "CLIENTE2:", 9) == 0){
      sbuf.mtype = 3;
      strcpy(sbuf.mtext, "Respuesta privada para el cliente 2");
    }
    else{
      printf("Cliente desconocido\n");
      continue;
    }

    buf_lenght = strlen(sbuf.mtext) + 1;

    if (msgsnd(msqid, &sbuf, buf_lenght, IPC_NOWAIT) < 0){
      perror("msgsnd");
      exit(1);
    }

    printf("Respuesta enviada\n");
  }

  exit(0);
}

