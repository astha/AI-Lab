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
	token = strtok(tweetCstring, " \",#.\t()!:\n?[]");
	string tokenStr; 
	while (token != NULL){
		tokenStr = string(token);

		
		int length = tokenStr.length();

		// Removing People Tags
		if (token[0] == '@' || token[0] == '$' || strcmp(token, "\n") == 0 || strcmp(token, "\t") == 0 || length <= 2) {
			token = strtok(NULL, " \",#.\t()!:\n?[]");
			continue;
		}


		// cout<<tokenStr<<endl;
		if(tokenStr.substr(length-3, 3) == "ing"){
			tokenStr = tokenStr.substr(0,length-3);
			length -= 3;
		}


		else if(tokenStr.substr(length-2, 2) == "ed" || tokenStr.substr(length-2, 2) == "ly" || tokenStr.substr(length-2, 2) == "'s" ){
			tokenStr = tokenStr.substr(0,length-2);
			length -= 2;
		}

		if(length >= 2 && tokenStr.substr(0, 2) == "//"){
			tokenStr = tokenStr.substr(2,length-2);
			length -= 2;
		}

		
		// exit(0);
		// cout<<tokenStr<<endl;

		if (vocab.find(tokenStr) != vocab.end()){
			wordFreq[tokenStr] ++;
		}
		else {
			cout<<wordCount<<" and "<<tokenStr<<endl;
			vocab[tokenStr] = wordCount++;
			wordFreq[tokenStr] = 1;
		}
		tweetVector.push_back(tokenStr);

		// cout << tokenStr << endl;
		token = strtok(NULL, " \",#.\t()!:\n?[]");
	}
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
			trainingData.push_back(tweetVector);
			trainingOutput.push_back(output);
		}
		else {
			trainingData.push_back(tweetVector);
			trainingOutput.push_back(output);
		}
	}
	fp.close();
}

vector<int> createInputVector(vector<string> in){
	vector<int> out(wordCount, 0);
	
	for (int i = 0 ; i < in.size(); i++){
		if (vocab.find(in[i]) != vocab.end()){
			out[vocab[in[i]]] ++;
		}
	}
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
	
}

void trainData(){
// cout << "IN" << endl;
	numInputs = wordCount;
	numOutputs = 3;
	arrangement = vector<int>{numInputs, numOutputs};
	myNeuronGrid = neuronNetwork(arrangement, 0);

	// cout << "FINAL" << endl;

	threshold = 1;
	for (int i = 0 ; i < trainingData.size(); i++){
		input.push_back(createInputVector(trainingData[i]));

	}


/*feed input and correct output*/
	double totalError = 0;
	lld numSteps = 0;
	cout<<"corpus size is "<<input.size()<<endl;
	/*try random training*/
	// exit(0);
	while(1){
		totalError = 0;
		cout << "HERE" << endl;
		cout<<input.size()<<endl;
		exit(0);
		for(int i = 0 ; i<input.size(); i++){
			// cout<<"*"<<flush;
			// exit(0);
			
			myNeuronGrid.feedInput(input[i], trainingOutput[i]);
			totalError += myNeuronGrid.propagateForward();
			myNeuronGrid.propagateBackward();
		}
		numSteps += input.size();
		// cout<<"================================================================================\n";
		cout<<endl<<totalError<<endl;
		// cout<<"================================================================================\n";
		if(totalError < threshold){
			break;
		}
	}
	// cout << totalError << endl;
}


void testOnData(){

	// testInput.clear();
	// for (int i = 0 ; i < testData.size(); i++){
	// 	testInput.push_back(createInputVector(testData[i]));

	// }

	// for(int i = 0 ; i<testInput.size(); i++){
	// 	myNeuronGrid.feedInput(testInput[i], testOutput[i]);
	// 	int ones = 0;
	// 	for(int j = 0 ;j<testInput[i].size(); j++){
	// 		if(testInput[i][j])ones++;
	// 	}
	// 	cout<<ones<<endl;
	// 	myNeuronGrid.print();
	// 	cout<<"\n\n";
	// }
	myNeuronGrid.printWeights();
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
	positiveTweetsFile = "TweetsCorpus/twitter_positive";
	
	negativeTweetsFile = "TweetsCorpus/twitter_negative";
	// negativeTweetsFile = "TweetsCorpus/twitter_objective";
	objectiveTweetsFile = "TweetsCorpus/twitter_objective";


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


	printInput();

	trainData();

	while(1);
	return 0;

}