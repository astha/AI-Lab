
%filenames="scanner"
%lex-source="scanner.cc"

%%

"&"		{
			store_token_name("AND");
			return Parser::AND; 
		}

"|"		{
			store_token_name("OR  ");
			return Parser::OR; 
		}

"!"		{
			store_token_name("NOT");
			return Parser::NOT; 
		}

[[:alpha:]_][[:alpha:][:digit:]_]* {
					store_token_name("NAME");

					ParserBase::STYPE__ * val = getSval();
					val->string_value = new std::string(matched());

					return Parser::NAME; 
				}


[()]	{
			store_token_name("META CHAR");
			return matched()[0];
		}

"->"	{
			store_token_name("IMPLIES");
			return Parser::IMPLIES; 
		}
