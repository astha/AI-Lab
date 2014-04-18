%scanner ../scanner.h
%scanner-token-function d_scanner.lex()
%filenames parser
%parsefun-source parser.cc

%union 
{
	std::string * string_value;
	Ast * ast;
};

%token <string_value> NAME
%token IMPLIES
%token AND
%token OR
%token NOT
%type<ast> variable
%type<ast> simple_formula
%type<ast> formula


%start formula
	
%%

formula:
	simple_formula {
		$$ = $1;
		program_Ast = $$;
	}
|
	formula IMPLIES simple_formula {
		$$ = new implies_Ast($1,$3);
		program_Ast = $$;
	}
|
	formula AND simple_formula {
		string fal = "F";
		$$ = new implies_Ast(new implies_Ast($1,new implies_Ast($3,new Name_Ast(fal))), new Name_Ast(fal));
		program_Ast = $$;
	}
|
	formula OR simple_formula {
		string fal = "F";
		$$ = new implies_Ast(new implies_Ast($1,new Name_Ast(fal)),$3);
		program_Ast = $$;
	}
	
;

simple_formula:
	variable {
		$$ = $1;
	}
|
	'(' formula ')' {
		$$ = $2;
	}
|
	NOT simple_formula {
		string fal = "F";
		$$ = new implies_Ast($2,new Name_Ast(fal));
	}
;

variable:
	NAME
	{
		$$ = new Name_Ast(*$1);
		//propositions.insert(*$1);
		delete $1;
	}
;
