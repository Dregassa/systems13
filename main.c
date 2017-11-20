#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define READ 0 
#define WRITE 1

void parent_stuff(int fds[], int oldnum){
	
	write(fds[WRITE], &oldnum , sizeof(oldnum) );
	close(fds[WRITE]);

	printf("[parent] sending: %d\n", oldnum);
	
	int newnum; 
	read(fds[READ], &newnum, sizeof(newnum));
	close(fds[READ]);

	printf("[parent] recieved: %d\n", newnum);

}

void child_stuff(int fds[]){
	int num; 
	read(fds[READ], &num, sizeof(num));
	close(fds[READ]);

	printf("[child] doing maths on: %d\n", num);
	
	num *= num;
	write(fds[WRITE], &num, sizeof(num));
	close(fds[WRITE]);
}

int main(){

	int fds[2];

	pipe(fds);
	int f = fork();

	if (f == -1){
		perror("Pipe could not be opened");
		exit(1);
	}

	if (!f){//parent
		parent_stuff(fds, 15);
	} 
	else{//child
		child_stuff(fds);
	}

	return 0;
}
