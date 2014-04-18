
/*********************************************************************************************

                                cfglp : A CFG Language Processor
                                --------------------------------

           About:

           Implemented   by  Tanu  Kanvar (tanu@cse.iitb.ac.in) and Uday
           Khedker    (http://www.cse.iitb.ac.in/~uday)  for the courses
           cs302+cs306: Language  Processors  (theory and  lab)  at  IIT
           Bombay.

           Release  date  Jan  15, 2013.  Copyrights  reserved  by  Uday
           Khedker. This  implemenation  has been made  available purely
           for academic purposes without any warranty of any kind.

           Documentation (functionality, manual, and design) and related
           tools are  available at http://www.cse.iitb.ac.in/~uday/cfglp


***********************************************************************************************/

#include<iostream>
#include<fstream>

using namespace std;
#include"ast.hh"
#include <set>


Ast * program_Ast;
set<string> propositions;

Ast::Ast()
{}

Ast::~Ast()
{}

//*********************
Name_Ast::Name_Ast(string & name)
{
  type = 1;
	variable_name = name;
}

Name_Ast::~Name_Ast()
{}


void Name_Ast::print_ast()
{
	cout<<variable_name;
}

void Name_Ast::print_ast_a(FILE* fpOut)
{
  fprintf(fpOut, "%s", variable_name.c_str());
}

int Name_Ast::get_type()
{
  return type;
}

Ast * Name_Ast::get_rhs_ast(){
  return NULL;
}

Ast * Name_Ast::get_lhs_ast(){
  return NULL;
}

bool Name_Ast::check_mp(Ast * reducer){
  return false;
}

bool Name_Ast::same_as(Ast * duplicate){
  if(type == duplicate->get_type()){
    return(variable_name == duplicate->variable_name);
  }
  else{
    return false;
  }
}

string Name_Ast::get_string(){
  return variable_name;
}

int Name_Ast::get_size(){
  return 1;
}



//*********************
implies_Ast::implies_Ast(Ast * a1, Ast * a2)
{
  type = 2;
  lhs = a1;
  rhs = a2;
}

implies_Ast::~implies_Ast()
{
  delete lhs;
  delete rhs;
}


void implies_Ast::print_ast()
{
  cout<<"( ";
  if(lhs != NULL)lhs->print_ast();
  cout<<" -> ";
  if(rhs != NULL)rhs->print_ast();
  cout<<")";
}

void implies_Ast::print_ast_a(FILE* fpOut)
{

  fprintf(fpOut, "(");
  if(lhs != NULL)lhs->print_ast_a(fpOut);
  fprintf(fpOut, " -> ");
  if(rhs != NULL)rhs->print_ast_a(fpOut);
  fprintf(fpOut, ")");
}

int implies_Ast::get_type()
{
  return type;
}

Ast * implies_Ast::get_rhs_ast(){
  return rhs;
}

Ast * implies_Ast::get_lhs_ast(){
  return lhs;
}

bool implies_Ast::check_mp(Ast * reducer){
  if(lhs->same_as(reducer)) return true;
  return false;
}

bool implies_Ast::same_as(Ast * duplicate){
  if(type == duplicate->get_type()){
    if(duplicate->get_lhs_ast() == NULL || duplicate->get_rhs_ast() == NULL){
      return false;
    }
    return(lhs->same_as(duplicate->get_lhs_ast()) && rhs->same_as(duplicate->get_rhs_ast()));
  }
  else{
    return false;
  }
}

string implies_Ast::get_string(){
  string ret_str = "(";
  if(lhs != NULL)ret_str += lhs->get_string();
  ret_str += "->";
  if(rhs != NULL)ret_str += rhs->get_string();
  ret_str += ")";
  return ret_str;
}

int implies_Ast::get_size(){
  int ast_size = 0;
  if(lhs != NULL)ast_size += lhs->get_size();
  if(rhs != NULL)ast_size += rhs->get_size();
  return ast_size;
}