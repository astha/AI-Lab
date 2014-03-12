
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

#ifndef AST_HH
#define AST_HH

#include<string>

#define AST_SPACE "         "
#define AST_NODE_SPACE "            "

using namespace std;

class Ast;

class Ast
{
protected:
public:
	Ast();
	~Ast();

	virtual void print_ast() = 0;
	// virtual Eval_Result & evaluate() = 0;
};

extern Ast * program_Ast;


class Name_Ast:public Ast
{
	string variable_name;

public:
	Name_Ast(string & name);
	~Name_Ast();

	void print_ast();
	// Eval_Result & evaluate();
};

class implies_Ast:public Ast
{
  Ast * lhs;
  Ast * rhs;

public:
  implies_Ast(Ast * a1, Ast * a2);
  ~implies_Ast();

  void print_ast();
  // Eval_Result & evaluate();
};

#endif
