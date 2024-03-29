#include <scheduler.h>
#include <stdio.h>

extern THANDLER threads[MAXTHREAD];
extern int currthread;
extern int blockevent;
extern int unblockevent;

//QUEUE retro[1000];
//int prioridad;
//int priority[4];
QUEUE priorities[5];
int prior=0;
int i = 0;
int proc = 1;

QUEUE ready;
QUEUE waitinginevent[MAXTHREAD];

void scheduler(int arguments)
{
	int old,next;
	int changethread=0;
	int waitingthread=0;

	int event=arguments & 0xFF00;
	int callingthread=arguments & 0xFF;

	if(event==NEWTHREAD)
	{
		// Un nuevo hilo va a la cola de listos
		proc++;
		threads[callingthread].status=READY;
		_enqueue(&priorities[0],callingthread);
		//_enqueue(&retro[prioridad],callingthread);
		//prioridad++;
	}

	if(event==BLOCKTHREAD)
	{
		threads[callingthread].status=BLOCKED;
		_enqueue(&waitinginevent[blockevent],callingthread);

		changethread=1;
	}

	if(event==TIMER)
	{

		threads[callingthread].status=READY;
		if(proc == 1|| prior == 4) {

			_enqueue(&priorities[prior],callingthread);

		}
		else{
			_enqueue(&priorities[prior+1],callingthread);
		}
		changethread=1;

	}

	if(event==ENDTHREAD)
	{
		threads[callingthread].status=END;
		changethread=1;
		proc--;
	}

	if(event==UNBLOCKTHREAD)
	{
			threads[callingthread].status=READY;
		if(prior == 4) {
			_enqueue(&priorities[prior],callingthread);
		}
		else{
			_enqueue(&priorities[prior+1],callingthread);
		}
	}


	if(changethread){
		old=currthread;
		for(i = 0; i< 5; i++){
			if(!_emptyq(&priorities[i])){
					next=_dequeue(&priorities[i]);
					prior = i;
					break;
			}

	}
		threads[next].status=RUNNING;
		_swapthreads(old,next);

	}

/*
	else{
		int x = 0;
		int mayorPrioridad = 1000;
		for(x = 0;x > priority.length; x++){
				if (mayorPrioridad > priority[i])
					mayorPrioridad = priority[i];
		}

		old=currthread;
		_enqueue(&ready,old);
		next=_dequeue(&ready);
		threads[next].status=RUNNING;
		_swapthreads(old,next);

	}
*/


}
