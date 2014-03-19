/*
FOR eight puzzle
takes as input start and goal puzzle configuration in row order form
*/

#include <iostream>
#include "node.h"
using namespace std;

int main(){
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

	node startNode = node(start);
	node goalNode = node(goal);
	startNodeId = startNode.insertIntoGraph();
	// cout<<startNodeId<<endl;
	goalNodeId = goalNode.insertIntoGraph();
	startNode.gn = 0;
	node::openList.insert(startNodeId);
	node::setStartNode(startNode);
	node::setGoalNode(goalNode);
	int steps = 0;
	while(node::openList.find(goalNodeId)==node::openList.end()){
		node::modify(node::choose());
		steps++;
	}
	cout<<"FOUND in "<<steps<<" steps "<<endl;

	cout<<"puzzles in reverse order"<<endl;
	int length = 0;
	node * n = node::graph[goalNodeId];
	while(1){
		n->printNode();
		if(n->id == 0){
			break;
		}
		length++;
		n = node::graph[n->pappa];
	}
	cout<<"length of path is "<<length<<endl;
	if(node::monotonicity){
		cout<<"It is monotonic"<<endl;
	}else{
		cout<<"it is not monotonic"<<endl;
	}
	return 0;
}