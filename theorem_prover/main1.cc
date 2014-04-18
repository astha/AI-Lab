

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <list>
#include <stdio.h>
#include <cstdlib>
#include <signal.h>
#include <algorithm>
#include <fstream>

using namespace std;

#include"ast.hh"

#include "parser.h"

vector<Ast*> hyp_vec;
vector<pair<int,int>> hyp_vec_pos;
vector<Ast*> hyp_vec_session;
vector<Ast*> hyp_vec_prop;
set<string> suggestion_strings;
set<Ast*> suggestion_asts;

FILE * fpOut;

bool done;

Ast * script_F;

bool bored;

bool check_particular_mp(Ast * a);
bool apply_mod_pon();
bool apply_mod_pon_session();
bool check_already_present(Ast * ast);

bool myCompare(Ast * a1, Ast * a2){
	return (a1->get_size() <= a2->get_size());
}

void insert_suggestions(Ast * ast){
	if(ast == NULL) return;
	if(suggestion_strings.find(ast->get_string()) == suggestion_strings.end()){
		suggestion_strings.insert(ast->get_string());
		suggestion_asts.insert(ast);
	}

	if(ast->get_type() == 1){
		return;
	}
	else{
		insert_suggestions(ast->get_lhs_ast());
		insert_suggestions(ast->get_rhs_ast());
	}
}

void create_propositions_asts(){
	hyp_vec_prop.clear();
	for(int i = 0 ;i<hyp_vec_session.size(); i++){
		insert_suggestions(hyp_vec_session[i]);
	}
	// copy the set in vector
	set<Ast*>::iterator ast_it;
	for(ast_it = suggestion_asts.begin(); ast_it != suggestion_asts.end(); ast_it++)
		hyp_vec_prop.push_back(*ast_it);

	if(suggestion_strings.find("F") == suggestion_strings.end())
		hyp_vec_prop.push_back(script_F);

	// sort the vector in order of size
	sort(hyp_vec_prop.begin(), hyp_vec_prop.end(), myCompare);

}

bool generate_suggestions() {
	// type 1
	// a->(b->a)
	suggestion_asts.clear();
	suggestion_strings.clear();
	hyp_vec_pos.clear();
	create_propositions_asts();

	cout<<"\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n";
	for(int i =0 ; i<hyp_vec_prop.size(); i++){
		hyp_vec_prop[i]->print_ast();
		cout<<endl;
	}
	cout<<"\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n";

	Ast * suggestion;
	vector<Ast*> hyp_vec_temp = hyp_vec_prop;
	// hyp_vec_temp.insert(hyp_vec_temp.end(), hyp_vec.begin(), hyp_vec.end());

	int vec_size = hyp_vec_temp.size();
	int count = 0;
	for(int i =0 ;i<vec_size; i++){
		for(int j =0 ;j<vec_size && j != i; j++){
			suggestion = new implies_Ast(hyp_vec_temp[i], new implies_Ast(hyp_vec_temp[j], hyp_vec_temp[i]));
			// suggestion->print_ast();
			// cout<<endl;
			// count++;
			if(check_already_present(suggestion)) continue;
			if(check_particular_mp(suggestion)){
				apply_mod_pon_session();
				if(done) return true;
				// return true;
			}
			else if(bored){
				return false;
			}
		}
	}

	// type 2
	// (a->(b->c))->((a->b)->(a->c))
	// cout<<vec_size<<endl;
	for(int i =0 ;i<vec_size; i++){
		for(int j =0 ;j<vec_size; j++){
			for(int k =0 ;k<vec_size; k++){

				if(!(!hyp_vec_temp[i]->same_as(hyp_vec_temp[j]) && !hyp_vec_temp[i]->same_as(hyp_vec_temp[k]) && !hyp_vec_temp[j]->same_as(hyp_vec_temp[k])))continue;
				suggestion = new implies_Ast(new implies_Ast(hyp_vec_temp[i], new implies_Ast(hyp_vec_temp[j], hyp_vec_temp[k])), new implies_Ast(new implies_Ast(hyp_vec_temp[i], hyp_vec_temp[j]),new implies_Ast(hyp_vec_temp[i], hyp_vec_temp[k])));
				if(check_already_present(suggestion)) continue;
				if(check_particular_mp(suggestion)){
					apply_mod_pon_session();
					if(done) return true;
				}
				else if(bored){
					return false;
				}
			}
		}
	}

	// type 3
	// ((a->F)->F)->a
	for(int i =0 ;i<vec_size; i++){
		suggestion = new implies_Ast(new implies_Ast(new implies_Ast(hyp_vec_temp[i],script_F), script_F), hyp_vec_temp[i]);
		// suggestion->print_ast();
		// 	cout<<endl;
		// 	count++;
		if(check_already_present(suggestion)) continue;
		if(check_particular_mp(suggestion)){
			apply_mod_pon_session();
			if(done) return true;
		}
		else if(bored){
			return false;
		}
	}

	// cout<<count<<endl;
	// exit(0);

	

	return false;
}

bool check_particular_mp(Ast * a){
	char ch;
	for(int j =0 ;j<hyp_vec_session.size(); j++){
		if(hyp_vec_session[j]->get_type() == 2 && hyp_vec_session[j]->check_mp(a)){
			// cout<<"SUGGESTION:: ";
			// cout<<"Modus ponens can be applied on -->>  ";
			// a->print_ast();
			// cout<<" and ";
			// hyp_vec_session[j]->print_ast();
			// cout<<"\n";
			// cout<<"Should I use it (y/n/b): "<<endl;
			// cin>>ch;
			// if(ch == 'y'){
			hyp_vec_session.push_back(a);
			return true;
			// }
			// if(ch == 'b'){
			// 	bored = true;
			// 	return false;
			// }
		}
	}

	for(int j =0 ;j<hyp_vec_session.size(); j++){
		if(hyp_vec_session[j]->get_type() == 2 && a->check_mp(hyp_vec_session[j])){
			hyp_vec_session.push_back(a);
			return true;
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
	cout<<"\n Current hypothesis set\n";
	for(int i = 0; i<hyp_vec.size(); i++){
		hyp_vec[i]->print_ast();
		cout<<endl;
	}
}

bool check_already_present(Ast * ast){
	for(int i = 0; i<hyp_vec_session.size(); i++){
		if(hyp_vec_session[i]->same_as(ast)) return true;
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
					// cout<<"MESSAGE:: Modus ponens applied on -->>  ";
					mp_lhs->print_ast_a(fpOut);
					fprintf(fpOut, ":");
					hyp_vec[j]->print_ast_a(fpOut);
					fprintf(fpOut, "\n");
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

bool apply_mod_pon_session(){
	int hyp_size = hyp_vec_session.size();
	Ast * mp_lhs;
	for(int i =0 ;i<hyp_size; i++){
		mp_lhs = hyp_vec_session[i];
		for(int j =0 ;j<hyp_size; j++){
			if(hyp_vec_session[j]->get_type() == 2){
				if(hyp_vec_session[j]->check_mp(mp_lhs) && !check_already_present(hyp_vec_session[j]->get_rhs_ast())){
					// cout<<"MESSAGE:: Modus ponens applied on -->>  ";
					mp_lhs->print_ast_a(fpOut);
					// cout<<":";
					fprintf(fpOut, ":");
					hyp_vec_session[j]->print_ast_a(fpOut);
					fprintf(fpOut, "\n");
					if((hyp_vec_session[j]->get_rhs_ast())->same_as(script_F)){
						done = true;
						return true;
					}
					hyp_vec_session.push_back(hyp_vec_session[j]->get_rhs_ast());
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

	fpOut = fopen("mp.txt", "a+");

	string fal = "F";
	script_F = new Name_Ast(fal);
	create_propositions_asts();
	cout<<endl;
	cout<<"TO PROVE -->>  ";
	program_Ast->print_ast();
	cout<<endl<<endl;

	Ast * temp_ast = program_Ast;

	
	// create hyp vector on the lhs
	while(1){
		if(temp_ast->get_type() == 1){
			// Name ast, done
			if(!temp_ast->same_as(script_F))
				hyp_vec.push_back(new implies_Ast(temp_ast, new Name_Ast(fal)));
			break;
		}
		else{
			hyp_vec.push_back(temp_ast->get_lhs_ast());
			temp_ast = temp_ast->get_rhs_ast();
		}
	}


	done = false;

	
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
			fclose(fpOut);
			return 0;

		}
		else{
			// take help
 			while(1){
 				hyp_vec_session = hyp_vec;
 				bored = false;
 				if(generate_suggestions()){
 					break;
 				}
 				bored = false;
 				if(generate_suggestions()){
 					break;
 				}
 				bored = false;
 				if(generate_suggestions()){
 					break;
 				}
				cout<<"Help please ::\n";
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
			if(done) continue;
			hyp_vec.push_back(program_Ast);
			cout<<"Hypothesis added successfully!! Party!!!!!!"<<endl;
		}
	}
	return 0;
}
