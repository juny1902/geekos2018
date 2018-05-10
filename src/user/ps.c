/*
 * Copyright (c) 2001,2003,2004 David H. Hovemeyer <daveho@cs.umd.edu>
 * Copyright (c) 2003,2013,2014 Jeffrey K. Hollingsworth <hollings@cs.umd.edu>
 *
 * All rights reserved.
 *
 * This code may not be resdistributed without the permission of the copyright holders.
 * Any student solutions using any of this code base constitute derviced work and may
 * not be redistributed in any form.  This includes (but is not limited to) posting on
 * public forums or web sites, providing copies to (past, present, or future) students
 * enrolled in similar operating systems courses the University of Maryland's CMSC412 course.
 */

#include <conio.h>
#include <process.h>
struct Process_Info ptable[50];
int main(int argc __attribute__ ((unused)), char **argv
         __attribute__ ((unused))) {
	
	char stat, core, affi;
	int i = 0;
	PS(ptable,50);
	Print("PID PPID PRIO STAT AFF TIME COMMAND\n");
	
	for(i=0;i<50;i++)
	{
	// If there's no process in entry
		if(ptable[i].pid == 0) return 0;

		// Setting status code
		if(ptable[i].status == STATUS_ZOMBIE){
			stat = 'Z';
		}
		else if(ptable[i].status == STATUS_RUNNABLE){
			stat = 'R';
		}
		else if(ptable[i].status == STATUS_BLOCKED){
			stat = 'B';
		}else{
			stat = 'U'; // Unknown State
		}

		// Setting current core
		if(stat == 'R'){
			core = ptable[i].currCore + '0';
		}else{
			core = ' ';
		}

		// Setting Affinity
		if(ptable[i].affinity == -1)
		{
			affi = 'A';
		}else
		{
			affi = ptable[i].affinity + '0';
		}

		// Printing
		Print("%3d %4d %4d %2c%2c %3c %4d %s\n",
				ptable[i].pid, ptable[i].parent_pid,
				ptable[i].priority,
				core,stat,affi,
				ptable[i].totalTime, ptable[i].name);
	}
	return 0;
}

