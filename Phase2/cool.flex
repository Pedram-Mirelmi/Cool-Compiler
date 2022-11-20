	/*
 *  The scanner definition for COOL.
 */

/*
 *  Stuff enclosed in %{ %} in the first section is copied verbatim to the
 *  output, so headers and global definitions are placed here to be visible
 * to the code in the file.  Don't remove anything that was here initially
 */
%{
#include <cool-parse.h>
#include <stringtab.h>
#include <utilities.h>

/* The compiler assumes these identifiers. */
#define yylval cool_yylval
#define yylex  cool_yylex

/* Max size of string constants */
#define MAX_STR_CONST 1025
#define YY_NO_UNPUT   /* keep g++ happy */

extern FILE *fin; /* we read from this file */

/* define YY_INPUT so we read from the FILE fin:
 * This change makes it possible to use this scanner in
 * the Cool compiler.
 */
#undef YY_INPUT
#define YY_INPUT(buf,result,max_size) \
	if ( (result = fread( (char*)buf, sizeof(char), max_size, fin)) < 0) \
		YY_FATAL_ERROR( "read() in flex scanner failed");

char string_buf[MAX_STR_CONST]; /* to assemble string constants */
char *string_buf_ptr;

extern int curr_lineno;
extern int verbose_flag;

extern YYSTYPE cool_yylval;


/*
 *  Add Your own definitions here
 */
#include <iostream>
#include <sstream>
		std::stringstream temp_str_buff;
using namespace std;

%}

/*
 * Define names for regular expressions here.
 */
%x IN_STRING
%x MULTI_LINE_COMMENT
%x SINGLE_LINE_COMMENT


DIGIT					[0-9]
ALPHA					[A-Za-z_]
CONST_INT				{DIGIT}+

TYPEID 					[A-Z]({ALPHA}|{DIGIT})*
OBJECTID 				[a-z]({ALPHA}|{DIGIT})*

WHITE_SPACE				[ \n\f\r\t\v]

 /*
  * Keywords
  */
CLASS					(?i:class)
ELSE 					(?i:else)
IF						(?i:if)
FI						(?i:fi)
IN						(?i:in)
INHERITS				(?i:inherits)
ISVOID 					(?i:isvoid)
LET						(?:let)
LOOP 					(?i:loop)
POOL 					(?i:pool)
THEN 					(?i:then)
WHILE 					(?i:while)
CASE 					(?i:case)
ESAC 					(?i:esac)
NEW 					(?i:new)
OF 						(?i:of)
NOT 					(?i:not)


FALSE 					f(?i:alse)
TRUE  					t(?i:rue)


ASSIGN					"<-"

	
MUL_COMMENT_START		"(*"
MUL_COMMENT_END			"*)"
DOUBLE_DASH				"--"
DOUBLE_QUOTE			"\""
BACKSLASH				"\\"




%%

 /*
  *  Nested comments
  */


 /*
  *  The multiple-character operators.
  */

<INITIAL>{DOUBLE_DASH} {
	// std::cout << "in single comment" << std::endl;
	BEGIN(SINGLE_LINE_COMMENT);
}

<SINGLE_LINE_COMMENT>. {
	// DO NOTHING HERE
}

<SINGLE_LINE_COMMENT>\n {
	curr_lineno++;
	// std::cout << "exiting single comment" << std::endl;
	BEGIN(INITIAL);
}


<SINGLE_LINE_COMMENT><<EOF>> {
	// std::cout << "exiting single comment" << std::endl;
	BEGIN(INITIAL);
}

<INITIAL>{MUL_COMMENT_END} {
	yylval.error_msg = "Unmatched *)";
	return ERROR;
}

<INITIAL>{MUL_COMMENT_START} {
	// std::cout << "in multi comment" << std::endl;
	BEGIN(MULTI_LINE_COMMENT);
}


<MULTI_LINE_COMMENT><<EOF>> {
	BEGIN(INITIAL); // RESUME LEXING ...
	// std::cout << "exit multi comment" << std::endl;
	yylval.error_msg = "EOF in comment";
	return ERROR;
}

<MULTI_LINE_COMMENT>{MUL_COMMENT_END} {
	// std::cout << "exit multi comment" << curr_lineno<< std::endl;
	BEGIN(INITIAL);
}


<MULTI_LINE_COMMENT>(.|\n) {
	 if(yytext[0] == '\n')
	 	curr_lineno++;
	/* DO NOTHING */
}



<INITIAL>{DOUBLE_QUOTE} {
	// std::cout << "in string" << std::endl;
	BEGIN(IN_STRING);	
}

<IN_STRING><<EOF>> {
	yylval.error_msg = "EOF in string constant";
	temp_str_buff.str(std::string());
	return ERROR;
}

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
		case '\n':
			temp_str_buff << '\n';
			curr_lineno++;
			break;
		default:
			temp_str_buff << yytext[1];
			break;
	}
}

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


 /*
  *		Keywords
  */
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





 /*
  *	Identifiers
  */
<INITIAL>{TYPEID} {
	yylval.symbol = idtable.add_string(yytext);
	return TYPEID;
}
 
<INITIAL>{OBJECTID} {
	yylval.symbol = idtable.add_string(yytext);
	return OBJECTID;
}

 /*
  *		Assignment
  */
<INITIAL>{ASSIGN}       { return ASSIGN; }



 /*
  *		Compare operators
 */
<INITIAL>"<="        { return LE; }
<INITIAL>"<"         { return (int)'<'; }
<INITIAL>"="         { return (int)'='; }



 /* 
  *		Single characters
  */
<INITIAL>"{"			{ return (int)'{'; }
<INITIAL>"}"			{ return (int)'}'; }
<INITIAL>"("			{ return (int)'('; }
<INITIAL>")"			{ return (int)')'; }
<INITIAL>";"			{ return (int)';'; }
<INITIAL>","         { return (int)','; }


 /*
  *		Arithmatics
  */
<INITIAL>"+"         { return (int)'+'; }
<INITIAL>"-"         { return (int)'-'; }
<INITIAL>"*"         { return (int)'*'; }
<INITIAL>"/"         { return (int)'/'; }

 /*
  *		Other
  */

<INITIAL>"@"         { return (int)'@'; }
 
<INITIAL>"~"         { return (int)'~'; }

<INITIAL>":"         { return (int)':'; }

<INITIAL>"\."        { return (int)'.'; }


<INITIAL>{WHITE_SPACE} {
	if(yytext[0] == '\n' || yytext[0] == '\f')
		curr_lineno++;
}

 /*
  *	Unknown or unmatched character
  */
<INITIAL>. {
	yylval.error_msg = yytext;
	return ERROR;	
}
 /*
  * Keywords are case-insensitive except for the values true and false,
  * which must begin with a lower-case letter.
  */


 /*
  *  String constants (C syntax)
  *  Escape sequence \c is accepted for all characters c. Except for 
  *  \n \t \b \f, the result is c.
  *
  */


%%

