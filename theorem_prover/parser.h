
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

#ifndef Parser_h_included
#define Parser_h_included

#include "ast.hh"
#include "parserbase.h"
#include "scanner.h"

//////////////////////////// Parser Initial Code ///////////////////////////

#include<iostream>
#include<sstream>
#include<string>
#include<vector>
#include<list>
#include<map>

using namespace std;

#include <string.h>


////////////////////////////////////////////////////////////////////////////

#undef Parser
class Parser: public ParserBase
{
	Scanner d_scanner;

    public:
	Parser(string input_file_name)
	{
		d_scanner.switchStreams(input_file_name, "");
		d_scanner.setSval(&d_val__);
	}

        int parse();
        void print();

	int get_line_number();					// Used for errors

    private:
        void error(char const *msg);
        int lex();
        
	void executeAction(int ruleNr);
        void errorRecovery();
        int lookup(bool recovery);
        void nextToken();
        void print__();
};


#endif

