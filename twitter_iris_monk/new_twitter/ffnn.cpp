#include <iostream>
#include <cstdio>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <climits>
#include <sstream>
using namespace std;

typedef long long int lld;

#define e 2.7182818284
double eta = 0.1;
double mf = 0;
int mode;
int mfbool = 0;
class neuron{
public:
	int id;
	int layerId;
	double net;
	double output;
	vector<double> outputWeights;
	double delta;
	vector<double> prevOutputWeights;
	neuron();
	neuron(int id, int layerId);
	void initializeWeights(int nextLayerSize);
	void setOutput(double out);
	void calculateAndUpdateOutput(vector<neuron> & previousLayer);
	double getOutput();
	double getDelta();
	void setDelta(double delta);
	void updateHiddenDelta(vector<neuron> &nextLayer);
	void updateWeight(vector<neuron> &prevLayer);
	double getOutputWeight(int i);
	double setOutputWeight(int i, double val);
	void printWeights();
	double getPreviousOutputWeight(int id);
	void setPreviousOutputWeight(int id, double tmp);
};

double neuron::setOutputWeight(int i, double val){
	outputWeights[i] = val;
	return val;
}

neuron::neuron(int id, int layerId){
	this->id = id;
	this->layerId = layerId;
	outputWeights.clear();
	
}

double neuron::getOutputWeight(int i){
	return outputWeights[i];
}

double neuron::getDelta(){
	return delta;
}

double neuron::getPreviousOutputWeight(int id){
	return prevOutputWeights[id];
}

void neuron::setPreviousOutputWeight(int id, double tmp){
	prevOutputWeights[id]=tmp;
}

void neuron::updateWeight(vector<neuron> &prevLayer){
	double tmp;
	for(int i =0 ; i<prevLayer.size(); i++){
		if(mfbool == 0){
			prevLayer[i].setOutputWeight(id, prevLayer[i].getOutputWeight(id) + (eta*delta*prevLayer[i].getOutput()));
		}else{
			tmp = prevLayer[i].getOutputWeight(id);
			// cout<<(prevLayer[i].getOutputWeight(id) - prevLayer[i].getPreviousOutputWeight(id))<<endl;
			prevLayer[i].setOutputWeight(id, prevLayer[i].getOutputWeight(id) + ( mf*(prevLayer[i].getOutputWeight(id) - prevLayer[i].getPreviousOutputWeight(id))+(1-mf)*(eta*delta*prevLayer[i].getOutput())));
			prevLayer[i].setPreviousOutputWeight(id,tmp);
		}
	}

}

void neuron::updateHiddenDelta(vector<neuron> & nextLayer){
	delta = 0;
	for(int i =0; i<nextLayer.size()-1; i++){
		delta += outputWeights[i]*nextLayer[i].getDelta()*output*(1-output);
	}
}

void neuron::initializeWeights(int nextLayerSize){
	/*initializing all weights to 1*/
	srand(time(NULL));
	for(int i = 0 ; i<nextLayerSize; i++){
		outputWeights.push_back(double(rand())/INT_MAX);

		// outputWeights.push_back(i);
		prevOutputWeights.push_back(0.0);
	}
}

void neuron::setOutput(double out){
	output = out;
}

double neuron::getOutput(){
	return output;
}

void neuron::calculateAndUpdateOutput(vector<neuron> & previousLayer){
	/*calculate the input argument i.e. net = sigma(wi*xi) */
	net = 0;
	for(int i = 0 ; i < previousLayer.size(); i++){
		net += previousLayer[i].getOutputWeight(id)*previousLayer[i].getOutput();
	}

	/*calculate the output using the sigmoid function i.e. 1/(1+e^(-net))*/
	// cout<<layerId<<" and "<<id<<" net is  "<<net<<" "<<(double)1/(double)(1+pow(e,-net))<<endl;
	this->output = 1/(1+pow(e,-net));
	// this->output Math::Exp (flt32_t exp);
	// this->output = (double)flt64_t Math::Exp (flt64_t exp);
}

void neuron::printWeights(){
	cout<<" layerid = "<<layerId<<" ";
	for(int i = 0 ;i<outputWeights.size(); i++){
		cout<<outputWeights[i]<<" ";
	}
	cout<<endl;
}

/***************************************************************************************/
/***************************************************************************************/

class neuronNetwork{
public:
	vector<int> expectedOutput;
	vector<vector<neuron> > Grid;
	int bpMode;

	neuronNetwork();
	neuronNetwork(vector<int> arrangement, int mode);
	void feedInput(vector<int> inp, vector<int> out);
	double propagateForward();
	void propagateBackward();
	void propagateBackward_active();
	void propagateBackward_later();
	void setDelta(double delta);
	void updateWeights();
	int print();
	void printWeights();
};

void neuron::setDelta(double delta){
	this->delta = delta;
}

neuronNetwork::neuronNetwork(vector<int> arrangement, int mode){
	bpMode = mode;
	Grid.clear();
	int nextLayerSize;
	// cout << "YAHAN"  << endl;
	for(int i =0 ; i<arrangement.size(); i++){
		// cout << "YAHAN1"  << endl;
		Grid.push_back(vector<neuron>());
		// cout << "YAHAN2"  << endl;
		/*one extra neuron to take care of theta which will always output 1*/
		/*remember this adds one dummy neuron in the outermost layer as well*/
		for(int j = 0 ; j<=arrangement[i]; j++){
			// cout << "YAHAN3"  << endl;
			Grid.back().push_back(neuron(j,i));
			// cout << "YAHAN4"  << endl;
			// cout<<"neuron created id = "<<j<<" and layer = "<<i<<endl;
			Grid.back().back().initializeWeights(i == arrangement.size() ? 0 : arrangement[i+1]);
		}
		// cout << "YAHAN5"  << endl;
		Grid[i].back().setOutput(1);
		// cout << "YAHAN6"  << endl;
	}
	// cout << "DONE" << endl;
}

void neuronNetwork::feedInput(vector<int> inp, vector<int> out){
	expectedOutput = out;
	/*set the output of the neurons in the first layer*/
	int i;
	for(i = 0 ; i<inp.size(); i++){
		Grid[0][i].setOutput(inp[i]);
	}
	Grid[0][i].setOutput(1);
	// cout<<"setting output of dummy "<<i<<" "<<Grid[0][i].getOutput()<<endl;
}

double neuronNetwork::propagateForward(){
	int i, j;
	for(i = 1; i<Grid.size(); i++){
		for(j = 0; j<Grid[i].size()-1; j++){
			/*set the output of this neuron using previous layer neurons*/
			Grid[i][j].calculateAndUpdateOutput(Grid[i-1]);
			// cout<<i<<" "<<j<<" "<<Grid[i][j].getOutput()<<" this is what i get "<<endl;
		}
		Grid[i][j].setOutput(1);
	}
	vector<neuron> &lastLayer = Grid.back();
	double curError = 0;
	for(int i = 0 ; i<lastLayer.size()-1; i++) {
		// cout<<lastLayer[i].layerId<<" "<<lastLayer[i].id<<" "<<lastLayer[i].getOutput()<<" see me"<<endl;
		curError += (expectedOutput[i] - lastLayer[i].getOutput())*(expectedOutput[i] - lastLayer[i].getOutput());
	}
	return curError;
}

void neuronNetwork::propagateBackward(){
	if(bpMode == 0) propagateBackward_active();
	else propagateBackward_later();
}

void neuronNetwork::propagateBackward_active(){
	/*calculate deta values for all the edges, updates on weights will be done later*/

	/*output layer*/
	vector<neuron> &outputLayer = Grid.back();
	double delta;
	for(int i = 0 ; i<Grid.back().size()-1; i++){
		delta = (expectedOutput[i] - outputLayer[i].getOutput())*outputLayer[i].getOutput()*(1-outputLayer[i].getOutput());
		outputLayer[i].setDelta(delta);
	}
	/*set the delta for dummy neuron in the last layer = 0*/
	outputLayer.back().setDelta(0);

	/*for hidden layers*/
	int lastLayer = Grid.size()-1;
	for(int i = 0 ; i<Grid[lastLayer].size()-1; i++){
		Grid[lastLayer][i].updateWeight(Grid[lastLayer-1]);
	}
	
	for(int i = Grid.size()-2; i>=1; i--){
		vector<neuron> &currentLayer = Grid[i];
		for(int j = 0 ; j<currentLayer.size(); j++){
			currentLayer[j].updateHiddenDelta(Grid[i+1]);
		}
		for(int j = 0 ; j<currentLayer.size()-1; j++){
			currentLayer[j].updateWeight(Grid[i-1]);
		}
	}
}

void neuronNetwork::propagateBackward_later(){
	/*calculate deta values for all the edges, updates on weights will be done later*/

	/*output layer*/
	vector<neuron> &outputLayer = Grid.back();
	double delta;
	for(int i = 0 ; i<Grid.back().size()-1; i++){
		delta = (expectedOutput[i] - outputLayer[i].getOutput())*outputLayer[i].getOutput()*(1-outputLayer[i].getOutput());
		outputLayer[i].setDelta(delta);
	}
	/*set the delta for dummy neuron in the last layer = 0*/
	outputLayer.back().setDelta(0);

	/*for hidden layers*/
	int lastLayer = Grid.size()-1;
	for(int i = 0 ; i<Grid[lastLayer].size()-1; i++){
		Grid[lastLayer][i].updateWeight(Grid[lastLayer-1]);
	}
	
	for(int i = Grid.size()-2; i>=1; i--){
		vector<neuron> &currentLayer = Grid[i];
		for(int j = 0 ; j<currentLayer.size(); j++){
			currentLayer[j].updateHiddenDelta(Grid[i+1]);
		}
		for(int j = 0 ; j<currentLayer.size()-1; j++){
			currentLayer[j].updateWeight(Grid[i-1]);
		}
	}
	updateWeights();
}

void neuronNetwork::updateWeights(){
	for(int i = 1; i<Grid.size(); i++){
		for(int j = 0; j<Grid[i].size(); j++){
			Grid[i][j].updateWeight(Grid[i-1]);
		}
	}
}

int neuronNetwork::print(){

	cout<<"net on the output neurons"<<endl;
	for(int i = 0 ; i<Grid.back().size(); i++){
		cout<<Grid.back()[i].net<<" ";
	}
	cout<<endl;
	cout<<"Expected Output ";
	for(int i = 0 ; i<expectedOutput.size(); i++){
		cout<<expectedOutput[i]<<" ";
	}
	cout<<endl;
	cout<<"Current Output  ";
	for(int i = 0 ; i<expectedOutput.size(); i++){
		cout<<Grid.back()[i].getOutput()<<" ";
	}
	cout<<endl;

	int k = 0; 
	for(int i = 0; i<expectedOutput.size(); i++){
		if(abs(Grid.back()[i].getOutput() - expectedOutput[i]) < 0.2) k++;
	}
	if(k == expectedOutput.size()){
		cout<<"correct"<<endl;
		return 1;
	}
	else {
		cout<<"wrong"<<endl;
		return 0;
	}
	
}

vector<vector<int> > genTruthTable(int n){
	lld twoPowN = 1<<n;
	vector<vector<int> > v(twoPowN, vector<int>(n,0));

	int curVal = twoPowN/2;
	bool alternate = true;
	for(int j = n-1 ;j>=0; j--){
		alternate = true;
		for(int i = 0; i<twoPowN; i++){
			if(alternate) v[i][n-1-j] = 0;
			else v[i][n-1-j] = 1;
			if((i+1)%curVal == 0) {
				alternate = !alternate;
			}
		}
		curVal /= 2;
	}
	return v;
}


void neuronNetwork::printWeights(){
	for(int i = 0 ; i<Grid[0].size(); i++){
		cout<<"Neuron "<<i<<" ";
		Grid[0][i].printWeights();
		cout<<endl;
		cout<<"net value is "<<Grid[0][i].net;
		cout<<endl;
	}
	cout<<endl;
	for(int i = 0 ; i<Grid[1].size(); i++){
		cout<<"Neuron layer1 "<<i<<" ";
		// Grid[1][i].printWeights();
		// cout<<endl;
		cout<<"net value is "<<Grid[1][i].net;
		cout<<endl;
	}

}


vector<vector<int> > genOutputTable_segment(int n, int m){

/*
	    2
     ______
 1  |      | 3
    | ____ |
 6  |   0  | 4
    |______|
         5
*/

/*
	0 -> 0111111
	1 -> 0001100
	2 -> 1011011
	3 -> 1011110
	4 -> 1101100
	5 -> 1110110
	6 -> 1110111
	7 -> 0011100
	8 -> 1111111
	9 -> 1111100
*/

	lld twoPowN = 1<<n;
	vector<vector<int> > v(twoPowN, vector<int>(m,1));
	v[63] = vector<int>{0,0,0,0};
	v[12] = vector<int>{0,0,0,1};
	v[91] = vector<int>{0,0,1,0};
	v[94] = vector<int>{0,0,1,1};
	v[108] = vector<int>{0,1,0,0};
	v[118] = vector<int>{0,1,0,1};
	v[119] = vector<int>{0,1,1,0};
	v[28] = vector<int>{0,1,1,1};
	v[127] = vector<int>{1,0,0,0};
	v[124] = vector<int>{1,0,0,1};
	return v;
}

void majorityAndParity(vector<vector<int> > & out, vector<vector<int> > & in, bool major){
	int curCount = 0;
	int n = 5;
	for(int i = 0; i<in.size(); i++){
		curCount = 0;
		for(int j = 0 ;j<n; j++){
			if(in[i][j]){
				curCount++;
			}
		}
		if(curCount > n/2 && major){
			out[i][0] = 1;
		}
		else if(curCount%2==1 && !major){
			out[i][0] = 1;
		}
	}
}


// int main(int argv, char ** argc){
// 	/*Mode :
// 	1 - epoch mode
// 	0 - not epoch mode
// 	*/

// 	sscanf(argc[1], "%lf", &mf);
// 	sscanf(argc[2], "%lf", &eta);
// 	// sscanf(argc[3], "%c", &type);

// 	if(mf != 0){
// 		mfbool = 1;
// 	}else{
// 		mfbool = 0;
// 	}

// 	cout<<"Enter the operation\n";
// 	cout<<"(a) 2-input NAND\n";
// 	cout<<"(b) 2-input NOR\n";
// 	cout<<"(c) 2-input XOR\n";
// 	cout<<"(d) 5-input Palindrome\n";
// 	cout<<"(e) 5-input Majority\n";
// 	cout<<"(f) 6-input Even Parity\n";
// 	cout<<"(g) 7 segment display with only valid inputs\n";
// 	cout<<"(h) 7 segment display with all inputs\n";


// 	char type;
// 	cin>>type;

// 	vector<int> arrangement;
// 	vector<vector<int> > input;
// 	vector<vector<int> > output;

// 	int numInputs, numOutputs;
// 	double threshold;

// 	neuronNetwork myNeuronGrid(arrangement, 0);

// 	switch(type){
// 		case 'a':
// 			/*nand network*/
// 			arrangement = vector<int>{2,1};
// 			numInputs = 2;
// 			numOutputs = 1;
// 			myNeuronGrid = neuronNetwork(arrangement, 0);
// 			input = vector<vector<int> >{
// 				vector<int>{0,0},
// 				vector<int>{0,1},
// 				vector<int>{1,0},
// 				vector<int>{1,1}
// 			};
// 			output = vector<vector<int> >{
// 				vector<int>{1},
// 				vector<int>{1},
// 				vector<int>{1},
// 				vector<int>{0}
// 			};
// 			threshold = 0.01;
// 			break;

// 		case 'b':
// 			/*nor network*/
// 			arrangement = vector<int>{2,1};
// 			numInputs = 2;
// 			numOutputs = 1;
// 			myNeuronGrid = neuronNetwork(arrangement, 0);
// 			input = vector<vector<int> >{
// 				vector<int>{0,0},
// 				vector<int>{0,1},
// 				vector<int>{1,0},
// 				vector<int>{1,1}
// 			};
// 			output = vector<vector<int> >{
// 				vector<int>{1},
// 				vector<int>{0},
// 				vector<int>{0},
// 				vector<int>{0}
// 			};
// 			threshold = 0.01;
// 			break;

// 		case 'c':
// 			/*xor network*/
// 			arrangement = vector<int>{2,2,1};
// 			numInputs = 2;
// 			numOutputs = 1;
// 			myNeuronGrid = neuronNetwork(arrangement, 0);
// 			input = vector<vector<int> >{
// 				vector<int>{0,0},
// 				vector<int>{0,1},
// 				vector<int>{1,0},
// 				vector<int>{1,1}
// 			};
// 			output = vector<vector<int> >{
// 				vector<int>{0},
// 				vector<int>{1},
// 				vector<int>{1},
// 				vector<int>{0}
// 			};
// 			threshold = 0.01;
// 			break;

// 		case 'd':
// 			/*5 input palindrome*/
// 			arrangement = vector<int>{5,4,1};
// 			numInputs = 5;
// 			numOutputs = 1;
// 			myNeuronGrid = neuronNetwork(arrangement, 0);
// 			input = genTruthTable(numInputs);
// 			output = vector<vector<int> >{1<<numInputs, vector<int>{0}};
// 			output[0]= vector<int>{1};
// 			output[10]= vector<int>{1};
// 			output[17]= vector<int>{1};
// 			output[27]= vector<int>{1};
// 			output[4]= vector<int>{1};
// 			output[14]= vector<int>{1};
// 			output[21]= vector<int>{1};
// 			output[31]= vector<int>{1};
// 			threshold = 0.01;

// 		case 'e':
// 			/*5 input majority*/
// 			arrangement = vector<int>{5,1};
// 			numInputs = 5;
// 			numOutputs = 1;
// 			myNeuronGrid = neuronNetwork(arrangement, 0);
// 			input = genTruthTable(numInputs);
// 			output = vector<vector<int> >{1<<numInputs, vector<int>{0}};
// 			majorityAndParity(output, input, true);
// 			threshold = 0.01;
// 			break;

// 		case 'f':
// 			/*5 input parity*/
// 			arrangement  = vector<int>{5,4,1};
// 			numInputs = 5;
// 			numOutputs = 1;
// 			myNeuronGrid = neuronNetwork(arrangement, 0);
// 			input = genTruthTable(numInputs);
// 			output = vector<vector<int> >{1<<numInputs, vector<int>{0}};
// 			majorityAndParity(output, input, false);
// 			threshold = 0.32;
// 			break;

// 		case 'g':
// 			/*7 segment display*/
// 			arrangement = vector<int>{7,5,4};
// 			numInputs = 7;
// 			numOutputs = 4;
// 			myNeuronGrid = neuronNetwork(arrangement, 0);
// 			input = vector<vector<int> >{
// 				vector<int>{0,1,1,1,1,1,1},
// 				vector<int>{0,0,0,1,1,0,0},
// 				vector<int>{1,0,1,1,0,1,1},
// 				vector<int>{1,0,1,1,1,1,0},
// 				vector<int>{1,1,0,1,1,0,0},
// 				vector<int>{1,1,1,0,1,1,0},
// 				vector<int>{1,1,1,0,1,1,1},
// 				vector<int>{0,0,1,1,1,0,0},
// 				vector<int>{1,1,1,1,1,1,1},
// 				vector<int>{1,1,1,1,1,0,0}
// 			};
// 			output = vector<vector<int> >{
// 				vector<int>{0,0,0,0},
// 				vector<int>{0,0,0,1},
// 				vector<int>{0,0,1,0},
// 				vector<int>{0,0,1,1},
// 				vector<int>{0,1,0,0},
// 				vector<int>{0,1,0,1},
// 				vector<int>{0,1,1,0},
// 				vector<int>{0,1,1,1},
// 				vector<int>{1,0,0,0},
// 				vector<int>{1,0,0,1}
// 			};
// 			threshold = 0.01;
// 			break;

// 		case 'h':
// 			/*7 segment display*/
// 			arrangement = vector<int>{7,4,4};
// 			numInputs = 7;
// 			numOutputs = 4;
// 			myNeuronGrid = neuronNetwork(arrangement, 0);
// 			input = genTruthTable(numInputs);
// 			output = genOutputTable_segment(numInputs,numOutputs);
// 			threshold = 3.1;
// 			break;
// 	}


// /*TRAINING DATA*/

// /*feed input and correct output*/
// 	double totalError = 0;
// 	lld numSteps = 0;
// 	while(1 && numSteps<50000){
// 		totalError = 0;
// 		for(int i = 0 ; i<input.size(); i++){
// 			myNeuronGrid.feedInput(input[i], output[i]);
// 			totalError += myNeuronGrid.propagateForward();
// 			myNeuronGrid.propagateBackward();
// 		}
// 		numSteps += input.size();
// 		// cout<<totalError<<endl;
// 		if(totalError < threshold){
// 			break;
// 		}
// 	};

// 	if(numSteps >= 50000){
// 		numSteps = 0;
// 	}

// 	// cout<<"Number of steps taken = "<<numSteps<<endl;
// 	cout<<numSteps<<endl;

// 	// for(int i = 0 ; i<input.size(); i++){
// 	// 	cout<<"\n\n";
// 	// 	cout<<"input -> ";
// 	// 	for(int g = 0 ; g<input[i].size(); g++){
// 	// 		cout<<input[i][g]<<" ";
// 	// 	}
// 	// 	cout<<endl;
// 	// 	myNeuronGrid.feedInput(input[i], output[i]);
// 	// 	totalError += myNeuronGrid.propagateForward();
// 	// 	myNeuronGrid.print();
// 	// }

// 	return 0;
// }


//10.144.22.120:1997/twitterapp

//www.clift.iitb.ac.in/twitterapp