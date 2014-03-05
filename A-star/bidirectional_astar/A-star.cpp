/*
FOR eight puzzle
takes as input start and goal puzzle configuration in row order form
*/
#include <sys/types.h>
     #include <unistd.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "node.h"
#include <fstream>
#include <signal.h>
using namespace std;

int steps;

int pid1, pid2;
FILE * write_file;

void handler(int signum){

	// if (signum == SIGUSR1) {
	// signal(SIGUSR1, handler);
	// signal(SIGUSR2, handler2);

	node::modify(node::choose());
	steps++;

	if(getpid() == pid1){
		kill(pid2,SIGUSR1);
	}
	else{
		kill(pid1,SIGUSR1);
	}
	// }
	// else if (signum == SIGUSR2){
// }
}

void handler2(int signum) {
	// signal(SIGUSR2, handler2);
	char line [250];
	int nodenum;
	cout << getpid() << endl;
	if (getpid() == pid1){
		FILE * fp1 = fopen("file1","r");
		fgets(line,250,fp1);
		nodenum = atoi(line);
		fclose(fp1);
		write_file = fopen("common_to_start", "w");
		// wait(0);
	}
	else {
		FILE * fp1 = fopen("file2","r");
		fgets(line,250,fp1);
		nodenum = atoi(line);
		fclose(fp1);

		write_file = fopen("common_to_goal", "w");
	}

	int length = 0;
	node * n = node::graph[nodenum];
	while(1){
		fputs(n->printState2().c_str(),write_file);
		if(n->id == 0){
			break;
		}
		length++;
		n = node::graph[n->pappa];
	}
	cout<<"length of path is "<<length<<endl;

	fclose(write_file);
	// if (pid1 == getpid()) wait(0);
	kill(getpid(),SIGKILL);

}


extern string filename;
extern string command;
int main(int argc, char** argv ){

	signal(SIGUSR1, handler);
	signal(SIGUSR2, handler2);
	pid1 = getpid();
	


	cout<<"Enter the startPuzzle in row order with 0 in place of gap"<<endl;
	int start[3][3];
	int goal[3][3];
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3 ; j++){
			cin>>start[i][j];
		}
	}

	cout<<"Enter the goalPuzzle in row order with 0 in place of gap"<<endl;

	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3 ; j++){
			cin>>goal[i][j];
		}
	}
	int goalNodeId, startNodeId;
	node startNode , goalNode;

	if((pid2 = fork()) != -1){
		if(pid2 == 0){
			pid2 = getpid();
			filename = "f2";
			startNode = node (goal);	
			goalNode = node (start);

		}
		else{
			filename = "f1";
			startNode = node (start);	
			goalNode = node (goal);
			
		}
	}
	char  command1[50];
	sprintf(command1, "./run.sh %d %d" , pid1, pid2);
	command.assign(command1);
	// node startNode = node(3,3,0);
	// node goalNode = node(0,0,1);
	// cout<<startNodeId<<endl;
	startNodeId = startNode.insertIntoGraph();
	goalNodeId = goalNode.insertIntoGraph();
	startNode.gn = 0;
	node::openList.insert(startNodeId);
	node::setStartNode(startNode);
	node::setGoalNode(goalNode);
	steps = 0;
	// cout << pid1 << " " << pid2 << endl;
	
	if (getpid() == pid1) {
		handler(10);

	}



	// cout<<"the variable is "<<prog<<endl;
	while(node::openList.find(goalNodeId)==node::openList.end()){
		// while(getenv("program1")[0] != prog){
		// 	// cout<<"hkkjhjkh k  "<<getenv("program1")[0]<<" hjkhkj"<<endl;
		// }
		// cout << "ASTHA"  << endl;
		// if (prog == '0') {
			// cout  << "ASTHA   5 " << endl	;
		// 	setenv("program1","1",1);
		// 	// system("export program1=1");	
		// 	cout<<getenv("program1")<<"yohjhkhkjhkj there"<<endl;			
		// }
		// else {
		// 	setenv("program1","0",1);
		// 	cout  << "ASTHA" << endl	;
		// }
	}
	cout<<"FOUND in "<<steps<<" steps "<<endl;

	cout<<"puzzles in reverse order"<<endl;
	// int length = 0;
	// node * n = node::graph[goalNodeId];
	// while(1){
	// 	n->printNode();
	// 	if(n->id == 0){
	// 		break;
	// 	}
	// 	length++;
	// 	n = node::graph[n->pappa];
	// }
	// cout<<"length of path is "<<length<<endl;
	if(node::monotonicity){
		cout<<"It is monotonic"<<endl;
	}else{
		cout<<"it is not monotonic"<<endl;
	}
	return 0;
}