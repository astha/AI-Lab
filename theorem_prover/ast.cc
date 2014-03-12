
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


Ast * program_Ast;

Ast::Ast()
{}

Ast::~Ast()
{}

//*********************
Name_Ast::Name_Ast(string & name)
{
	variable_name = name;
}

Name_Ast::~Name_Ast()
{}


void Name_Ast::print_ast()
{
	cout<<variable_name;
}

//*********************
implies_Ast::implies_Ast(Ast * a1, Ast * a2)
{
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


// Eval_Result & Name_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
// {
// 	return ;
// }