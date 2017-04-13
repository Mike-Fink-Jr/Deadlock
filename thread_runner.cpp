

#include "thread_runner.h"
#include "stdafx.h"


#include <iostream>
#include <stdio.h>
#include <mutex>
#include <thread>
#include <functional>
#include <stdio.h>


using namespace std;
/*
	run() argument key:
						Deadlock- 'd'
				Mutual Exclusion- 'm'
				   Hold and Wait- 'h'
					No Premption- 'p'
				   Circular Wait- 'c'
*/

//function pointers
void(*thread_fn1)(void);
void(*thread_fn2)(void);
void(*thread_fn3)(void);

mutex m1;
mutex m2;
mutex m3; 





/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
Part 1 - Create a multithreaded program that deadlocks.

Note:
since the deadlock will crash the program, this is ran last in the program so I can demonstrate all of the parts in Part 2 in one fluid sequence
I also originally had these in multiple files, but I can never get header files just right so I gave up after 2 mins of figeting with it



In order for deadlock to happen 4 conditions must be in effect at the same time
1) Mutual exclusion: only one process at a time can use a resource

2) Hold and wait: a process holding at least one resource is waiting to acquire additional resources held by other processes

3) No preemption: a resource can be released only voluntarily by the process holding it, after that process has completed its task

4) Circular wait: there exists a set {P0, P1, …, Pn} of waiting processes such that P0 is waiting for a resource that is held by P1, 
   P1 is waiting for a resource that is held by P2, …, Pn–1 is waiting for a resource that is held by Pn, and Pn is waiting for a 
   resource that is held by P0.

   -
*/
void dl1() {
	m1.lock();
	for (int i = 10; i >-1; i--)	
		cout << "\nt1-" << i;
	m2.lock();
	m1.unlock();
	m2.unlock();
}

void dl2() {
	m2.lock();
	for (int i = 10; i >-1; i--)	
		cout << "\nt2-" << i;
	m3.lock();
	m2.unlock();
	m3.unlock();
}

void dl3() {
	m3.lock();
	for (int i = 10; i >-1; i--)	
		cout << "\nt3-" << i;
	m1.lock();
	m3.unlock();
	m1.unlock();
}







/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*1) Mutual exclusion : only one process at a time can use a resource.
						not required for sharable resources like read-only files.
						must hold for non-sharable resources.

		the difference between this code and the deadlock code is that: 
		for each of me1,me2,me3 the program locks a mutex and then unlocks at the end of the execution, 
		ensureing that only one thread is useing the mutex at one time and that it will let go of it once it is done


*/
void me1() {
	m1.lock();
	for (int i = 10; i >-1; i--)	
		cout << "\nt1-" << i;
	m1.unlock();
}

void me2() {
	m2.lock();
	for (int i = 10; i >-1; i--)	
		cout << "\nt2-" << i;
	m2.unlock();
}

void me3() {
	m3.lock();
	for (int i = 10; i >-1; i--)	
		cout << "\nt3-" << i;
	m3.unlock();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
Circular Wait -impose a total ordering of all resource types, and require that each process requests resources in an increasing order of enumeration

	the difference between this code and the deadlock code is that:
	The threads in this system form a circular list or chain where each process in the list is waiting for a resource held by the next process in the list.
	this circular line will ensure that as soon as one thread unlocks its mutex, the next thread in line for that mutex will get it and use it and that there 
	will always be a cirulation within the threads.


*/

void cw1() {
	m1.lock();
	m2.lock();
	for (int i = 10; i >-1; i--)	
		cout << "\nt1-" << i;
	m2.unlock();
	m1.unlock();
}

void cw2() {
	m2.lock();
	m3.lock();
	for (int i = 10; i >-1; i--)	
		cout << "\nt2-" << i;
	m3.unlock();
	m2.unlock();
}

void cw3() {
	m3.lock();
	m1.lock();
	for (int i = 10; i >-1; i--)	
		cout << "\nt3-" << i;
	m1.unlock();
	m3.unlock();

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
Hold and Wait -
		must guarantee that whenever a process requests a resource, it does not hold any other resources  
			 1) Require process to request and be allocated all its resources 
			    before it begins execution, or allow process to request resources only when the process has none
				allocated to it.

			2)  Low resource utilization; starvation possible



			the difference between this code and the deadlock code is that: 
			in hw1 there is a pre execution checker that will keep trying to use its corresponding resource
			this check keeps each thread from grabbing onto a resource that other threads need to execute and holding it for ever 
			and lets each thread play nice with each other and wait their turn before grabbing their first resource
			


*/


void hw1() {

	bool pre_execution= true;

	while (pre_execution) 
	{
		if (m1.try_lock()) 
		{
			for (int i = 10; i >-1; i--)	
				cout << "\nt1-" << i;
			m1.unlock();
			pre_execution= false;
		}
	}


}

void hw2() {
	m2.lock();
	for (int i = 10; i >-1; i--)
		cout << "\nt2-" << i;
	m3.lock();
	m2.unlock();
	m3.unlock();
}

void hw3() {
	m3.lock();
	for (int i = 10; i >-1; i--)
		cout << "\nt3-" << i;
	m1.lock();
	m3.unlock();
	m1.unlock();
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
No Premption	- If a process that is holding some resources requests another resource that cannot be
				  immediately allocated to it, then all resources currently being held are released
				
				- Preempted resources are added to the list of resources for which the process is waiting
				
				- Process will be restarted only when it can regain its old resources, as well as the new ones that it is requesting


		Similar to hold and wait this code checks if it can access the resource it needs, but if it cant then 
		it unlocks the item it is holding, no premption and hold and wait are very similar and are esentially 
		two different sides of the same coin as they want to make sure that if a process can't use a resource 
		then it wants to let others use it if they need to 
		
*/

void np1()
{
	bool locked = true;
	while (locked)
	{
		m1.lock();
		if (!m2.try_lock()) // cant access m2
			m1.unlock(); // shares m1
		else
		{
			locked = false;
			for (int i = 10; i >-1; i--)	cout << "\nt1-" << i;
			m2.unlock();
			m1.unlock();
		}
	}
}

void np2()
{
	bool locked = true;
	while (locked)
	{
		m2.lock();
		if (!m3.try_lock()) 
			m2.unlock();
		else
		{
			locked = false;
			for (int i = 10; i >-1; i--)	cout << "\nt2-" << i;
			m3.unlock();
			m2.unlock();
		}
	}
}
void np3() 
{
	bool locked = true;
	while (locked)
	{
		m3.lock();
		if (!m1.try_lock()) 
			m3.unlock();
		else
		{
			locked = false;
			for (int i = 10; i >-1; i--)	cout << "\nt3-" << i;
			m1.unlock();
			m3.unlock();
		}
	}




}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////








void execute()
{

	char c = 'x';
	thread thread1(thread_fn1);
	thread thread2(thread_fn2);
	thread thread3(thread_fn3);


	thread1.join();
	thread2.join();
	thread3.join();

	cout << "\n\n";
	cout << "Threads successfully ran.";

	
	do { cout << "press 'c' to continue\n"; cin >> c; } while (c != 'c');
}


int run(char key)
{
	

	char c = 'x';

	switch (key)
	{
	case 'd':// deadlock


		do { cout << "Enter 'r' to run a multi-threaded program with deadlock \n"; cin >> c; } while (c != 'r');
		cout << "Deadlock Execution \n";
		thread_fn1 = &dl1;
		thread_fn2 = &dl2;
		thread_fn3 = &dl3;
		cout << "\n\n\n";
		break; //end deadlock

	case 'm': //mutual exclusion


		do { cout << "Enter 'r' to run a multi-threaded program without Mutual Exlusion  \n"; cin >> c; } while (c != 'r');
		cout << "Without Mutual Exclusion Execution \n";
		thread_fn1 = &me1;
		thread_fn2 = &me2;
		thread_fn3 = &me3;
		cout << "\n\n\n";
		break; //end mutual exclusion

	case 'h': //hold and wait
		cout << "\n\n\n";

		do { cout << "Enter 'r' to run execute a multi-threaded program without Holding and Waiting  \n"; cin >> c; } while (c != 'r');
		cout << "Without Hold and Wait Execution \n";
		thread_fn1 = &hw1;
		thread_fn2 = &hw2;
		thread_fn3 = &hw3;
		cout << "\n\n\n";
		break;//end hold and wait

	case 'c':  //circular wait
		cout << "\n\n\n";

		do { cout << "Enter 'r' to run execute a multi-threaded program without Circular Waiting  \n"; cin >> c; } while (c != 'r');
		cout << "Without Circular Waiting Execution \n";
		thread_fn1 = &cw1;
		thread_fn2 = &cw2;
		thread_fn3 = &cw3;
		cout << "\n\n\n";
		break; //end circular wait

	case 'p': //no premption
		cout << "\n\n\n";

		do { cout << "Enter 'r' to run execute a multi-threaded program without Premption  \n"; cin >> c; } while (c != 'r');
		cout << "Without Premption Execution \n";
		thread_fn1 = &np1;
		thread_fn2 = &np2;
		thread_fn3 = &np3;
		cout << "\n\n\n";
		break; //end no premption

	case 'z': return 1;

	default:
		cout << "Please enter a valid key";
		cin >> c;
		return run(c);

	}//endswitch

	execute();
	return 0;
}
