#include <cstdio>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>
#include <cstring>
using namespace std;

unordered_map<string , int> vocab;
unordered_map<string , int> wordFreq;
char tweetCstring[5000];


string processTweet(string tweet){
	string tokenString
	char * token;
	tweetCstring = tweet.c_str();
	token = strtok(tweetCstring, " \",#.");
	while()
}

int main(){
	string positiveTweetsFile;
	cout<<"Enter the path to positive tweets file :";
	cin>>positiveTweetsFile;

	ifstream fp;
	fp.open(positiveTweetsFile.c_str());
	string tweet;

	while(!fp.eof()){
		getline(fp, tweet);
		tweet  = processTweet(tweet);
	}

	return 0;

}