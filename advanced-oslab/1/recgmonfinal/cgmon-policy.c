#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main() {
        FILE *fp;

        char A[1024];
        char path[1024];
        char app_name[256];
        int cpu;
        int sum=0;
        
	/* You may have to initialise sum to existing sum */
        int given_cpu_shares=2000;

	/* Creating temporary adress for storing information on a file --[unique for each pid] */        
	sprintf(path,"/etc/rex%d",(int)getpid());

        fp=fopen(path,"wr");
        while (scanf("%s\n",A) != -1) {

                sscanf(A,"policy:%[^:]:cpu:%d",app_name,&cpu);
                sum=cpu+sum;
                fprintf(fp,"%s\n",A);

		/* Cpu shares are acceptable? */
                if(sum>given_cpu_shares){

                        /* Pop error message */
                        fclose(fp);
                        printf("score:%f\n",(given_cpu_shares - sum)*1.0);

                        /* Delete file and exit */
                        remove(path);
                        return 0;
                }

        }
        fclose(fp);

        /* Cpu shares are acceptable, reopen the file from the top, readonly */
        fp=fopen(path,"r");

	/* Output score value */
        printf("score:%f\n",(given_cpu_shares - sum)*1.0);

        while (fscanf(fp,"%s\n",A) != -1) {

		sscanf(A,"policy:%[^:]:cpu:%d\n",app_name,&cpu);
	
		/* Normalisation of the cpu.shares value and output*/
		printf("set_limit:%s:cpu.shares:%d\n",app_name,(cpu*1024)/sum);

        }
        fclose(fp);

	/* Delete file and exit */
        remove(path);
        return 0;

}

