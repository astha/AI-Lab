#include <iostream>
#include <map>
#include <set>
#include <cmath>
#include <list>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h> 
using namespace std;


class node{

public:
	int id;
	
	int hn;
	int gn;
	int pappa;
	list <pair<int,int> > neighbours;

	static int nextid;
	static map<int, node*> graph;
	static set<int> openList;
	static set<int> closedList;
	static node startNode;
	static node goalNode;
	static bool monotonicity;

	node();
	int insertIntoGraph(); // return id of node inserted or found
	static list<pair<int,int> > choose();
	static list<pair<int,int> > chooseFromClosedList(int id);
	static void modify(list<pair<int,int> > children);

	//problem specific
	bool isSameAs(node * n);
	void generateNeighbours();
	int calculateHn();
	static void setStartNode(node n);
	static void setGoalNode(node n);
	void printNode();
// Following code must be uncommented when working on eight puzzle problem
/****************************************************************************/
	int array[3][3];
	// coordinates of blank
	int x;
	int y;
	node(int a[][3]);
/****************************************************************************/

// Following code must be uncommented when working on missionary problem
/****************************************************************************/
	// pair<int,int> config; // config on left side
	// int boatPos; // 0 : left , 1 : right
	// node(int monsters, int humans, int bp);
/****************************************************************************/	
};