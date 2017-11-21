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

int main(int argc, char ** argv){
	
	int number = 15;	

	if (argc == 2){
		number = atoi(argv[1]); //convert string to int
	}


	int to_child[2];
	int to_parent[2];

	if( pipe(to_child) == -1){
		perror("Pipe to child could not be created");
		exit(1);
	}
	if ( pipe(to_parent) == -1){
		perror("Pipe to parent could not be created");
		exit(2);
	}

	int f = fork();

	if (f == -1){
		perror("Child could not be forked");
		exit(3);
	}

	if (!f){//parent
		parent_stuff(to_parent, to_child, number);
	} 
	else{//child
		child_stuff(to_child, to_parent);
	}

	return 0;
}
