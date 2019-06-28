#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <signal.h>
#include "semaforo.h"


#define atomic_xchg(A,B) __asm__ __volatile__( \
                                        " lock xchg %1,%0 ;\n" \
                                        : "=ir" (A) \
                                        : "m" (B), "ir" (A) \
                                        );

char *pais[3]={"Peru","Bolvia","Colombia"};
int *g;
semaforo *s;

void _enqueue(QUEUE *q,int val)
{
	q->elements[q->head]=val;
	// Incrementa al apuntador
	q->head++;
	q->head=q->head%MAXTHREAD;
}


int _dequeue(QUEUE *q)
{
	int valret;
	valret=q->elements[q->tail];
	// Incrementa al apuntador
	q->tail++;
	q->tail=q->tail%MAXTHREAD;
	return(valret);
}

int _emptyq(QUEUE *q)
{
	return(q->head==q->tail);
}

void initsem(semaforo *s, int val){
	int l=1;
	do{ atomic_xchg(l,*g);} while(l!=0);
	s->val = val;
	s->stoped.head=0;
	s->stoped.tail=0;
	*g = 0;
}

void signalsem(semaforo *s){

	int l=1;

	do{ atomic_xchg(l,*g);} while(l!=0);
	s->val++;
	if(_emptyq(&s->stoped)){
		*g=0;
		return;
	}
	int pid = _dequeue(&s->stoped);
	kill(pid,SIGCONT);
	*g=0;
}

void waitsem(semaforo *s){
	int l;

	while(1){
		l=1;
		do{
			atomic_xchg(l,*g);
		} while(l!=0);
		if(s->val > 0){
			s->val--;
			*g=0;
			return;
		}
		else{
			int x = (int) getpid();
			_enqueue(&s->stoped,x);
			*g=0;
			kill(getpid(),SIGSTOP);

		}
	}
}


void proceso(int i)
{
  int k;
  for(k=0;k<CICLOS;k++)
  {
    waitsem(s);

    printf("Entra %s",pais[i]);
    fflush(stdout);
    sleep(rand()%3);
    printf("- %s Sale\n",pais[i]);

    signalsem(s);
    // Espera aleatoria fuera de la sección crítica
    sleep(rand()%3);
  }
  exit(0); // Termina el proceso
}

int main()
{
  int pid;
  int status;
  int shmid, shmidsem;
  int i;
  // Solicitar memoria compartida
  shmid=shmget(0x1234,sizeof(g),0666|IPC_CREAT);
  if(shmid==-1)
  {
    perror("Error en la memoria compartida\n");
    exit(1);
    }
	shmidsem=shmget(0x1235,sizeof(s),0666|IPC_CREAT);
	if(shmidsem==-1)
	{
		perror("Error en la memoria compartida\n");
		exit(1);
		}
// Conectar la variable a la memoria compartida
  g=shmat(shmid,NULL,0);
  if(g==NULL)
  {
  perror("Error en el shmat\n");
  	exit(2);
  }
	s=(semaforo *)shmat(shmidsem,NULL,0);
	if(s==NULL)
  {
  	perror("Error en el shmat\n");
  	exit(2);
  }

  *g=0;
	initsem(s, 1);
  srand(getpid());
  for(i=0;i<3;i++)
  {
// Crea un nuevo proceso hijo que ejecuta la función proceso()
	  pid=fork();
	  if(pid==0)
	    proceso(i);
  }
  for(i=0;i<3;i++)
    pid = wait(&status);
  // Eliminar la memoria compartida
  shmdt(g);
	return 0;
}
