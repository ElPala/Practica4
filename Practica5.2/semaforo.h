
#define MAXTHREAD 50
#define CICLOS 3


typedef struct _QUEUE {
	int elements[MAXTHREAD];
	int head;
	int tail;
} QUEUE;

typedef struct _semaforo {
	QUEUE stoped;
  int val;
} semaforo;

void _enqueue(QUEUE *q,int val);
int _dequeue(QUEUE *q);
int _emptyq(QUEUE *q);
void waitsem(semaforo *s);
void signalsem(semaforo *s);
void initsem(semaforo *s, int val);
