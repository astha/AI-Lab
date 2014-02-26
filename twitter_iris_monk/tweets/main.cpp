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

unordered_map<string , int> vocab;
unordered_map<string , int> wordFreq;

vector<vector<string> > testData;
vector<vector<string> > trainingData;
vector<string> tweetVector;

vector<vector<int> > trainingOutput;
vector<vector<int> > testOutput;

// vector<vector<int> > inputVector;

vector<int> arrangement;
vector<vector<int> > input;
vector<vector<int> > testInput;
// 	vector<vector<int> > output;
neuronNetwork myNeuronGrid(arrangement,0);

int numInputs, numOutputs;
double threshold;

int wordCount;
char tweetCstring[5000];

void processTweet(string tweet){
	string tokenString;
	transform(tweet.begin(), tweet.end(), tweet.begin(), ::tolower);
	tweetVector.clear();
	char * token;
	strcpy(tweetCstring,tweet.c_str());
	token = strtok(tweetCstring, " \t\n");
	string tokenStr; 
	while (token != NULL){
		tokenStr = string(token);

		if (vocab.find(tokenStr) != vocab.end()){
			wordFreq[tokenStr]++;
		}
		else {
			// cout<<wordCount<<" and "<<tokenStr<<endl;
			vocab.insert(pair<string, int>(tokenStr,wordCount++));
			wordFreq[tokenStr] = 1;
		}
		tweetVector.push_back(tokenStr);

		// cout << tokenStr << endl;
		token = strtok(NULL, " \t\n");
	}
}

void deleteLowFreqWords(){
	unordered_map<string, int> newVocab;
	unordered_map<string, int> newFreq;

	int currIndex = 0;

	unordered_map<string, int>::iterator it;
	for(it = wordFreq.begin(); it != wordFreq.end(); it++){
		if(it->second>1 && (it->first).length() > 2){
			cout<<it->first<<" and "<<currIndex<<endl;
			newFreq.insert(pair<string, int>(it->first, it->second));
			newVocab.insert(pair<string, int>(it->first, currIndex++));
		}
	}
	vocab = newVocab;
	wordFreq = newFreq;
	wordCount = currIndex;
	// exit(0);
}


void readTweetFile(string filename , vector<int> output){
	int tweetCount = 0;
	ifstream fp;
	fp.open(filename.c_str());

	string tweet;

	while(!fp.eof()){
		tweetCount++;
		getline(fp, tweet);
		
		processTweet(tweet);
		if (tweetCount%5==0){
			testData.push_back(tweetVector);
			testOutput.push_back(output);
		}
		else {
			trainingData.push_back(tweetVector);
			trainingOutput.push_back(output);
		}
	}
	fp.close();
}

vector<int> createInputVector(vector<string> in){
	// cout<<wordCount<<endl;input
	// exit(0);
	vector<int> out(wordCount, 0);
	
	// cout<<"nums coming for in sinputize "<<in.size()<<"  :  ";
	for (int i = 0 ; i < in.size(); i++){
		if (vocab.find(in[i]) != vocab.end()){
			// cout<<vocab[in[i]]<<" ";
			out[vocab[in[i]]] ++;
		}
	}
	// cout<<endl;
	return out;
}

vector<int> createInputVector1(vector<string> in){
	// cout<<wordCount<<endl;input
	// exit(0);
	vector<int> out(wordCount, 0);
	
	cout<<"creating training vector : ";
	for (int i = 0 ; i < in.size(); i++){
		cout<<in[i]<<" ";
		if (vocab.find(in[i]) != vocab.end()){
			// cout<<vocab[in[i]]<<" ";
			out[vocab[in[i]]]++;
		}
	}
	cout<<endl;

	cout<<"printing formed vector : ";
	for (int i = 0 ; i < out.size(); i++){
		if (out[i] != 0){
			cout<<i<<" and "<<out[i]<<"    ";
		}
	}
	cout<<endl;
	cout<<endl;
	// cout<<endl;
	exit(0);
	return out;
}

void printInput(){

	for (int i = 0 ; i < trainingData.size(); i++){
		vector<int> a = createInputVector(trainingData[i]);
		// for (int k = 0 ; k < trainingData[i].size() ; k++){
		// 	cout << trainingData[i][k] << " ";
		// }
		// cout << endl;
		// cout << a.size() << endl;
		// for (int k = 0 ; k < a.size(); k++){
		// 	cout << a[k] << " ";
		// }
		// cout << endl;
		// cout << "YAHAN AAYA\n";
	}
	// cout << "OUT" << endl;

	// exit(0);	
}

void printFeatureVectorArgvector(vector<int> v, int i);
void testAllData();
void testOnData();

void trainData(){
	numInputs = wordCount;
	numOutputs = 3;
	arrangement = vector<int>{numInputs,4,4, numOutputs};
	myNeuronGrid = neuronNetwork(arrangement, 1);

	input.clear();

	threshold = 1.2;
	// cout<<"size of training data is "<<trainingData.size()<<endl;
	for (int i = 0 ; i < trainingData.size(); i++){
		input.push_back(createInputVector(trainingData[i]));
	}

	// exit(0);

	/*feed input and correct output*/
	double totalError = 0;
	lld numSteps = 0;
	cout<<"corpus size is "<<input.size()<<endl;
	while(1){
		totalError = 0;
		// cout << "HERE" << endl;
		// cout<<input.size()<<endl;
		
		for(int i = 0 ; i<input.size(); i++){
			// cout<<"*"<<flush;
			// exit(0);
			// cout<<"HERE "<<endl;
			// cout<<i<<endl;
			myNeuronGrid.feedInput(input[i], trainingOutput[i]);
			totalError += myNeuronGrid.propagateForward();
			myNeuronGrid.propagateBackward();
			// cout<<flush<<endl;
			// printFeatureVectorArgvector(input[i], i);
			// cout<<flush<<endl;
		}
		numSteps += input.size();
		// cout<<"================================================================================\n";
		cout<<totalError<<flush<<endl;
		// cout<<"================================================================================\n";
		if(totalError < threshold){
			break;
		}
	}
	testOnData();
}

void testAllData(){
// cout << "IN" << endl

	// cout << "FINAL" << endl;

	input.clear();

	threshold = 1;
	// cout<<"size of training data is "<<trainingData.size()<<endl;
	for (int i = 0 ; i < trainingData.size(); i+=5){
		input.push_back(createInputVector(trainingData[i]));
	}

	// exit(0);

	/*feed input and correct output*/
	/*try random training*/
	for(int i = 0 ; i<input.size(); i++){
		// cout<<"*"<<flush;
		// exit(0);
		// cout<<"HERE "<<endl;
		cout<<i<<endl;
		myNeuronGrid.feedInput(input[i], trainingOutput[i]);
		myNeuronGrid.propagateForward();
		myNeuronGrid.print();
		// cout<<flush<<endl;
		// printFeatureVectorArgvector(input[i], i);
		// cout<<flush<<endl;
	}
	// cout << totalError << endl;
}

void printFeatureVector(int i){
	vector<int> myfeature;
	myfeature = createInputVector(trainingData[i]);

	cout<<"Imp words in tweet :"<<endl;
	for(int a = 0 ; a<trainingData[i].size(); a++){
		cout<<trainingData[i][a]<<" ";
	}
	cout<<endl;

	numInputs = wordCount;
	numOutputs = 3;
	arrangement = vector<int>{numInputs, numOutputs};
	myNeuronGrid = neuronNetwork(arrangement, 1);

	double netVal = 0;

	cout<<"feature vector is with size : "<<myfeature.size()<<endl;
	for(int a = 0 ; a<myfeature.size(); a++){
		if(myfeature[a] != 0){
			cout<<a<<" , "<<myfeature[a]<<"  |  ";
			netVal += (myfeature[a]*myNeuronGrid.Grid[0][a].outputWeights[0]);
		}
	}
	// exit(0);
	cout<<endl<<flush;
	cout<<"netVal is "<<netVal<<" and the value comes out to be "<<1/(1+(pow(e,netVal)))<<endl;

}

void printFeatureVectorArgvector(vector<int> myfeature, int i){
	// vector<int> myfeature;
	// myfeature = createInputVector(trainingData[i]);

	cout<<"Imp words in tweet :"<<endl;
	for(int a = 0 ; a<trainingData[i].size(); a++){
		cout<<trainingData[i][a]<<" ";
	}
	cout<<endl;

	numInputs = wordCount;
	numOutputs = 3;
	arrangement = vector<int>{numInputs,4,4, numOutputs};
	myNeuronGrid = neuronNetwork(arrangement, 0);

	double netVal = 0;

	cout<<"feature vector is with size : "<<myfeature.size()<<endl;
	for(int a = 0 ; a<myfeature.size(); a++){
		if(myfeature[a] != 0){
			cout<<a<<" , "<<myfeature[a]<<"  |  ";
			netVal += (myfeature[a]*myNeuronGrid.Grid[0][a].outputWeights[0]);
		}
	}
	// exit(0);
	cout<<endl<<flush;
	cout<<"netVal is "<<netVal<<" and the value comes out to be "<<1/(1+(pow(e,netVal)))<<endl;

}


void testOnData(){
	int numPositive = 0;
	int numNegative = 0;
	int numObjective = 0;
	int correctPositive = 0;
	int correctNegative = 0;
	int correctObjective = 0;
	int numPositiveGiven = 0;
	int numNegativeGiven = 0;
	int numObjectiveGiven = 0;
	testInput.clear();
	double maximum = -20;
	// cout<<testData.size()<<" size 0 h testData ka"<<endl;
	for (int i = 0 ; i < testData.size(); i++){
		testInput.push_back(createInputVector(testData[i]));
	}
	// cout<<testInput.size()<<"yo there"<<endl;
	int correct = 0;
	for(int i = 0 ; i<testInput.size(); i++){
		myNeuronGrid.feedInput(testInput[i], testOutput[i]);
		myNeuronGrid.propagateForward();

		maximum = max(myNeuronGrid.Grid.back()[0].getOutput(), max(myNeuronGrid.Grid.back()[1].getOutput(), myNeuronGrid.Grid.back()[2].getOutput()));

		if(maximum == myNeuronGrid.Grid.back()[0].getOutput()){
			numNegativeGiven++;
		}
		else if(maximum == myNeuronGrid.Grid.back()[1].getOutput()){
			numObjectiveGiven++;
		}
		else{
			numPositiveGiven++;
		}

		if(myNeuronGrid.print() == 1){
			correct +=1;
			if(testOutput[i][0] == 1){
				numNegative++;
				correctNegative++;
			}
			else if(testOutput[i][1] == 1){
				numObjective++;
				correctObjective++;
			}
			else{
				numPositive++;
				correctPositive++;
			}
		}
		else{
			if(testOutput[i][0] == 1){
				numNegative++;
			}
			else if(testOutput[i][1] == 1){
				numObjective++;
			}
			else{
				numPositive++;
			}
		}

		// correct += myNeuronGrid.print();
		cout<<"\n\n";
	}
	cout<<"Recall(positive) :: "<<(double)correctPositive/(double)numPositive<<endl;
	cout<<"Recall(negative) :: "<<(double)correctNegative/(double)numNegative<<endl;
	cout<<"Recall(objective) :: "<<(double)correctObjective/(double)numObjective<<endl;
	cout<<"Precision(positive) :: "<<(double)correctPositive/(double)numPositiveGiven<<endl;
	cout<<"Precision(negative) :: "<<(double)correctNegative/(double)numNegativeGiven<<endl;
	cout<<"Precision(objective) :: "<<(double)correctObjective/(double)numObjectiveGiven<<endl;
	cout<<"Number of correct outputs = "<<correct<<" out of "<<testInput.size()<<" tweets"<<endl;
	// myNeuronGrid.printWeights();
}

void runTest(int signum){
	signal(SIGINT, runTest);
	testOnData();
}

int main(){

	wordCount = 0;
	string positiveTweetsFile, negativeTweetsFile, objectiveTweetsFile;
//	cout<<"Enter the path to positive tweets file :";
//	cin>>positiveTweetsFile;
	positiveTweetsFile = "featurePositive";
	
	negativeTweetsFile = "featureNegative";
	// negativeTweetsFile = "TweetsCorpus/twitter_objective";
	objectiveTweetsFile = "featureObjective";


	readTweetFile(negativeTweetsFile, vector<int>{1,0,0} );
	// cout<< wordCount<< endl;
// negative, neutral, positive
	readTweetFile(positiveTweetsFile, vector<int>{0,0,1} );
	// cout<< wordCount << endl;

	readTweetFile(objectiveTweetsFile, vector<int>{0,1,0} );
	// cout<< wordCount<< endl;

	signal(SIGINT, runTest);

	// readTweetFile(objectiveTweetsFile,vector<int>{0,1,0} );
	// cout<< wordCount << endl;
	// deleteLowFreqWords();

	// printInput();
	// printFeatureVector(0);
	// cout<<"######################################"<<endl;
	// printFeatureVector(1);
	// cout<<"######################################"<<endl;
	// printFeatureVector(2);
	// cout<<"######################################"<<endl;

	trainData();
	return 0;

}