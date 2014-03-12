

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <list>
#include <stdio.h>
#include <cstdlib>

using namespace std;

#include"ast.hh"

#include "parser.h"

int main(int argc, char * argv[]) 
{
	string input_file_name = argv[1];
	Parser cfglp_parser(input_file_name);

	if(cfglp_parser.parse()){
		cout<<"Cannot parse the input program"<<endl;
		exit(0);
	}

	program_Ast->print_ast();
	cout<<endl;

	return 0;
}
