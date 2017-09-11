#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(){
int fp;
char A[20];
fp = open("/dev/lunix0-temp",O_RDONLY);
if(fp == -1){
        fprintf(stderr,"Error reading from file\n");
        exit(1);
}
int a;
a=read(fp,A,2);
printf("READ 1: I readed %d bytes which are %s\n",a,A);
sleep(2);
a=read(fp,A,6);
printf("READ 2: I readed %d bytes which are %s\n",a,A);
sleep(2);
a=read(fp,A,8);
printf("READ 2: I readed %d bytes which are %s\n",a,A);
sleep(2);
printf("Goodbye Princess\n");
sleep(2);

return 0;
}


