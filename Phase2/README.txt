We're about to write a lexer for cool language and we're using a tool named "flex"! 
flex is a tool that helps us define a DFA that runs over the code and do some actions defined by us for us.
The whole procces is devided into two compilation parts. first we write some code in flex format. In the next step flex compile that and build a DFA in C++. Then we can compile that simply using g++ command and use it to lex our "cool" code.


PAY ATTENTION!
That the main function, tables to store the constant value and identifiers and other stuff are already coded for us in separate files. 
main function -> lextest.cc
Entries and their tables headers -> stringtab.h , stringtab_functions.h
Entries and their tables definitions -> stringtab.cc
etc ...

we just needed to modify the cool.flex file!

Each flex code has the following format:

%{

	// This is the first part where everything we write here will be copied and pasted into the C++ file. 
	// This is the part that we can #include our other codes and definitions.
	// example:
	 
	#include <iostream>

}%

// right after that, here comes the flex definitions. First we can define some variables for some frequently used regular expressions. seocnd we can define some "states" for our DFA
// take note that the default state is called "INITIAL"
// example:
	DIGIT					[0-9] 		// regular expression
	
	
	%x IN_STRING			// state
	

%%

// after the definitions part here comes the main part where we define some pairs of 'RE-action's that tells the flex that whenever see this 'RE' do this 'action'
// example:
	{DIGIT}		{ std::cout << "some digit observed!" << std::endl; } 

// The important thing here is that the rules have priority from top to bottom.

%%

in this section we can also bring the main function or ...

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


so that was the format of a flex file.
now let's take a look at our file. 
in the first part we included the header "sstream" it will help us build the string constants using the std::stringstream because it's easier than C style (char*) string
so we declare and defien a global variable from this type:

std::stringstream temp_str_buff;

####################################################################

in the definitions part first we defined 3 states:

%x IN_STRING
%x MULTI_LINE_COMMENT
%x SINGLE_LINE_COMMENT

as you can see they names has been chosen reasonably.

Next you can see the names for the commonly used REs.

#############################################################################
in the "rules" section

due to the priority of the rules first we try to catch the errors:

// first we check for single line comment. if so, we enter the SINGLE_LINE_COMMENT state.
<INITIAL>{DOUBLE_DASH} {
	// std::cout << "in single comment" << std::endl;
	BEGIN(SINGLE_LINE_COMMENT);
}

// while we're in that state we skip whatever we read (exept for newline):
<SINGLE_LINE_COMMENT>. {
	// DO NOTHING HERE
}

// when we reach the new line or EOF we exit that state and enter the INITIAL state which is the default:
<SINGLE_LINE_COMMENT>\n {
	// std::cout << "exiting single comment" << std::endl;
	BEGIN(INITIAL);
}


<SINGLE_LINE_COMMENT><<EOF>> {
	// std::cout << "exiting single comment" << std::endl;
	BEGIN(INITIAL);
}

// next we try to catch the "unmatched *)" error:
<INITIAL>{MUL_COMMENT_END} {
	yylval.error_msg = "Unmatched *)";
	return ERROR;
}


// if not we check the multi line comment and if so, we go to the corresponding state:
<INITIAL>{MUL_COMMENT_START} {
	// std::cout << "in multi comment" << std::endl;
	BEGIN(MULTI_LINE_COMMENT);
}

// we check the EOF error in this state:
<MULTI_LINE_COMMENT><<EOF>> {
	BEGIN(INITIAL); // RESUME LEXING ...
	// std::cout << "exit multi comment" << std::endl;
	yylval.error_msg = "EOF in comment";
	return ERROR;
}

// because *) contains two letter we check it befor any letter commented and if so, we go back to INITIAL:
<MULTI_LINE_COMMENT>{MUL_COMMENT_END} {
	// std::cout << "exit multi comment" << curr_lineno<< std::endl;
	BEGIN(INITIAL);
}

// now we can simply skip each letter in the comment (just increment the curr_lineno):
<MULTI_LINE_COMMENT>(.|\n) {
	 if(yytext[0] == '\n')
	 	curr_lineno++;
	/* DO NOTHING */
}


// now we can go for strings:
<INITIAL>{DOUBLE_QUOTE} {
	// std::cout << "in string" << std::endl;
	BEGIN(IN_STRING);	
}

// catch the EOF error:
<IN_STRING><<EOF>> {
	yylval.error_msg = "EOF in string constant";
	temp_str_buff.str(std::string());
	return ERROR;
}

// now we have some exeptions for \c in strings to make everything simple we go just as said in the "cool manual":
// so we match '\c' and check cases for 'c' using "switch statements"

<IN_STRING>{BACKSLASH}(.|\n) { // exeptions are: \b \t \n \f
	switch(yytext[1])
	{
		case 'b':
			temp_str_buff << '\b';
			break;
		case 't':
			temp_str_buff << '\t';
			break;
		case 'n':
			temp_str_buff << '\n';
			break;
		case 'f':
			temp_str_buff << '\f';
			break;
		case '\n':					// scaped new line in string
			temp_str_buff << '\n';
			curr_lineno++;
			break;
		default:					// unnecessary scaped character
			temp_str_buff << yytext[1];
			break;
	}
}

// now whenever we read anything except for " we add first we check for non-escaped new-line of null-character then we add it to our "std::stringstream temp_str_buff"
<IN_STRING>[^\"] {
	if (yytext[0] == '\n')
	{
		BEGIN(INITIAL);
		yylval.error_msg = "Unterminated string constant";
		curr_lineno++;
		temp_str_buff.str(std::string());
		return ERROR;
	}
	if (yytext[0] == 0)
	{
		BEGIN(INITIAL);
		yylval.error_msg = "String contains null character";
		temp_str_buff.str(std::string());
		return ERROR;
	}
	temp_str_buff << yytext[0];
}

// when we reached the end of the string we check store the whole string in "str_const" variable
// empty the std::stringstream temp_str_buff.
// check the length for errors ... 
// and finally add the string const to the corresponding table.
<IN_STRING>{DOUBLE_QUOTE} {
	// std::cout << "exiting string" << std::endl;
	BEGIN(INITIAL);
	std::string str_const = temp_str_buff.str();
	temp_str_buff.str(std::string());
	// std::cout << str_const << std::endl;
	if(str_const.size() > MAX_STR_CONST)
	{
		BEGIN(INITIAL);
		yylval.error_msg = "String constant too long";
		temp_str_buff.str(std::string());
		return ERROR;
	}
	yylval.symbol = stringtable.add_string((char*)str_const.c_str());
	// strncpy(string_buf, str_const.c_str(), str_const.size());
	// std::cout << yylval.symbol << std::endl;
	return STR_CONST;
}

// now we match the constants integers and boolean values
<INITIAL>{CONST_INT} {
	yylval.symbol = inttable.add_string(yytext);
	return INT_CONST;
}

<INITIAL>{FALSE} {
	yylval.boolean = false;
	return BOOL_CONST;
}

<INITIAL>{TRUE} {
	yylval.boolean = true;
	return BOOL_CONST;
}


// then keywords ...:
<INITIAL>{CLASS}     { return CLASS; }
<INITIAL>{ELSE}      { return ELSE; }
<INITIAL>{FI}        { return FI; }
<INITIAL>{IF}        { return IF; }
<INITIAL>{IN}        { return IN; }
<INITIAL>{INHERITS}  { return INHERITS; }    
<INITIAL>{LET}       { return LET; } 
<INITIAL>{LOOP}      { return LOOP; }    
<INITIAL>{POOL}      { return POOL; }
<INITIAL>{THEN}      { return THEN; }
<INITIAL>{WHILE}     { return WHILE; }
<INITIAL>{CASE}      { return CASE; }
<INITIAL>{ESAC}		 { return ESAC; }
<INITIAL>{NEW}       { return NEW; }
<INITIAL>{OF}        { return OF; }
<INITIAL>{NOT}       { return NOT; }
<INITIAL>{ISVOID}	 { return ISVOID; }


// then identifiers:
<INITIAL>{TYPEID} {
	yylval.symbol = idtable.add_string(yytext);
	return TYPEID;
}
 
<INITIAL>{OBJECTID} {
	yylval.symbol = idtable.add_string(yytext);
	return OBJECTID;
}

// and others ...

<INITIAL>{ASSIGN}       { return ASSIGN; }

<INITIAL>"<="        { return LE; }
<INITIAL>"<"         { return (int)'<'; }
<INITIAL>"="         { return (int)'='; }

<INITIAL>"{"			{ return (int)'{'; }
<INITIAL>"}"			{ return (int)'}'; }
<INITIAL>"("			{ return (int)'('; }
<INITIAL>")"			{ return (int)')'; }
<INITIAL>";"			{ return (int)';'; }
<INITIAL>","         { return (int)','; }


<INITIAL>"+"         { return (int)'+'; }
<INITIAL>"-"         { return (int)'-'; }
<INITIAL>"*"         { return (int)'*'; }
<INITIAL>"/"         { return (int)'/'; }
<INITIAL>"@"         { return (int)'@'; }
 
<INITIAL>"~"         { return (int)'~'; }

<INITIAL>":"         { return (int)':'; }

<INITIAL>"\."        { return (int)'.'; }



// here we just skip the white spaces and just check for new lines to increment the curr_lineno
<INITIAL>{WHITE_SPACE} {
	if(yytext[0] == '\n' || yytext[0] == '\f')
		curr_lineno++;
}

// and this rule is for the any unmatched character
<INITIAL>. {
	yylval.error_msg = yytext;
	return ERROR;	
}


