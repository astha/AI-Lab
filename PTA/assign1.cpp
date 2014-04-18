#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <list>
#include <map>
#include <ctime>
#include <vector>
#include <cstdio>
using namespace std;

int soln[11];
int twoPowN;
int matrix[1025][11];
int n;

map<vector<int>,int > mySet;


int recurse(){

	int val = 0;
	for(int i = 0 ;i<twoPowN; i++){
		val = 0;
		for(int j = 0 ; j<n+1; j++){
			val += (matrix[i][j]*soln[j]);
		}
		if(val <= 0){
			return i;
		}
	}
	return -1;
}

void nand(){
	// matrix[twoPowN-1][n] = -1;
	for(int i = 0; i<n; i++){
		matrix[twoPowN-1][i] = -1;
	}
	matrix[twoPowN-1][n] = 1;
}

void nor(){
	matrix[0][n] = -1;
	for(int i = 1; i<twoPowN; i++){
		for(int j = 0 ;j<n+1; j++){
			matrix[i][j] *= -1;
		}
	}
}

void xoor(){
	int curCount = 0;
	int j;
	for(int i = 0; i<twoPowN; i++){
		curCount = 0;
		for(j = 0 ;j<n; j++){
			if(matrix[i][j]){
				curCount++;
			}
		}
		if(!(curCount % 2 == 1)){
			for(int k = 0 ; k<n+1; k++){
				matrix[i][k] *= -1;
			}
		}
	}
}

void majority(){
	int curCount = 0;
	for(int i = 0; i<twoPowN; i++){
		curCount = 0;
		for(int j = 0 ;j<n; j++){
			if(matrix[i][j]){
				curCount++;
			}
		}
		if(!(curCount > n/2)){
			for(int k = 0 ; k<n+1; k++){
				matrix[i][k] *= -1;
			}
		}
	}
}

void palindrome(){
	bool isPalin = true;
	for(int i =0; i<twoPowN; i++){
		//check palindrome
		isPalin = true;
		for(int j = 0 ;j<n/2; j++){
			if(matrix[i][j] != matrix[i][n-j-1]){
				isPalin = false;
				break;
			}
		}
		if(!isPalin){
			for(int k = 0 ; k<n+1; k++){
				matrix[i][k] *= -1;
			}
		}
	}
}


void evenParity(){
	int curCount = 0;
	for(int i = 0; i<twoPowN; i++){
		curCount = 0;
		for(int j = 0 ;j<n; j++){
			if(matrix[i][j]){
				curCount++;
			}
		}
		if(curCount % 2 == 1){
			for(int k = 0 ; k<n+1; k++){
				matrix[i][k] *= -1;
			}
		}
	}
}


void segment0(){
	for(int i = 0; i<twoPowN; i++){
		if (!(!matrix[i][0] && matrix[i][1] && matrix[i][2] && matrix[i][3] && matrix[i][4] 
		   && matrix[i][5] && matrix[i][6])){
			for(int k = 0 ; k<n+1; k++){
				matrix[i][k] *= -1;
			}
		}
	}
}

void segment1(){
	for(int i = 0; i<twoPowN; i++){
		if (!(!matrix[i][0] && !matrix[i][1] && !matrix[i][2] && matrix[i][3] && matrix[i][4] 
		   && !matrix[i][5] && !matrix[i][6])){
			for(int k = 0 ; k<n+1; k++){
				matrix[i][k] *= -1;
			}
		}
	}
}

void segment2(){
	for(int i = 0; i<twoPowN; i++){
		if (!(matrix[i][0] && !matrix[i][1] && matrix[i][2] && matrix[i][3] && !matrix[i][4] 
		   && matrix[i][5] && matrix[i][6])){
			for(int k = 0 ; k<n+1; k++){
				matrix[i][k] *= -1;
			}
		}
	}
}

void segment3(){
	for(int i = 0; i<twoPowN; i++){
		if (!(matrix[i][0] && !matrix[i][1] && matrix[i][2] && matrix[i][3] && matrix[i][4] 
		   && matrix[i][5] && !matrix[i][6])){
			for(int k = 0 ; k<n+1; k++){
				matrix[i][k] *= -1;
			}
		}
	}
}

void segment4(){
	for(int i = 0; i<twoPowN; i++){
		if (!(matrix[i][0] && matrix[i][1] && !matrix[i][2] && matrix[i][3] && matrix[i][4] 
		   && !matrix[i][5] && !matrix[i][6])){
			for(int k = 0 ; k<n+1; k++){
				matrix[i][k] *= -1;
			}
		}
	}
}

void segment5(){
	for(int i = 0; i<twoPowN; i++){
		if (!(matrix[i][0] && matrix[i][1] && matrix[i][2] && !matrix[i][3] && matrix[i][4] 
		   && matrix[i][5] && !matrix[i][6])){
			for(int k = 0 ; k<n+1; k++){
				matrix[i][k] *= -1;
			}
		}
	}
}

void segment6(){
	for(int i = 0; i<twoPowN; i++){
		if (!(matrix[i][0] && matrix[i][1] && matrix[i][2] && !matrix[i][3] && matrix[i][4] 
		   && matrix[i][5] && matrix[i][6])){
			for(int k = 0 ; k<n+1; k++){
				matrix[i][k] *= -1;
			}
		}
	}
}

void segment7(){
	for(int i = 0; i<twoPowN; i++){
		if (!(!matrix[i][0] && !matrix[i][1] && matrix[i][2] && matrix[i][3] && matrix[i][4] 
		   && !matrix[i][5] && !matrix[i][6])){
			for(int k = 0 ; k<n+1; k++){
				matrix[i][k] *= -1;
			}
		}
	}
}

void segment8(){
	for(int i = 0; i<twoPowN; i++){
		if (!(matrix[i][0] && matrix[i][1] && matrix[i][2] && matrix[i][3] && matrix[i][4] 
		   && matrix[i][5] && matrix[i][6])){
			for(int k = 0 ; k<n+1; k++){
				matrix[i][k] *= -1;
			}
		}
	}
}

void segment9(){
	for(int i = 0; i<twoPowN; i++){
		if (!(matrix[i][0] && matrix[i][1] && matrix[i][2] && matrix[i][3] && matrix[i][4] 
		   && matrix[i][5] && !matrix[i][6])){
			for(int k = 0 ; k<n+1; k++){
				matrix[i][k] *= -1;
			}
		}
	}
}


void genTruthTable(){
	//initialize sol'
	srand(time(NULL));
	for(int i = 0; i<n+1; i++){
		soln[i] = 2;
	}
	twoPowN = 1<<n;
	int curVal = twoPowN/2;
	bool alternate = true;
	for(int j = n-1 ;j>=0; j--){
		alternate = true;
		for(int i = 0; i<twoPowN; i++){
			if(alternate) matrix[i][j] = 0;
			else matrix[i][j] = 1;
			if((i+1)%curVal == 0) {
				alternate = !alternate;
			}
		}
		curVal /= 2;
	}

	for(int i = 0; i<twoPowN; i++){
		matrix[i][n] = -1;
	}
}


int main(int argv, char ** argc){
	// string s = argc[1];
	float C = atof(argc[1]);
	C = C;
	// cout<<C<<endl;
	// exit(0);
	FILE * fp1 = fopen("outputs", "a+");
	// cout<<C<<endl;

	// exit(0);
	// int 	C = 4;
	cout<<"Enter the operation\n";
	cout<<"(a) 2-input NAND\n";
	cout<<"(b) 2-input NOR\n";
	cout<<"(c) 2-input XOR\n";
	cout<<"(d) 5-input Palindrome\n";
	cout<<"(e) 5-input Majority\n";
	cout<<"(f) 6-input Even Parity\n";
	cout<<"               \n";
	cout<<"    ___2___    \n";
	cout<<"   |       |   \n";
	cout<<" 1 |       | 3 \n";
	cout<<"   |-- 0 --|   \n";
	cout<<" 6 |       | 4 \n";
	cout<<"   |_______|   \n";
	cout<<"       5       \n";
	cout<<"\n";


	cout<<"(0) 7-Segment Display 0\n";
	cout<<"(1) 7-Segment Display 1\n";
	cout<<"(2) 7-Segment Display 2\n";
	cout<<"(3) 7-Segment Display 3\n";
	cout<<"(4) 7-Segment Display 4\n";
	cout<<"(5) 7-Segment Display 5\n";
	cout<<"(6) 7-Segment Display 6\n";
	cout<<"(7) 7-Segment Display 7\n";
	cout<<"(8) 7-Segment Display 8\n";
	cout<<"(9) 7-Segment Display 9\n";


	char type;
	// cin>>type;
	type = 'a';

	switch(type){
		case 'a':
			n = 2;
			genTruthTable();
			nand();
			break;
		case 'b':
			n = 2;
			genTruthTable();
			nor();
			break;
		case 'c':
			n = 2;
			genTruthTable();
			xoor();
			break;
		case 'd':
			n = 5;
			genTruthTable();
			palindrome();
			break;
		case 'e':
			n = 5;
			genTruthTable();
			majority();
			break;
		case 'f':
			n = 6;
			genTruthTable();
			evenParity();
			break;

		case'1':
			n = 7;
			genTruthTable();
			segment1();
			break;
		case'2':
			n = 7;
			genTruthTable();
			segment2();
			break;
		case'3':
			n = 7;
			genTruthTable();
			segment3();
			break;
		case'4':
			n = 7;
			genTruthTable();
			segment4();
			break;
		case'5':
			n = 7;
			genTruthTable();
			segment5();
			break;
		case'6':
			n = 7;
			genTruthTable();
			segment6();
			break;
		case'7':
			n = 7;
			genTruthTable();
			segment7();
			break;
		case'8':
			n = 7;
			genTruthTable();
			segment8();
			break;
		case'9':
			n = 7;
			genTruthTable();
			segment9();
			break;
		case'0':
			n = 7;
			genTruthTable();
			segment0();
			break;
	}


	for(int j = 0 ;j<twoPowN; j++){
		for(int i = 0 ;i<n+1; i++){
			printf("%3d ", matrix[j][i]);
		}
		printf("\n");
	}
	cout<<endl<<endl;

	int curBreak;
	int numSteps = 0;
	for(int i = 0; i<n+1; i++){
		printf("%3d ", soln[i]);
	}
	cout<<endl;
	while(1){
		numSteps++;
		curBreak = recurse();
		// cout<<curBreak<<endl;
		if(curBreak < 0){
			cout<<"Solution found!!\n";
			cout<<"Num of steps taken = "<<numSteps<<endl;
			fprintf(fp1, "%lf %d\n", C, numSteps);
			for(int i = 0; i<n+1; i++){
				printf("%3d ", soln[i]);
			}
			cout<<endl;
			fclose(fp1);
			return 0;
		}

		else{

			for(int i = 0; i<n+1; i++){
				soln[i] += C*matrix[curBreak][i];
			}
			vector<int> vec (soln, soln + sizeof(soln) / sizeof(int) );
			if(mySet.find(vec) != mySet.end()){
				cout<<"Cycle detected!!\n";
				fprintf(fp1, "%lf %d CYCLE\n", C, numSteps);
				cout<<"Num of steps taken = "<<numSteps<<endl;
				return 0;
			}
			else{
				mySet[vec] = 1;
			}
			for(int i = 0; i<n+1; i++){
				printf("%3d ", soln[i]);
			}
			cout<<endl;
		}
	}

	return 0;



}