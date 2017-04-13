/* Prjoect2_CS473.cpp 
Computer Science 473 - Spring 2017
Michael Fink - mjf5614

Extension Granted by Dr. Sustersic, see documentation attached to the submission

*/

#include "stdafx.h"
#include "thread_runner.h"



#include <iostream>
#include <stdio.h>
#include <mutex>
#include <thread>
#include <functional>

using namespace std;

//Each of these runs the threads in thread runner, I made the function call for deadlock to go last because if it were to go first it would crash the program, and this way 
// you can view all functions at once in the same command window

//also for some reason the Visual Studios IDE i am useing will  say that thread_runner.obj is not present when running without debugging
// but clicking the button 1-2 more times fixes the problem for some reason and then it works.
int main() {

	//Mutual-Exclusion:
	if (run('m')) return 0;

	//Hold and Wait:
	if (run('h')) return 0;
	
	//No preemption:
	if (run('p')) return 0;

	//Circular Wait:
	if (run('c')) return 0;

	//Part 1-Deadlocking program
	if (run('d')) return 0;
	int repeat = 1;
	
	while (repeat)
	{
		char c = 'x';
		printf("Enter key to run program:\n\tDeadlock- d\n\tPrevent Mutual Exclusion- m\n\tRemove Hold and Wait- h\n\tWithout Premption- p\n\tWithout Circular Wait- c\n\tQuit-z");
		cin >> c;
		repeat = run(c);
		
	} 

	return 0;
}




/*
run() argument key
Deadlock- 'd'
Mutual Exclusion- 'm'
Hold and Wait- 'h'
No Premption- 'p'
Circular Wait- 'c'
*/



