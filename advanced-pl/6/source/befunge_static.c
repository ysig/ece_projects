//some of the old ludwig van
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

#define NEXT_ONE_C next(1,false);
#define NEXT_ONE next(1,true);
#define NEXT_TWO next(2,true);
//#define NEXT_INSTRUCTION goto *(void *)(label_tab[string_toggle?((inst=='"')?TOG_STR:CHARACTER):((inst>='0'&&inst<='9')?DIGIT:get_opcode(inst))])
#define NEXT_INSTRUCTION goto *(void *)(label_tab[(inst>='0'&&inst<='9')?DIGIT:get_opcode(inst)])
#define NEXT_STRING_BYTE goto *(void *)(label_tab[(inst=='"')?TOG_STR_OFF:CHARACTER])


#define STACK_MAX 100
#define GRID_MAX_ROWS 80
#define GRID_MAX_COLS 50

#define BEF_DEBUG 0
#define READ_DEBUG 0

char grid[GRID_MAX_ROWS][GRID_MAX_COLS]={{' '}};
int n,m;
//n grammes
//m steiles
int cnt;
int fc='\0';
bool nonce_op = false;
//enum opcodes space_opcode = (enum opcodes)NEXT;
char inst;
int rows,cols;
enum directions pc_direction;
bool string_toggle;
enum opcodes oc;

enum directions{
	RIGHT=0,
	LEFT,
	UP,
	DOWN
};

enum opcodes{
	NONCE=0,
	NEXT,
	PC_RIGHT,
	PC_LEFT,
	PC_TOP,
	PC_DOWN,
	PC_RAND,
	ADD,
	SUB,
	MUL,
	DIV,
	MOD,
	NOT,
	GRE,
	IF_HOR,
	IF_VER,
	DUP,
	SWP,
	POP,
	OUT_INT,
	OUT_CHAR,
	SKIP_NEXT,
	GET,
	PUT,
	IN_INT,
	IN_CHAR,	
	TOG_STR_ON,
	END,
	DIGIT,
	CHARACTER,
	TOG_STR_OFF
};

//enum opcode space_opcode = NEXT;
enum directions pc_direction;

/* works because opcode size < '0' */

enum opcodes get_opcode(char c){
	switch(c){
		case '\0':
		case ' ':
			return NEXT;
			break;
		case '>':
			return PC_RIGHT;
			break;
		case '<':
			return PC_LEFT;
			break;
		case '^':
			return PC_TOP;
			break;
		case 'v':
			return PC_DOWN;
			break;
		case '?':
			return PC_RAND;
			break;
		case '+':
			return ADD;
			break;
		case '-':
			return SUB;
			break;
		case '*':
			return MUL;
			break;
		case '/':
			return DIV;
			break;
		case '%':
			return MOD;
			break;
		case '!':
			return NOT;
			break;
		case '`':
			return GRE;
			break;
		case '_':
			return IF_HOR;
			break;
		case '|':
			return IF_VER;
			break;
		case ':':
			return DUP;
			break;
		case '\\':
			return SWP;
			break;
		case '$':
			return POP;
			break;
		case '.':
			return OUT_INT;
			break;
		case ',':
			return OUT_CHAR;
			break;
		case '#':
			return SKIP_NEXT;
			break;
		case 'g':
			return GET;
			break;
		case 'p':
			return PUT;
			break;
		case '&':
			return IN_INT;
			break;
		case '~':
			return IN_CHAR;
			break;
		case '@':
			return END;
			break;
		case '"':
			return TOG_STR_ON;
			break;
		default:
			if((c>= '0' && c<='9')){
				return c;
			}
			else{
#if BEF_DEBUG
				fprintf(stderr,"Warning: Nonce character \'%c\' with ascii %d\nRows = %d, cols = %d\n",c,(int)c,rows,cols);
#endif
				nonce_op = true;
				return NONCE;
			}
	}
}
//TO DO WRITE TRANSLATE OPCODE (can use also a table) (because this time the matching is not sparse)
char opcode_dictionary[]={(char)(255),' ','>','<','^','v','?','+','-','*','/','%','!','`','-','|',':','\\','$','.',',','#','g','p','&','~','@','"'};

void next(int i,bool f){
	switch(pc_direction){
		case UP:
			rows -= i;
			rows %= n;
			rows = rows < 0 ? rows + n : rows;
			break;
		case DOWN:
			rows += i;
			rows %= n;
			break;
		case LEFT:
			cols -= i;
			cols %= m;
			break;
		case RIGHT:
			cols += i;
			cols %= m;
			cols = cols < 0 ? cols + m : cols;
			break;
	}
	inst = grid[rows][cols];
#if BEF_DEBUG
	fprintf(stdout,"Next inst is %c\n",inst);
#endif
	if(f){oc = get_opcode(inst);}
}

/* Stack functions */


struct stack {
    int     *data;
    int     size;
	int stack_size;
};
typedef struct stack Stack;


void Stack_Init(Stack *S)
{
	S->data = (int *)malloc(STACK_MAX*sizeof(int));
    S->size = 0;
	S->stack_size = STACK_MAX;
}

int Stack_Top(Stack *S)
{
    if (S->size == 0) {
        fprintf(stderr, "Warning: stack empty\n");
        return 0;
    } 

    return S->data[S->size-1];
}

void Stack_Push(Stack *S, int d)
{
    if (S->size < STACK_MAX){
        S->data[S->size] = d;
		S->size++;
		//fprintf(stdout,"Pushing on Stack\nValue=%d\nCurrent stack size = %d\n",d,S->size);
	}
    else{
		S->stack_size += STACK_MAX;
     	S->data = realloc(S->data,S->stack_size);
		Stack_Push(S,d);
	}
}

int Stack_Pop(Stack *S)
{
	int res;
	//fprintf(stdout,"Popping Stack\nS->size=%d\n",S->size);
    if (S->size == 0){
        res = 0;
	}
    else{
		res = S->data[--S->size];
	}
	return res;
}

/* End of stack functions */
void parse(){

static void *label_tab[] = {
		&&nonce_label,
		&&next_label,
		&&pc_right_label,
		&&pc_left_label,
		&&pc_top_label,
		&&pc_down_label,
		&&pc_rand_label,
		&&add_label,
		&&sub_label,
		&&mul_label,
		&&div_label,
		&&mod_label,
		&&not_label,
		&&greater_label,
		&&hor_if_label,
		&&ver_if_label,
		&&dup_label,
		&&swap_label,
		&&pop_label,
		&&out_int_label,
		&&out_char_label,
		&&skip_next_label,
		&&get_label,
		&&put_label,
		&&input_int_label,
		&&input_char_label,
		&&toggle_string_label_on,
		&&end_label,
		&&dig_label,
		&&character_label,
		&&toggle_string_label_off
};

char c1;
int v1,v2;
Stack *s = (Stack *) malloc(sizeof(Stack));;
cols=0;
rows=0;
string_toggle = false;
pc_direction = RIGHT;
inst = grid[rows][cols];
Stack_Init(s);
while(true){
	if(string_toggle){
		if(inst='\"'){
			toggle_string_label_off:
			//printf("String Label off\n");
			string_toggle = !string_toggle;
			NEXT_ONE;
			NEXT_INSTRUCTION;
		}
		else{
		character_label:
			//printf("Pushing inst = '%c'\n",inst);
			Stack_Push(s,(int) inst);
			NEXT_ONE_C;
			NEXT_STRING_BYTE;
		}
	}		
	else{
	//fprintf(stdout,"ELSe:\n");
	oc = get_opcode(inst);
	switch(oc){
		case NONCE:
		nonce_label:
			fprintf(stderr,"Error: uknown instruction\nExiting..\n");
			//fprintf(stderr,"Warning: uknown instruction\nJumping to next..\n");
		exit_label: exit(1);
			goto next_label;
			break;
		case NEXT:
		next_label:
			NEXT_ONE;
			NEXT_INSTRUCTION;
			break;
		case PC_RIGHT:
		pc_right_label:
			//printf("PC right\n");
			pc_direction=RIGHT;
			NEXT_ONE;
			NEXT_INSTRUCTION;
			break;
		case PC_LEFT:
		pc_left_label:
			pc_direction=LEFT;
			NEXT_ONE;
			NEXT_INSTRUCTION;
			break;
		case PC_TOP:
		pc_top_label:
			pc_direction=UP;
			NEXT_ONE;
			NEXT_INSTRUCTION;
			break;
		case PC_DOWN:
		pc_down_label:
			pc_direction=DOWN;
			NEXT_ONE;
			NEXT_INSTRUCTION;
			break;
		case PC_RAND:
		pc_rand_label:
			pc_direction=(enum directions) rand()%4;
			NEXT_ONE;
			NEXT_INSTRUCTION;
			break;
		case ADD:
		add_label:
			v2 = Stack_Pop(s);
			v1 = Stack_Pop(s);
			Stack_Push(s,v1+v2);
			NEXT_ONE;
			NEXT_INSTRUCTION;
			break;
		case SUB:
		sub_label:
			v2 = Stack_Pop(s);
			v1 = Stack_Pop(s);
			Stack_Push(s,v1-v2);
			NEXT_ONE;
			NEXT_INSTRUCTION;
			break;
		case MUL:
		mul_label:
			v2 = Stack_Pop(s);
			v1 = Stack_Pop(s);
			Stack_Push(s,v1*v2);
			NEXT_ONE;
			NEXT_INSTRUCTION;
			break;
		case DIV:
		div_label:
			v2 = Stack_Pop(s);
			v1 = Stack_Pop(s);
			if(v2==0){
				fprintf(stderr,"Error:\nDividing with zero!\n\nExiting..\n");
				exit(1);
			}
			Stack_Push(s,v1/v2);
			NEXT_ONE;
			NEXT_INSTRUCTION;
			break;
		case MOD:
		mod_label:
			v2 = Stack_Pop(s);
			v1 = Stack_Pop(s);
			if(v2==0){
				fprintf(stderr,"Error:\nModulo with zero!\n\nExiting..\n");
				exit(1);
			}
			Stack_Push(s,v1%v2);
			NEXT_ONE;
			NEXT_INSTRUCTION;
			break;
		case NOT:
		not_label:
			v1 = Stack_Pop(s);
			Stack_Push(s,(int)((bool)!v1));
			NEXT_ONE;
			NEXT_INSTRUCTION;
			break;
		case GRE:
		greater_label:
			v2 = Stack_Pop(s);
			v1 = Stack_Pop(s);
			Stack_Push(s,(int)((bool)(v1>v2)));
			NEXT_ONE;
			NEXT_INSTRUCTION;
			break;
		case IF_HOR:
		hor_if_label:
			v1 = Stack_Pop(s);
			if(v1){
				pc_direction=LEFT;
			}else{
				pc_direction=RIGHT;
			}
			NEXT_ONE;
			NEXT_INSTRUCTION;
			break;
		case IF_VER:
		ver_if_label:
			v1 = Stack_Pop(s);
			if(v1){
				pc_direction=UP;
			}else{
				pc_direction=DOWN;
			}
			NEXT_ONE;
			NEXT_INSTRUCTION;
			break;
		case DUP:
		dup_label:
			v2 = Stack_Pop(s);
			//fprintf(stdout,"\nPushing duplicate v2=%c\n",v2);
			Stack_Push(s,v2);
			Stack_Push(s,v2);
			NEXT_ONE;
			NEXT_INSTRUCTION;
			break;
		case SWP:
		swap_label:
			v2 = Stack_Pop(s);
			v1 = Stack_Pop(s);
			Stack_Push(s,v2);
			Stack_Push(s,v1);
			NEXT_ONE;
			NEXT_INSTRUCTION;
			break;
		case POP:
		pop_label:
			//i prefer BOP
			if(s->size==0){
				Stack_Push(s,0);
			}
			else{
				Stack_Pop(s);
			}
			NEXT_ONE;
			NEXT_INSTRUCTION;
			break;
		case OUT_INT:
		out_int_label:
			//fprintf(stdout,"out_int_label\n");
			if(s->size==0){
				Stack_Push(s,0);
				v1=0;
			}
			else{
				v1=Stack_Pop(s);
			}
			fprintf(stdout,"%d ",v1);
			NEXT_ONE;
			NEXT_INSTRUCTION;
			break;
		case OUT_CHAR:
		out_char_label:
			//fprintf(stdout,"out_char_label\n");
			if(s->size==0){
				Stack_Push(s,0);
				v1=0;
			}
			else{
				v1=Stack_Pop(s);
			}
			fprintf(stdout,"%c",v1);
			NEXT_ONE;
			NEXT_INSTRUCTION;
			break;
		case SKIP_NEXT:
		skip_next_label:
			NEXT_TWO;
			NEXT_INSTRUCTION;
			break;
		case GET:
		get_label:
			v2 = Stack_Pop(s);
			v1 = Stack_Pop(s);
			if(v2>GRID_MAX_ROWS||v1>GRID_MAX_COLS){
				fprintf(stderr,"Warning: Get instruction from outside of the grid\nInserting \"space\" character result\n");
				c1 = ' ';
			}else{
				c1 = grid[v2][v1];
			}
			Stack_Push(s,(int)c1);
			NEXT_ONE;
			NEXT_INSTRUCTION;
			break;
		case PUT:
		put_label:
			v2 = Stack_Pop(s);
			v1 = Stack_Pop(s);
			if(v2>GRID_MAX_ROWS||v1>GRID_MAX_COLS){
				fprintf(stderr,"Error: Put instruction to the outside of the grid\n");
				goto exit_label;
			}
			n = MAX(v2+1,n);
			m = MAX(v1+1,m);
			grid[v2][v1] = (char)Stack_Pop(s);
			NEXT_ONE;
			NEXT_INSTRUCTION;
			break;
		case IN_INT:
		input_int_label:
			scanf("%d",&v1);
			Stack_Push(s,v1);
			NEXT_ONE;
			NEXT_INSTRUCTION;
			break;
		case IN_CHAR:
		input_char_label:
			scanf("%c",&c1);
			Stack_Push(s,(int) c1);
			NEXT_ONE;
			NEXT_INSTRUCTION;
			break;
		case TOG_STR_ON:
		toggle_string_label_on:
			//printf("String Label on\n");
			string_toggle = !string_toggle;
			NEXT_ONE_C;
			NEXT_STRING_BYTE;
			break;
		case END:
		end_label:
			return;
			break;
		default:
			if(oc>='0'&&oc<='9'){
			dig_label:
				//printf("Digit %c\n",oc);
				Stack_Push(s,(int)(oc-'0'));
				NEXT_ONE;
				NEXT_INSTRUCTION;
			}
			else{
				fprintf(stderr,"Warning: Unhandled opcode for %c\nMoving to next..",inst);
				NEXT_ONE;
				NEXT_INSTRUCTION;
			}
			break;
	}
	}
}
}


int max(int a, int b,bool *f){
	if(f==NULL)
		return MAX(a,b);
	else
		if(*f){
			*f = false;
			return b;
		}else
			return MAX(a,b);
}

bool read_input(FILE *f){
	char ch = getc(f);
    int count = 0, i=0, j=0;
	bool firstLine = true;
	n=1;
	m=0;
	while (ch != EOF) {
		if(j>GRID_MAX_COLS||i>85){
			return false;
		}
		if(ch == '\n'){
			m = max(m,count,&firstLine);
			count = 0;
			i++;
			j=0;
			n++;
		}
		else{
			grid[i][j]=ch;
			j++;
			count++;
		}
        ch = getc(f);
#if READ_DEBUG
		fprintf(stdout,"Readed character %c with ascii %d, on line %d,%d\n",ch,(int)ch,i,j);
#endif
    }
	grid[i][j]=ch;
	if(ch == EOF && count!=0){
		m = max(m,count,&firstLine);
	}
	else{
		n--;
	}
	m++;
	return true;
}


void printHelp(){
	printf("Help !!\n");
}

void printGrid(bool full){
	int i,j,np,mp;
	if(full){
		np=GRID_MAX_ROWS;
		mp=GRID_MAX_COLS;
	}else{
		np=n;
		mp=m;	
	}
	if(grid == NULL){
		fprintf(stderr,"Nothing to print\n");
	}
	else{
		printf("Printing Grid ... \n\n");
		for(i=0; i<np; i++){
			for(j=0; j<mp; j++){
				printf("%c",grid[i][j]);
			}
			printf("\n");
		}
	}
}

int main(int argc, char *argv[]){
	FILE *f;
	memset(grid, ' ', GRID_MAX_ROWS*GRID_MAX_COLS*sizeof grid[0][0]);
	if(argc==2){
#if BEF_DEBUG
		fprintf(stdout,"Opening File %s\n",argv[1]);
#endif
		if(f = fopen(argv[1],"r+")){
			if(read_input(f)){
				fclose(f);
#if BEF_DEBUG
				printGrid(false);
				fprintf(stdout,"\nInit parsing..\n");
				fprintf(stdout,"n=%d,m=%d\n",n,m);
#endif
				parse();
			}else{
				fclose(f);
				fprintf(stderr,"Wrong Input: Input extends grid size\n");
				exit(1);
			}
		}
		else{
			fprintf(stderr,"Input file does not exist!\n");
			exit(1);
		}
	}
	else{
		printHelp();	
	}
	return 0;
}
