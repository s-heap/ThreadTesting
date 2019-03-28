# ThreadTesting
A simple program used to test threading in C. The program creates two threads tasked with dequeueing integers from a queue using a Mutex lock to ensure safe data manipulation. After a few seconds waiting, 10 integers are inserted into the queue for dequeueing. The threads activity is then printed as the integers are dequeue. Created Sept-Oct 2018
## Installation
Making use of Cs pthreads this program can be compiled using the command:
> gcc -o queueThreading queueThreading.c -lpthread
And run using the command:
> ./queueThreading
