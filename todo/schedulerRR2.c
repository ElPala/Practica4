#include <scheduler.h>
#include <stdio.h>
extern THANDLER threads[MAXTHREAD];
extern int currthread;
extern int blockevent;
extern int unblockevent;

QUEUE ready;
QUEUE waitinginevent[MAXTHREAD];
int q=0;
int bool;
void scheduler(int arguments)
{
	int old,next;
	int changethread=0;
	int waitingthread=0;

	int event=arguments & 0xFF00;
	int callingthread=arguments & 0xFF;

	if(event==NEWTHREAD)
	{
		_enqueue(&ready,callingthread);
		_enqueue(&ready,callingthread);
		// Un nuevo hilo va a la cola de listos
		threads[callingthread].status=READY;
	}

	if(event==BLOCKTHREAD)
	{
		//printf("llamada:%d actual:%d BLOQUEADO\n",callingthread, currthread );
		threads[callingthread].status=BLOCKED;
		_enqueue(&waitinginevent[blockevent],callingthread);
		changethread=1;
	}

	if(event==ENDTHREAD)
	{
		threads[callingthread].status=END;
		changethread=1;
	}

	if(event==UNBLOCKTHREAD)
	{
	//printf("llamada:%d actual: %d DESBLOQUEADO\n",callingthread, currthread );
		_enqueue(&ready,callingthread);
		_enqueue(&ready,callingthread);
			threads[callingthread].status=READY;


	}

	old=currthread;
	if(_emptyq(&ready)){
		return;
	}
	next=_dequeue(&ready);


	if(changethread){
		while(old==next){
			next=_dequeue(&ready);
		}
		q=0;
		if(!_emptyq(&ready)){
			threads[next].status=RUNNING;
			_swapthreads(old,next);
		}

	}else{

		if(old==next){
			q++;
			if(q==2){
				q=0;
				_enqueue(&ready,old);
				_enqueue(&ready,old);
			}
		}else {
			q=0;
			_enqueue(&ready,old);
			_enqueue(&ready,old);
			threads[next].status=RUNNING;
			_swapthreads(old,next);

		}
	}

}
