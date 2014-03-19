#include "node.h"

map<int,node*> create_map()
{
  map<int,node*> m;
  return m;
}

set<int> create_set()
{
  set<int> m;
  return m;
}

int node::nextid = 0;

map<int,node*> node::graph=create_map();

set<int> node::openList=create_set();

set<int> node::closedList=create_set();

node node::startNode = node();

node node::goalNode = node();

bool node::monotonicity = true;


node::node(){}

int node::insertIntoGraph(){
	// cout<<"trying to insert node ";
	// printNode();
	// cout<<endl;
	map<int,node*>::iterator it = graph.begin();
	while(it!=graph.end()){
		// it->second->printNode();
		if(this->isSameAs(it->second)){
			//found
			// cout<<"graph has size = "<<graph.size()<<endl;
			return it->first;
		}
		it++;
	}
	//not found
	id = nextid;
	hn = calculateHn();
	nextid++;
	graph.insert(make_pair<int,node*>(id,this));
	neighbours.assign(0,make_pair<int,int>(1,1));
	// cout<<"graph has size = "<<graph.size()<<endl;
	return id;
}

list<pair<int,int> > node::choose(){
	int parent = 0;
	int minFn = 10000000;
	node * n;
	for(set<int>::iterator it=openList.begin();it!=openList.end();it++){
		// cout<<*it<<" is chosen "<<endl;
		// sleep(1);
		n = graph[*it];
		if(minFn > n->hn+n->gn){
			parent = n->id;
			minFn = n->hn+n->gn;
		}
	}
	node * parentNode = graph[parent];
	// parentNode->printNode();
	parentNode->generateNeighbours();

	list<pair<int,int> > min = parentNode->neighbours;

	for (list<pair<int,int> >::iterator it = min.begin(); it != min.end(); it++){
		(*it).second += parentNode->gn;
	}

	min.push_front(make_pair<int,int>((int)parent,0));
	openList.erase(parent);
	closedList.insert(parent);
	return min;
}

list<pair<int,int> > node::chooseFromClosedList(int id){
	int parent = id;
	node * parentNode = graph[parent];
	list<pair<int,int> > min = parentNode->neighbours;

	for (list<pair<int,int> >::iterator it = min.begin(); it != min.end(); it++){
		(*it).second += parentNode->gn;
	}

	min.push_front(make_pair<int,int>((int)parent,0));
	return min;
}

void node::modify(list<pair <int,int> > input){
	list <pair<int,int> >::iterator it = input.begin();
	int pappa = it->first;
	it++;
	while(it != input.end()){
		if(openList.find(it->first)!=openList.end()){
			/*present in openList*/
			if(graph[pappa]->hn > graph[it->first]->hn + 1){
				monotonicity = false;
			}
			if( graph[it->first]->gn > it->second ){
				/*modify connection*/
				graph[it->first]->gn = it->second;
				graph[it->first]->pappa = pappa;
			}
		}else if(closedList.find(it->first)!=closedList.end()){
			/*present in closedList*/
			if(graph[it->first]->gn > it->second){
				/*modify connection*/
				graph[it->first]->gn = it->second;
				graph[it->first]->pappa = pappa;
				modify(chooseFromClosedList(it->first));
			}
		}else{
			/*not present in either*/
			if(graph[pappa]->hn > graph[it->first]->hn + 1){
				monotonicity = false;
			}
			graph[it->first]->gn = it->second;
			openList.insert(it->first);
			graph[it->first]->pappa = pappa;
		}
		it++;
	}
}


// Following code must be uncommented when working on eight puzzle problem

/****************************************************************************/

node::node(int a[][3]){
	for(int i = 0 ; i < 3 ; i++){
		for(int j = 0; j < 3 ; j++){
			array[i][j] = a[i][j];
			if(array[i][j] == 0){
				x = j;
				y = i;
			}
		}
	}
}

bool node::isSameAs(node * n){
	int ans = true;
	for(int i = 0 ; i < 3 ; i++){
		for(int j = 0 ; j < 3 ; j++){
			if(array[i][j] != n->array[i][j]){
				ans = false;
			}
		}
	}
	return ans;
}

void node::generateNeighbours(){
	//left
	int temp;
	int neigh;
	int arrayTemp[3][3];
	for(int i = 0 ; i < 3 ; i++){
		for(int j = 0 ; j < 3 ; j++){
			arrayTemp[i][j] = array[i][j];
		}
	}
	//LEFT
	if(x-1>=0){
		arrayTemp[y][x] = arrayTemp[y][x-1];
		arrayTemp[y][x-1] = 0;
		node & n = *new node(arrayTemp);
		neighbours.push_back(make_pair<int,int>(n.insertIntoGraph(),1));
		arrayTemp[y][x-1] = arrayTemp[y][x];
		arrayTemp[y][x] = 0;
	}

	//RIGHT
	if(x+1<=2){
		arrayTemp[y][x] = arrayTemp[y][x+1];
		arrayTemp[y][x+1] = 0;
		node & n = *new node(arrayTemp);
		neighbours.push_back(make_pair<int,int>(n.insertIntoGraph(),1));
		arrayTemp[y][x+1] = arrayTemp[y][x];
		arrayTemp[y][x] = 0;
	}

	//DOWN
	if(y+1<=2){
		arrayTemp[y][x] = arrayTemp[y+1][x];
		arrayTemp[y+1][x] = 0;
		node & n = *new node(arrayTemp);
		neighbours.push_back(make_pair<int,int>(n.insertIntoGraph(),1));
		arrayTemp[y+1][x] = arrayTemp[y][x];
		arrayTemp[y][x] = 0;
	}

	//UP
	if(y-1>=0){
		arrayTemp[y][x] = arrayTemp[y-1][x];
		arrayTemp[y-1][x] = 0;
		node & n = *new node(arrayTemp);
		neighbours.push_back(make_pair<int,int>(n.insertIntoGraph(),1));
		arrayTemp[y-1][x] = arrayTemp[y][x];
		arrayTemp[y][x] = 0;
	}
}

void node::setStartNode(node n){
	startNode.id = n.id;
	startNode.x = n.x;
	startNode.y = n.y;
	for(int i = 0 ; i < 3; i++){
		for(int j = 0; j < 3 ; j++){
			startNode.array[i][j] = n.array[i][j];
		}
	}
}


void node::setGoalNode(node n){
	goalNode.id = n.id;
	goalNode.x = n.x;
	goalNode.y = n.y;
	for(int i = 0 ; i < 3; i++){
		for(int j = 0; j < 3 ; j++){
			goalNode.array[i][j] = n.array[i][j];
		}
	}
}

/*int node::calculateHn(){
	int ans = 0;
	for(int i = 0 ; i < 3 ; i++){
		for(int j = 0 ; j < 3 ; j++){
			if(array[i][j] != goalNode.array[i][j] && array[i][j] != 0){
				ans += 1;
			}
		}
	}
	return ans;
}*/

int node::calculateHn(){
	int ans = 0;
	for(int i = 0 ; i < 3 ; i++){
		for(int j = 0 ; j < 3 ; j++){
			for(int ii = 0 ; ii < 3 ; ii++){
				for(int jj = 0 ; jj < 3 ; jj++){
					if(array[i][j] == goalNode.array[ii][jj] && array[i][j] != 0){
						ans += abs(ii-i) + abs(jj-j);
					}
				}
			}
		}
	}
	return ans;
}

void node::printNode(){
	cout<<"\nid : "<<id<<endl;
	for(int i = 0 ; i < 3;i++){
		for(int j = 0 ; j < 3 ; j++){
			cout<<array[i][j]<<" ";
		}
		cout<<endl;
	}
}

/****************************************************************************/



// Following code must be uncommented when working on eight puzzle problem

/****************************************************************************/

	// node::node(int monsters, int humans, int bp){
	// 	config.first = monsters;
	// 	config.second = humans;
	// 	boatPos = bp;
	// }

	// bool node::isSameAs(node * n){
	// 	if(n->config.first==config.first && n->config.second == config.second && n->boatPos == boatPos){
	// 		// cout<<"true returned"<<endl;
	// 		// n->printNode();
	// 		return true;
	// 	}else{
	// 		// cout<<"false returned"<<endl;
	// 		return false;
	// 	}
	// }

	// void node::generateNeighbours(){
	// 	// transfer 1 human
	// 	if(boatPos == 0){
	// 		//left
	// 		if(config.second != 0){
	// 			// atleast on human present
	// 			if(config.second-1 == 0 || config.second-1 == 3 || config.first == config.second-1){
	// 				// safe
	// 				node & n = *new node(config.first , config.second-1, 1);
	// 				neighbours.push_back(make_pair<int,int>(n.insertIntoGraph(),1));
	// 			}
	// 		}
	// 	}else if(boatPos == 1){
	// 		//right
	// 		if(config.second <= 2){
	// 			//atleast one human present
	// 			if(config.second+1 == 0 || config.second+1 == 3 || config.first == config.second+1){
	// 				//safe
	// 				node & n = *new node(config.first , config.second+1, 0);
	// 				neighbours.push_back(make_pair<int,int>(n.insertIntoGraph(),1));
	// 			}
	// 		}
	// 	}

	// 	// transfer 1 monster
	// 	if(boatPos == 0){
	// 		//left
	// 		if(config.first != 0){
	// 			// atleast on monster present
	// 			if(config.second == 0 || config.second == 3 || config.first-1 == config.second){
	// 				// safe
	// 				node & n = *new node(config.first-1 , config.second, 1);
	// 				// cout<<"monster transfered"<<endl;
	// 				neighbours.push_back(make_pair<int,int>(n.insertIntoGraph(),1));
	// 			}
	// 		}
	// 	}else if(boatPos == 1){
	// 		//right
	// 		if(config.first <= 2){
	// 			//atleast one monster present
	// 			if(config.second == 0 || config.second == 3 || config.first+1 == config.second){
	// 				//safe
	// 				node & n = *new node(config.first+1 , config.second, 0);
	// 				neighbours.push_back(make_pair<int,int>(n.insertIntoGraph(),1));
	// 			}
	// 		}
	// 	}

	// 	// transfer 2 humans
	// 	if(boatPos == 0){
	// 		//left
	// 		if(config.second >= 2){
	// 			// atleast two humans
	// 			if(config.second-2 == 0 || config.second-2 == 3 || config.first == config.second-2){
	// 				//safe
	// 				node & n = *new node(config.first, config.second-2, 1);
	// 				neighbours.push_back(make_pair<int,int>(n.insertIntoGraph(),1));
	// 			}
	// 		}
	// 	}else if(boatPos == 1){
	// 		if(config.second <= 1){
	// 			//atleast two humans
	// 			if(config.second+2 == 0 || config.second+2 == 3 || config.first == config.second+2){
	// 				node & n = *new node(config.first, config.second+2, 0);
	// 				neighbours.push_back(make_pair<int,int>(n.insertIntoGraph(),1));
	// 			}
	// 		}
	// 	}

	// 	// transfer 2 monsters
	// 	if(boatPos == 0){
	// 		//left
	// 		if(config.first >= 2){
	// 			// atleast two monsters
	// 			if(config.second == 0 || config.second == 3 || config.first-2 == config.second){
	// 				//safe
	// 				node & n = *new node(config.first-2, config.second, 1);
	// 				// cout<<"2 monsters transfered"<<endl;
	// 				neighbours.push_back(make_pair<int,int>(n.insertIntoGraph(),1));
	// 			}
	// 		}
	// 	}else if(boatPos == 1){
	// 		if(config.first <= 1){
	// 			//atleast two monsers
	// 			if(config.second == 0 || config.second == 3 || config.first+2 == config.second){
	// 				node & n = *new node(config.first+2, config.second, 0);
	// 				neighbours.push_back(make_pair<int,int>(n.insertIntoGraph(),1));
	// 			}
	// 		}
	// 	}

	// 	// transfer 1 of each
	// 	if(boatPos == 0){
	// 		//left
	// 		if(config.first != 0 && config.second != 0){
	// 			//atleast one of each
	// 			if(config.second-1 == 0 || config.second-1 == 3 || config.first == config.second){
	// 				//safe
	// 				node & n = *new node(config.first-1, config.second-1, 1);
	// 				// cout<<"1 of eeach  transfered"<<endl;
	// 				neighbours.push_back(make_pair<int,int>(n.insertIntoGraph(),1));
	// 			}
	// 		}
	// 	}else if(boatPos == 1){
	// 		//right
	// 		if(config.first <= 2 && config.second <= 2){
	// 			//atleast one of each
	// 			if(config.second+1 == 0 || config.second+1 == 3 || config.first == config.second){
	// 				//safe
	// 				node & n = *new node(config.first+1, config.second+1, 0);
	// 				neighbours.push_back(make_pair<int,int>(n.insertIntoGraph(),1));
	// 			}
	// 		}
	// 	}
	// }

	// int node::calculateHn(){
	// 	return config.first+config.second;
	// }

	// void node::setStartNode(node n){
	// 	startNode.config.first = n.config.first;
	// 	startNode.config.second = n.config.second;
	// 	startNode.boatPos = n.boatPos;
	// }

	// void node::setGoalNode(node n){
	// 	goalNode.config.first = n.config.first;
	// 	goalNode.config.second = n.config.second;
	// 	goalNode.boatPos = n.boatPos;
	// }	

	// void node::printNode(){
	// 	cout<<"\nID : "<<id<<endl;
	// 	cout<<"monsters = "<<config.first<<"  and humans = "<<config.second<<" and boatPos = "<<boatPos<<endl;
	// }
/****************************************************************************/
