# Thread-safe-list
This project has two parts: 

## Part 1 - Barrier:
An N-process barrier is a synchronization mechanism that allows N threads, where N is a fixed number, to wait until all of them have reached a certain point. Once all threads have reached this certain point (the barrier), they may all continue.

## Part 2 - Thread safe sorted linked list:
In this part you will implement multi-threaded sorted linked list (aka thread safe sorted linked list). Each node of the list has a unique field data which can be considered as the node key, (a situation when two nodes have same data value is not allowed) that is used to keep the list sorted in an increasing order.
In order to keep high concurrency, you will use fine-grained locking. Multiple operations should be able to run simultaneously but are expected to run normally and do not disturb each other. You must use hand over hand locking to achieve that goal.
What is hand over hand lockig?
The insertion and removal operations may be called concurrently. Operations involving different elements should not wait for each other unless this is strictly necessary.

## [Full instructions and examples](Instructions.pdf) (English)

