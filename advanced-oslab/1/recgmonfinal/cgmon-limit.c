#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main() {

	char monitor[256],app_name[256];
	int pid,csv;
	char A[1024];
	char command[1024];

	while (fscanf(stdin,"%s\n",A)!=-1) {

		/* Create Cgroup for some new app */
		if(sscanf(A,"create:%[^:]:cpu:%s",monitor,app_name) != 0){

			sprintf(command,"mkdir -p /sys/fs/cgroup/cpu/%s/%s",monitor,app_name);

			system(command);
			continue;

		}

		/* Remove Cgroup for some finished app */
		if(sscanf(A,"remove:%[^:]:cpu:%s",monitor,app_name) != 0){

			sprintf(command,"rmdir /sys/fs/cgroup/cpu/%s/%s",monitor,app_name);

			system(command);
			continue;

		}

		/* Add process to an existing Cgroup */		
		if(sscanf(A,"add:%[^:]:cpu:%[^:]:%d",monitor,app_name,&pid) != 0){

			sprintf(command,"echo %d >> /sys/fs/cgroup/cpu/%s/%s/tasks",pid,monitor,app_name);

			system(command);
			continue;

		}

		/* Set cpu.shares value for the Cgroup of some app */
		if(sscanf(A,"set_limit:%[^:]:cpu:%[^:]:cpu.shares:%d",monitor,app_name,&csv) != 0){

			sprintf(command,"echo %d > /sys/fs/cgroup/cpu/%s/%s/cpu.shares",csv,monitor,app_name);

			system(command);
			continue;

		}
	
	}	

	return 0;

}

