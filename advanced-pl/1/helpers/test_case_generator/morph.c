#include <stdio.h>
#include <stdlib.h>


// set initial node as 1 (from zero)
int main(){
int N,P,x,y,i;
FILE *fp;
fp = fopen("morph.txt","w");
scanf("%d %d\n",&N,&P);
fprintf(fp,"%d %d\n",N,P);
for(i=0; i<P; i++){
	scanf("%d %d\n",&x,&y);
	fprintf(fp,"%d %d\n",x+1,y+1);
}
fclose(fp);
}
