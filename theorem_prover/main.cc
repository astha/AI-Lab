

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <list>
#include <stdio.h>
#include <cstdlib>
#include <signal.h>

using namespace std;

#include"ast.hh"

#include "parser.h"

vector<Ast*> hyp_vec;

bool done;

Ast * script_F;

bool bored;

bool check_particular_mp(Ast * a);
bool apply_mod_pon();
bool check_already_present(Ast * ast);

bool generate_suggestions() {
	// type 1
	// a->(b->a)
	Ast * suggestion;
	int vec_size = hyp_vec.size();
	for(int i =0 ;i<vec_size; i++){
		for(int j =0 ;j<vec_size && j != i; j++){
			suggestion = new implies_Ast(hyp_vec[i], new implies_Ast(hyp_vec[j], hyp_vec[i]));
			if(check_already_present(suggestion)) continue;
			if(check_particular_mp(suggestion)){
				apply_mod_pon();
				return true;
			}
			else if(bored){
				return false;
			}
		}
	}

	// type 3
	// ((a->F)->F)->F
	for(int i =0 ;i<vec_size; i++){
		suggestion = new implies_Ast(new implies_Ast(new implies_Ast(hyp_vec[i],script_F), script_F), script_F);
		if(check_already_present(suggestion)) continue;
		if(check_particular_mp(suggestion)){
			apply_mod_pon();
			return true;
		}
		else if(bored){
			return false;
		}
	}

	// type 2
	// (a->(b->c))->((a->b)->(a->c))
	for(int i =0 ;i<vec_size; i++){
		for(int j =0 ;j<vec_size; j++){
			for(int k =0 ;k<vec_size; k++){
				suggestion = new implies_Ast(new implies_Ast(hyp_vec[i], new implies_Ast(hyp_vec[j], hyp_vec[k])), new implies_Ast(new implies_Ast(hyp_vec[i], hyp_vec[j]),new implies_Ast(hyp_vec[i], hyp_vec[k])));
				if(check_already_present(suggestion)) continue;
				if(check_particular_mp(suggestion)){
					apply_mod_pon();
					return true;
				}
				else if(bored){
					return false;
				}
			}
		}
	}
}

bool check_particular_mp(Ast * a){
	char ch;
	for(int j =0 ;j<hyp_vec.size(); j++){
		if(hyp_vec[j]->get_type() == 2 && hyp_vec[j]->check_mp(a)){
			cout<<"Suggestion:: ";
			cout<<"MODUS PONENS APPLIED ON -->>  ";
			a->print_ast();
			cout<<" and ";
			hyp_vec[j]->print_ast();
			cout<<"\n";
			cout<<"Should I use it (y/n/b): "<<endl;
			cin>>ch;
			if(ch == 'y'){
				hyp_vec.push_back(a);
				return true;
			}
			if(ch == 'b'){
				bored = true;
				return false;
			}
		}
	}

	for(int j =0 ;j<hyp_vec.size(); j++){
		if(hyp_vec[j]->get_type() == 2 && a->check_mp(hyp_vec[j])){
			cout<<"Suggestion:: ";
			cout<<"MODUS PONENS APPLIED ON -->>  ";
			hyp_vec[j]->print_ast();
			cout<<" and ";
			a->print_ast();
			cout<<"\n";
			cout<<"Should I use it (y/n/b): "<<endl;
			cin>>ch;
			if(ch == 'y'){
				hyp_vec.push_back(a);
				return true;
			}
			if(ch == 'b'){
				bored = true;
				return false;
			}
		}
	}
	return false;
}

void print_current_hypo(){
	for(int i = 0; i<hyp_vec.size(); i++){
		hyp_vec[i]->print_ast();
		cout<<endl;
	}
}

void print_current_hypo_sig(int signum){
	signal(SIGINT, print_current_hypo_sig);
	cout<<"\n current hypothesis set\n";
	for(int i = 0; i<hyp_vec.size(); i++){
		hyp_vec[i]->print_ast();
		cout<<endl;
	}
}

bool check_already_present(Ast * ast){
	for(int i = 0; i<hyp_vec.size(); i++){
		if(hyp_vec[i]->same_as(ast)) return true;
	}
	return false;
}

bool apply_mod_pon(){
	int hyp_size = hyp_vec.size();
	Ast * mp_lhs;
	for(int i =0 ;i<hyp_size; i++){
		mp_lhs = hyp_vec[i];
		for(int j =0 ;j<hyp_size; j++){
			if(hyp_vec[j]->get_type() == 2){
				if(hyp_vec[j]->check_mp(mp_lhs) && !check_already_present(hyp_vec[j]->get_rhs_ast())){
					cout<<"MODUS PONENS APPLIED ON -->>  ";
					mp_lhs->print_ast();
					cout<<" and ";
					hyp_vec[j]->print_ast();
					cout<<"\n";
					if((hyp_vec[j]->get_rhs_ast())->same_as(script_F)){
						done = true;
						return true;
					}
					hyp_vec.push_back(hyp_vec[j]->get_rhs_ast());
					return true;
				}
			}
		}	
	}
	return false;
}

int main(int argc, char * argv[]) 
{
	signal(SIGINT, print_current_hypo_sig);
	string input_file_name = argv[1];
	Parser cfglp_parser(input_file_name);

	if(cfglp_parser.parse()){
		cout<<"Cannot parse the input program"<<endl;
		exit(0);
	}
	cout<<endl;
	cout<<"TO PROVE -->>  ";
	program_Ast->print_ast();
	cout<<endl<<endl;

	Ast * temp_ast = program_Ast;

	string fal = "F";
	// create hyp vector on the lhs
	while(1){
		if(temp_ast->get_type() == 1){
			// Name ast, done
			
			hyp_vec.push_back(new implies_Ast(temp_ast, new Name_Ast(fal)));
			break;
		}
		else{
			hyp_vec.push_back(temp_ast->get_lhs_ast());
			temp_ast = temp_ast->get_rhs_ast();
		}
	}

	done = false;

	script_F = new Name_Ast(fal);
	string user_help_hyp;

	FILE * fp; 

	cout<<"*****************************************************************************\n\n";
	cout<<"INITIAL HYPOTHESES SET\n";
	print_current_hypo();
	cout<<"\n*****************************************************************************\n";
	

	while(1){
		while(apply_mod_pon() && !done){
			apply_mod_pon();	
		}

		if(done){
			cout<<"*****************************************************************************\n\n";
			cout<<"FINAL HYPOTHESES SET\n";
			print_current_hypo();
			cout<<"\n*****************************************************************************\n";
			cout<<"ITS DONE, ITS OVER"<<endl;
			return 0;
		}
		else{
			// take help
 			while(1){
 				bored = false;
 				if(generate_suggestions()){
 					continue;
 				}
				cout<<"MySelF dUmBo,NaaM To SuNa Hi HogA, HeLp PliZ::\n";
				cin>>user_help_hyp;
				fp = fopen("new_hyp", "w+");
				fprintf(fp, "%s\n", user_help_hyp.c_str());
				fclose(fp);
				Parser cfglp_parser_temp("new_hyp");
				if(cfglp_parser_temp.parse()){
					cout<<"incorrect hypothesis syntax!! Please re-enter.."<<endl;
				}
				else{
					break;
				}
			}
			hyp_vec.push_back(program_Ast);
			cout<<"Hypothesis added successfully!! Party!!!!!!"<<endl;
		}
	}
	return 0;
}
