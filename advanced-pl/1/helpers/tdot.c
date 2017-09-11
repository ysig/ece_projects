#include <stdio.h>
#include <stdlib.h>

int main(){
char command[256];
int i,N,K,x,y;
scanf("%d %d\n",&N,&K);
system("rm -f in.ei");
system("echo \"\"  in.ei");
system("echo \"graph graphname{\" >> in.ei");
for(i=1; i<=K; i++){
	scanf("%d %d\n",&x,&y);
	sprintf(command,"echo \"%d -- %d\" >> in.ei",x,y);
	system(command);
}
system("echo \"}\" >> in.ei");
system("dot -Tpng in.ei > output.png");
//system("rm in.dot");
return 0;
}
