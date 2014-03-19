
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
  cout<<") ";
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
    return(lhs->same_as(duplicate->get_lhs_ast()) && rhs->same_as(duplicate->get_rhs_ast()));
  }
  else{
    return false;
  }
}

// Eval_Result & Name_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
// {
// 	return ;
// }