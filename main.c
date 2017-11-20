#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define READ 0 
#define WRITE 1

void parent_stuff(int in[], int out[], int oldnum){
	
	close(out[READ]);
	close(in[WRITE]);

	write(out[WRITE], &oldnum , sizeof(oldnum) );
	close(out[WRITE]);

	printf("[parent] sending: %d\n", oldnum);
	

	int newnum; 

	read(in[READ], &newnum, sizeof(newnum));
	close(in[READ]);

	printf("[parent] recieved: %d\n", newnum);

}

void child_stuff(int in[], int out[]){
		
	close(in[WRITE]);
	close(out[READ]);
	
	int num; 
	read(in[READ], &num, sizeof(num));
	close(in[READ]);

	printf("[child] doing maths on: %d\n", num);
	
	num *= num;
	write(out[WRITE], &num, sizeof(num));
	close(out[WRITE]);
}

int main(){

	int to_child[2];
	int to_parent[2];

	pipe(to_child);
	pipe(to_parent);

	int f = fork();

	if (f == -1){
		perror("Child could not be forked");
		exit(1);
	}

	if (!f){//parent
		parent_stuff(to_parent, to_child, 15);
	} 
	else{//child
		child_stuff(to_child, to_parent);
	}

	return 0;
}
