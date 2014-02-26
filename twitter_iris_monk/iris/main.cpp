#include <cstdio>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>
#include <cstring>
#include <algorithm>
#include "ffnn.cpp"
#include <signal.h>
using namespace std;

#define e 2.7182818284

vector<vector<double> > testData;
vector<vector<double> > trainingData;

vector<vector<int> > trainingOutput;
vector<vector<int> > testOutput;

// vector<vector<int> > inputVector;

vector<int> arrangement;
vector<vector<int> > testInput;
// 	vector<vector<int> > output;
neuronNetwork myNeuronGrid(arrangement,0);

int numInputs, numOutputs;
double threshold;

void readIrisFile(string filename){
	int irisCount = 0;
	ifstream fp;
	fp.open(filename.c_str());

	string dataline;
	char mydataLine[200];
	char flowerType[20];
	
	while(!fp.eof()){
		getline(fp, dataline);
		// cout<<dataline<<endl;
		// exit(0);
		strcpy(mydataLine,dataline.c_str());
		vector<double> irisVector(4);
		// vector<double> irisVectorTemp(4);
		vector<int> output{0,0,0};

		sscanf(mydataLine, "%lf,%lf,%lf,%lf,%s", &irisVector[0], &irisVector[1], &irisVector[2], &irisVector[3], flowerType);
		// irisVector[0] = (int)(irisVectorTemp[0]*10);
		// irisVector[1] = (int)(irisVectorTemp[1]*10);
		// irisVector[2] = (int)(irisVectorTemp[2]*10);
		// irisVector[3] = (int)(irisVectorTemp[3]*10);

		if(strcmp(flowerType, "Iris-setosa") == 0){
			// cout<<"flo 1"<endl;
			output[0] = 1;
		}
		else if(strcmp(flowerType, "Iris-versicolor") == 0){
			// cout<<"flo 2"<endl;
			output[1] = 1;
		}
		else{
			output[2] = 1;
			// cout<<"flo 3"<endl;
		}

		// for(int i = 0 ;i<4; i++){
		// 	cout<<irisVector[i]<<" ";
		// }
		// cout<<" and ";
		// for(int i =0 ; i<3; i++){
		// 	cout<<output[i]<<" ";
		// }
		// cout<<endl;
		if (irisCount%5==1){
			testData.push_back(irisVector);
			testOutput.push_back(output);
		}
		else {
			trainingData.push_back(irisVector);
			trainingOutput.push_back(output);
		}
		irisCount++;
	}
	fp.close();
	// exit(0);
	// cout<<trainingData.size()<<" and "<<testData.size()<<endl;
	// exit(0);
}



// void printInput(){

// 	for (int i = 0 ; i < trainingData.size(); i++){
// 		vector<int> a = createInputVector(trainingData[i]);
// 		// for (int k = 0 ; k < trainingData[i].size() ; k++){
// 		// 	cout << trainingData[i][k] << " ";
// 		// }
// 		// cout << endl;
// 		// cout << a.size() << endl;
// 		// for (int k = 0 ; k < a.size(); k++){
// 		// 	cout << a[k] << " ";
// 		// }
// 		// cout << endl;
// 		// cout << "YAHAN AAYA\n";
// 	}
// 	// cout << "OUT" << endl;

// 	// exit(0);	
// }

// void printFeatureVectorArgvector(vector<int> v, int i);
// void testAllData();
void testOnData();

void trainData(){

	numInputs = 4 ;
	numOutputs = 3;
	arrangement = vector<int>{numInputs,4,3, numOutputs};
	myNeuronGrid = neuronNetwork(arrangement, 1);

	// input.clear();

	threshold = 1.5;
	/*feed input and correct output*/
	double totalError = 0;
	lld numSteps = 0;
	cout<<"corpus size is "<<trainingData.size()<<endl;
	while(1){
		totalError = 0;
		// cout << "HERE" << endl;
		// cout<<input.size()<<endl;
		
		for(int i = 0 ; i<trainingData.size(); i++){
			// cout<<"*"<<flush;
			// exit(0);
			// cout<<"HERE "<<endl;
			// cout<<i<<endl;
			myNeuronGrid.feedInput(trainingData[i], trainingOutput[i]);
			totalError += myNeuronGrid.propagateForward();
			myNeuronGrid.propagateBackward();
		}
		numSteps += trainingData.size();
		// cout<<"================================================================================\n";
		cout<<totalError<<flush<<endl;
		// cout<<"================================================================================\n";
		if(totalError < threshold){
			break;
		}
	}
	testOnData();
}

// void testAllData(){
// // cout << "IN" << endl

// 	// cout << "FINAL" << endl;

// 	input.clear();

// 	threshold = 1;
// 	// cout<<"size of training data is "<<trainingData.size()<<endl;
// 	for (int i = 0 ; i < trainingData.size(); i+=5){
// 		input.push_back(createInputVector(trainingData[i]));
// 	}

// 	// exit(0);

// 	/*feed input and correct output*/
// 	/*try random training*/
// 	for(int i = 0 ; i<input.size(); i++){
// 		// cout<<"*"<<flush;
// 		// exit(0);
// 		// cout<<"HERE "<<endl;
// 		cout<<i<<endl;
// 		myNeuronGrid.feedInput(input[i], trainingOutput[i]);
// 		myNeuronGrid.propagateForward();
// 		myNeuronGrid.print();
// 		// cout<<flush<<endl;
// 		// printFeatureVectorArgvector(input[i], i);
// 		// cout<<flush<<endl;
// 	}
// 	// cout << totalError << endl;
// }

// void printFeatureVector(int i){
// 	vector<int> myfeature;
// 	myfeature = createInputVector(trainingData[i]);

// 	cout<<"Imp words in tweet :"<<endl;
// 	for(int a = 0 ; a<trainingData[i].size(); a++){
// 		cout<<trainingData[i][a]<<" ";
// 	}
// 	cout<<endl;

// 	numInputs = wordCount;
// 	numOutputs = 3;
// 	arrangement = vector<int>{numInputs, numOutputs};
// 	myNeuronGrid = neuronNetwork(arrangement, 1);

// 	double netVal = 0;

// 	cout<<"feature vector is with size : "<<myfeature.size()<<endl;
// 	for(int a = 0 ; a<myfeature.size(); a++){
// 		if(myfeature[a] != 0){
// 			cout<<a<<" , "<<myfeature[a]<<"  |  ";
// 			netVal += (myfeature[a]*myNeuronGrid.Grid[0][a].outputWeights[0]);
// 		}
// 	}
// 	// exit(0);
// 	cout<<endl<<flush;
// 	cout<<"netVal is "<<netVal<<" and the value comes out to be "<<1/(1+(pow(e,netVal)))<<endl;

// }

// void printFeatureVectorArgvector(vector<int> myfeature, int i){
// 	// vector<int> myfeature;
// 	// myfeature = createInputVector(trainingData[i]);

// 	cout<<"Imp words in tweet :"<<endl;
// 	for(int a = 0 ; a<trainingData[i].size(); a++){
// 		cout<<trainingData[i][a]<<" ";
// 	}
// 	cout<<endl;

// 	numInputs = wordCount;
// 	numOutputs = 3;
// 	arrangement = vector<int>{numInputs,4,4, numOutputs};
// 	myNeuronGrid = neuronNetwork(arrangement, 0);

// 	double netVal = 0;

// 	cout<<"feature vector is with size : "<<myfeature.size()<<endl;
// 	for(int a = 0 ; a<myfeature.size(); a++){
// 		if(myfeature[a] != 0){
// 			cout<<a<<" , "<<myfeature[a]<<"  |  ";
// 			netVal += (myfeature[a]*myNeuronGrid.Grid[0][a].outputWeights[0]);
// 		}
// 	}
// 	// exit(0);
// 	cout<<endl<<flush;
// 	cout<<"netVal is "<<netVal<<" and the value comes out to be "<<1/(1+(pow(e,netVal)))<<endl;

// }


void testOnData(){

	// cout<<testInput.size()<<"yo there"<<endl;
	int correct = 0;
	for(int i = 0 ; i<testData.size(); i++){
		myNeuronGrid.feedInput(testData[i], testOutput[i]);
		myNeuronGrid.propagateForward();
		correct += myNeuronGrid.print();
		cout<<"\n\n";
	}
	cout<<"Number of correct outputs = "<<correct<<" out of "<<testData.size()<<" tweets"<<endl;
}

void runTest(int signum){
	signal(SIGINT, runTest);
	testOnData();
	// exit(0);
}

int main(){
	string irisDataFile;
	irisDataFile = "iris.data";


	readIrisFile(irisDataFile);

	signal(SIGINT, runTest);

	trainData();
	return 0;

}