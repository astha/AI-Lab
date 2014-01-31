#include <cstdio>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>
#include <cstring>
#include <algorithm>
#include "ffnn.cpp"
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
	token = strtok(tweetCstring, " \",#.\t()!:\n");
	string tokenStr; 
	while (token != NULL){
		tokenStr = string(token);

		// Removing People Tags
		if (token[0] == '@' || strcmp(token, "\n") == 0 || strcmp(token, "\t") == 0) {
			token = strtok(NULL, " \",#.\t()!:\n");
			continue;
		}

		if (vocab.find(tokenStr) != vocab.end()){
			wordFreq[tokenStr] ++;
		}
		else {
			vocab[tokenStr] = wordCount++;
			wordFreq[tokenStr] = 1;
		}
		tweetVector.push_back(tokenStr);

		// cout << tokenStr << endl;
		token = strtok(NULL, " \",#.\t()!:\n");
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
		if (tweetCount%5!=0){
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
	vector<int> out(wordCount, 0);
	
	for (int i = 0 ; i < in.size(); i++){
		if (vocab.find(in[i]) != vocab.end()){
			out[vocab[in[i]]] ++;
		}
	}
	return out;
}


void trainData(){

	numInputs = wordCount;
	numOutputs = 3;
	arrangement = vector<int>{numInputs,wordCount/2, numOutputs};
	cout<< "KYA!" << endl;
	myNeuronGrid = neuronNetwork(arrangement, 0);

	cout<< "KYA!" << endl;


	threshold = 0.1;
	for (int i = 0 ; i < trainingData.size(); i++){
		input.push_back(createInputVector(trainingData[i]));

	}

	cout<< "KYA!" << endl;

/*feed input and correct output*/
	double totalError = 0;
	lld numSteps = 0;
	while(1 && numSteps<50000){
		totalError = 0;
		cout << "HERE" << endl;
		for(int i = 0 ; i<input.size(); i++){
			myNeuronGrid.feedInput(input[i], trainingOutput[i]);
			totalError += myNeuronGrid.propagateForward();
			myNeuronGrid.propagateBackward();
		}
		numSteps += input.size();
		// cout<<totalError<<endl;
		if(totalError < threshold){
			break;
		}
	}
}

int main(){

	wordCount = 0;
	string positiveTweetsFile, negativeTweetsFile, objectiveTweetsFile;
//	cout<<"Enter the path to positive tweets file :";
//	cin>>positiveTweetsFile;
	positiveTweetsFile = "TweetsCorpus/twitter_positive";
	
	negativeTweetsFile = "TweetsCorpus/twitter_negative";
	objectiveTweetsFile = "TweetsCorpus/twitter_objective";


// negative, neutral, positive
	readTweetFile(positiveTweetsFile, vector<int>{0,0,1} );
	cout<< "KYA!" << endl;

	readTweetFile(negativeTweetsFile, vector<int>{1,0,0} );
	cout<< "KYA!" << endl;

	readTweetFile(objectiveTweetsFile,vector<int>{0,1,0} );
cout<< "KYA!" << endl;

	trainData();
	return 0;

}