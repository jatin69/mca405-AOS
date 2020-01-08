#include<fcntl.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h> 

int global;

void main() {
	int local;	
	local = 1 ;
	if ( vfork() == 0 ){
		/* child */	
		global = 2;	/* write parent data space */
		local = 3;	/* write parent stack */
		_exit(0);
	}
	printf("global %d local %d\n", global, local);
}