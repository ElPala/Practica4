B=bin
L=lib
I=include
S=sources
T=todo

all: $B/exampleRR $B/exampleRR2 $B/exampleRetro $B/exampleFCFS $L/schedulerRR.o $L/schedulerRR2.o $L/schedulerRetro.o $L/schedulerFCFS.o $L/libthreads.o

$B/exampleRR: $S/example.c $L/libthreads.o $L/schedulerRR.o
	gcc -o $B/exampleRR $S/example.c $L/libthreads.o $L/schedulerRR.o -I$I

$B/exampleRR2: $S/example.c $L/libthreads.o $L/schedulerRR2.o
	gcc -o $B/exampleRR2 $S/example.c $L/libthreads.o $L/schedulerRR2.o -I$I
	
$B/exampleRetro: $S/example.c $L/libthreads.o $L/schedulerRetro.o
	gcc -o $B/exampleRetro $S/example.c $L/libthreads.o $L/schedulerRetro.o -I$I

$B/exampleFCFS: $S/example.c $L/libthreads.o $L/schedulerFCFS.o
	gcc -o $B/exampleFCFS $S/example.c $L/libthreads.o $L/schedulerFCFS.o -I$I
	
$L/schedulerRR.o: $T/schedulerRR.c $I/libthreads.h $I/commondata.h
	gcc -o $L/schedulerRR.o -c $T/schedulerRR.c -I$I

$L/schedulerRR2.o: $T/schedulerRR2.c $I/libthreads.h $I/commondata.h
	gcc -o $L/schedulerRR2.o -c $T/schedulerRR2.c -I$I
	
$L/schedulerRetro.o: $T/schedulerRetro.c $I/libthreads.h $I/commondata.h
	gcc -o $L/schedulerRetro.o -c $T/schedulerRetro.c -I$I

$L/schedulerFCFS.o: $T/schedulerFCFS.c $I/libthreads.h $I/commondata.h
	gcc -o $L/schedulerFCFS.o -c $T/schedulerFCFS.c -I$I

$L/libthreads.o: $S/libthreads.c $I/libthreads.h $I/commondata.h
	gcc -o $L/libthreads.o -c $S/libthreads.c -I$I

clean:
	rm $B/exampleRR $B/exampleRR2 $B/exampleFCFS $B/exampleRetro
	rm $L/*.o
	rm $B/results.txt