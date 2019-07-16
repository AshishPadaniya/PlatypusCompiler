/************************************************************************
File Name :		scanner.c
Compiler :		MS Visual Studio 2015
Author :		Ashish Padaniya 040852591
Course :		CST 8152 – Compilers
Lab Section:    011
Assignment :	2
Date :			07-03-2018
Professor :		Sv. Ranev
Purpose :       Create a lexical analyzer for this language to implements all Functions and test all cases 
			    read stearm of chracters loaded into a input buffer
				and produces a stream of token representations and their attributes.
Function List : scanner_init(), malar_next_token(),
				char_class(),get_next_state(), iskeyword(), aa_func02(), aa_func03(), aa_func05(),
				aa_func08(), aa_func10(), aa_func11(), atolh()
**************************************************************************/

/* The #define _CRT_SECURE_NO_WARNINGS should be used in MS Visual Studio projects
 * to suppress the warnings about using "unsafe" functions like fopen()
 * and standard sting library functions defined in string.h.
 * The define does not have any effect in Borland compiler projects.
 */
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>   /* standard input / output */
#include <ctype.h>   /* conversion functions */
#include <stdlib.h>  /* standard library functions and constants */
#include <string.h>  /* string functions */
#include <limits.h>  /* integer types constants */
#include <float.h>   /* floating-point types constants */

/*#define NDEBUG        to suppress assert() call */
#include <assert.h>  /* assert() prototype */

/* project header files */
#include "buffer.h"
#include "token.h"
#include "table.h"

#define DEBUG  /* for conditional processing */
#undef  DEBUG

/* Global objects - variables */
/* This buffer is used as a repository for string literals.
   It is defined in platy_st.c */
extern Buffer * str_LTBL; /*String literal table */
int line; /* current line number of the source code */
extern int scerrnum;     /* defined in platy_st.c - run-time error number */

/* Local(file) global objects - variables */
static Buffer *lex_buf;/*pointer to temporary lexeme buffer*/

/* No other global variable declarations/definitiond are allowed */

/* scanner.c static(local) function  prototypes */ 
static int char_class(char c); /* character class function */
static int get_next_state(int, char, int *); /* state machine function */
static int iskeyword(char * kw_lexeme); /*keywords lookup functuion */
static long atolh(char * lexeme); /* converts hexadecimal string to decimal value */

/*Initializes scanner */
/********************************************************************************************************
Purpose :						  The following function is use to intialize the scanner
Author	:						  Sv.Ranev
History :						  07-03-2018
Called functions :  b_isempty() : checks if buffer is empty
					b_rewind()  : will work if buffer has been read before
					b_clear()   : to clear the buffer
Paramters :
					sc_buffer :   Passing the pointer to the buffer
Return Value :					  return value is EXIT_SUCESS or 0
Algorithm :						  returns EXIT_FAILURE when buffer is not empty and if it has been read before call rewind and after
								  that clear buffer to reset everything.
							
********************************************************************************************************/
int scanner_init(Buffer * sc_buf) {
  	if(b_isempty(sc_buf)) return EXIT_FAILURE;/*1*/
	/* in case the buffer has been read previously  */
	b_rewind(sc_buf);
	b_clear(str_LTBL);
	line = 1;
	return EXIT_SUCCESS;/*0*/
/*   scerrnum = 0;  *//*no need - global ANSI C */
}
/********************************************************************************************************
Purpose :			  The purpose of this function is to recognize the tokens and return them along with errors.
Author :			  Ashish Padaniya
History :			  07-03-2018

Called functions :    b_getc() : to get the chracter from the buffer
					  b_retract() : to retract the chracter from buffer
					  b_mark(): to set the mark in the buffer
					  b_getcoffset():to get the value of getcoffset
					  b_limit():To get addcoffset
					  b_addc():To add the character in the buffer
					  b_reset():Helps to set the value of getcoffset to the the current value of markcoffset.
					  get_next_state():To get the states from transition table
					  isalpha(): To check if chracter is alphabet or not
					  isdigit(): To check if chracter is digit or not
					  isspace(): To check if there is any space or tab between chracters
					  b_allocate():To allocattion of memory for the new buffer
					  b_free():To free the memory from the buffer
					  b_location():To get the pointer to the location of character
					  aa_func12(): It will returns errors if we have

Paramters :			sc_buffer:Buffer*	Passing the pointer to the buffer

Return Value :		It return the token with the appropriate attribute according to their specification.

Algorithm :			checking all cases for our Special case tokens processed separately one by one in the token-driven part of the scanner  
					'=' , ' ' , '(' , ')' , '{' , '}' , == , <> , '>' , '<' , ';'  white space
					!!comment , ',' , '"' , ';' , '-' , '+' , '*' , '/', # ,
				    .AND., .OR. , SEOF, 'illigal symbol', etc.These all cases returning token with their attribute according
					to their specification for example.AND., .OR. are logical operations so these are returning LOG_OP_T.
					Defining different functions for different states using transition table .
********************************************************************************************************/

Token malar_next_token(Buffer * sc_buf)
{
	int i = 0;
	Token t = { 0 }; /* token to return after pattern recognition. Set all structure members to 0 */
	unsigned char c; /* input symbol */
	int state = 0; /* initial state of the FSM */
	short lexstart;  /*start offset of a lexeme in the input char buffer (array) */
	short lexend;    /*end   offset of a lexeme in the input char buffer (array)*/
	int accept = NOAS; /* type of state - initially not accepting */

	/* endless loop broken by token returns it will generate a warning */
	while (1) {
		/*get the next character from  buffer*/
		c = b_getc(sc_buf);
		
		switch (c) {
		/*  check if there is any new line then increase the line count */
		case '\n': 
			line++;
			break;

		case '!': {

			c = b_getc(sc_buf);							

			if (c != '!') {								

				t.code = ERR_T;
				t.attribute.err_lex[0] = '!';			
				t.attribute.err_lex[1] = c;
				t.attribute.err_lex[2] = '\0';			


				while (c != '\n') {
					c = b_getc(sc_buf);
				}
				line++;
				return t;
			}

			else {
				while (c != '\n') {
					c = b_getc(sc_buf);
					if (c == 255 || c == '\0') return t;
				}
				line++;
				continue;
			}
		}

		case 255: case '\0':
			t.code = SEOF_T;
			return t;

		case '=':
			if ((b_getc(sc_buf) == '=')) {					
				t.code = REL_OP_T;
				t.attribute.rel_op = EQ;
				return t;
			}												
			else {
				b_retract(sc_buf);
				t.code = ASS_OP_T;
				return t;
			}

		case '<':
			if ((b_getc(sc_buf) == '>')) {
				t.code = REL_OP_T;
				t.attribute.rel_op = NE;
				return t;
			}
			
			else {
				b_retract(sc_buf);
				t.code = REL_OP_T;
				t.attribute.rel_op = LT;
				return t;
			}

		case '>':
			t.code = REL_OP_T;
			t.attribute.rel_op = GT;
			return t;

		case'{':
			t.code = LBR_T;
			return t;

		case'}':
			t.code = RBR_T;
			return t;

		case'(':
			t.code = LPR_T;
			return t;

		case')':
			t.code = RPR_T;
			return t;

		case',':
			t.code = COM_T;
			return t;

		case';':
			t.code = EOS_T;
			return t;

		case'+':
			t.code = ART_OP_T;
			t.attribute.arr_op = PLUS;
			return t;

		case'-':
			t.code = ART_OP_T;
			t.attribute.arr_op = MINUS;
			return t;

		case'*':
			t.code = ART_OP_T;
			t.attribute.arr_op = MULT;
			return t;

		case'/':
			t.code = ART_OP_T;
			t.attribute.arr_op = DIV;
			return t;

		case'#':
			t.code = SCC_OP_T;
			return t;

		case'.':
			c = b_getc(sc_buf);
			int reader = 0;
			if (c == 'A') {
				reader++;
				c = b_getc(sc_buf);
				if (c == 'N') {
					c = b_getc(sc_buf);
					reader++;

					if (c == 'D') {
						c = b_getc(sc_buf);
						reader++;

						if (c == '.') {
							t.code = LOG_OP_T;
							t.attribute.log_op = AND;
							return t;
						}
					}
				}
				
				i = 0;

				b_retract(sc_buf);

				for (i = 0; i <= reader; i++)
					b_retract(sc_buf);

				c = b_getc(sc_buf);
				t.code = ERR_T;
				t.attribute.err_lex[0] = c;
				t.attribute.err_lex[1] = '\0';
				return t;
			}

			else if (c == 'O') {
				reader = 0;
				c = b_getc(sc_buf);
				reader++;

				if (c == 'R') {
					c = b_getc(sc_buf);
					reader++;

					if (c == '.') {
						t.code = LOG_OP_T;
						t.attribute.log_op = OR;
						return t;
					}
				}
				
				i = 0;
				b_retract(sc_buf);


				for (i = 0; i <= reader; i++)
					b_retract(sc_buf);

				c = b_getc(sc_buf);
				t.code = ERR_T;
				t.attribute.err_lex[0] = c;
				t.attribute.err_lex[1] = '\0';
				return t;
			}
			
			else {
				b_retract(sc_buf);
				t.code = ERR_T;
				t.attribute.err_lex[0] = '.';
				t.attribute.err_lex[1] = '\0';
				return t;
			}

		}

		if (c == '"') {

			lexstart = b_mark(sc_buf, b_getcoffset(sc_buf));
			int len = 0;

			while (1) {
				c = b_getc(sc_buf);

				if (c == '\n') {
					line++;
				}
				
				else if (c == 255) {
					lexend = b_getcoffset(sc_buf);
					
					len = lexend - lexstart;

					b_reset(sc_buf);

					b_retract(sc_buf);
					t.code = ERR_T;
					
					if ((len) > 20) {
						i = 0;
						for (i = 0; i <= 16; i++)
							t.attribute.err_lex[i] = b_getc(sc_buf);

						t.attribute.err_lex[17] = '.';
						t.attribute.err_lex[18] = '.';
						t.attribute.err_lex[19] = '.';
						t.attribute.err_lex[20] = '\0';

						while (i < lexend) {
							b_getc(sc_buf);
							i++;
						}
					}
					else {

						for (i = 0; i <= 16; i++)
							t.attribute.err_lex[i] = b_getc(sc_buf);
						t.attribute.err_lex[i] = '\0';
					}
					return t;

				}
				else if (c == '"') {
					b_retract(sc_buf);
					lexend = b_getcoffset(sc_buf);

					len = lexend - lexstart;

					i = 0;

					b_reset(sc_buf);


					t.code = STR_T;
					t.attribute.str_offset = b_limit(str_LTBL);
					
					for (i = 0; i < len; i++) {
						c = b_getc(sc_buf);
						if ((b_addc(str_LTBL, c)) == NULL) return;
					}

					b_getc(sc_buf);
					if ((b_addc(str_LTBL, '\0')) == NULL) return;


					return t;
				}
			}
		}
		else if (isalpha(c) || isdigit(c)) {

			b_retract(sc_buf);
			lexstart = b_mark(sc_buf, b_getcoffset(sc_buf));
			c = b_getc(sc_buf);

			state = 0;

		FSM1:state = get_next_state(state, c, &accept);
			if (accept == NOAS) {
				c = b_getc(sc_buf);
				goto FSM1;
			}

			if (accept == ASWR) {
				b_retract(sc_buf);
			}
			
			lexend = b_getcoffset(sc_buf);
			lex_buf = b_allocate(lexend - lexstart + 1, 0, 'f');
			
			if (lex_buf == NULL) {
				scerrnum = 1;
				t = aa_func12("Runtime Error");
				t.code = RTE_T;
				return t;
			}
			
			i = 0;

			b_reset(sc_buf);

			for (i = lexstart; i < lexend; i++) {
				b_addc(lex_buf, b_getc(sc_buf));
			}
			b_addc(lex_buf, '\0');
			t = aa_table[state](b_location(lex_buf, 0));
			
			b_free(lex_buf);
			
			return t;
		}
		else if (isspace(c))
			continue;


		t.code = ERR_T;
		t.attribute.err_lex[0] = c;
		t.attribute.err_lex[1] = '\0';
		return t;
	}
}



/***********************************************************************************
Purpose :			gets the next state from the transition table and returns it to
the calling function
Author :			Svillen Ranev
History/Version :	1.0
Called functions :	char_class(),assert(), printf(), exit()
parameters :		state:int, takes the current state of FSM
					c:char, takes the current character read from buffer
					accept: int*, its the type of state
Return value :		int, returns the next state
Algorithm :

***********************************************************************************/
int get_next_state(int state, char c, int *accept)
{
	int col;
	int next;
	col = char_class(c);
	next = st_table[state][col];
#ifdef DEBUG
	printf("Input symbol: %c Row: %d Column: %d Next: %d \n", c, state, col, next);
#endif

	assert(next != IS);


#ifdef DEBUG
	if (next == IS) {
		printf("Scanner Error: Illegal state:\n");
		printf("Input symbol: %c Row: %d Column: %d\n", c, state, col);
		exit(1);
	}
#endif
	*accept = as_table[next];

	return next;
}

/***********************************************************************************

Purpose :			returns the column number of the transition table for a particular
					character
Author :			Ashish Padaniya
History/Version :	1.0
Called functions :
parameters :		c: char, takes character from calling function
Return value :		int, returns the column number
Algorithm :			Check all the possible cases to find out the accepting state
					and return the val which is our coloumn number


***********************************************************************************/
int char_class(char c)
{
	int val = 0;

	if (c >= 'A' && c <= 'F') val = 0;	/*check if the input character is in between A to F return val 0*/
	else if (c == 'x') val = 1;	/*if it is x return val 1*/
	else if ((c >= 'a' && c <= 'w') || (c == 'y' || c == 'z') || (c >= 'G' && c <= 'Z')) val = 2;/*if it is in between a to z or G to Z return 2*/
	else if (c == '0') val = 3;	 /*if it is 0 return 3*/
	else if (c >= '1' && c <= '9') val = 4; /*if it is in between 1 to 9*/
	else if (c == '.') val = 5; /*if it is . return 5*/
	else if (c == '$') val = 6; /*if it is $ return 6*/
	else val = 7; /*else is for other symbols return 7*/

	return val;
}



/***********************************************************************************

Purpose :			Convert the lexeme representing the arithmentic variable identifier AND keywords(VID - AVID / KW)
Author :			Ashish Padaniya
History/Version :	1.0
Called functions :	iskeyword()
parameters :		lexeme: char[], takes the stream of character to produce token
Return value :		Returns AVID_T and KW_T token
Algorithm :			Get the stream of chracters and pass them in iskeyword function to check if it is keyword or not.
					The keyword function returns index if it is -1 that means it is an AVID. Get that stream of characters
					and store it in vid_lex character by character and at the end add '\0' to make it C-type of String.
					Else It is a keyword. Return the token variable with their index where it is present.

***********************************************************************************/
Token aa_func02(char lexeme[]) {

	Token variable;
	int index = iskeyword(lexeme);



	if (index == -1) {
		int i = 0;
		variable.code = AVID_T;
		for (i = 0; i < VID_LEN; i++) {
			variable.attribute.vid_lex[i] = lexeme[i];
		}
		variable.attribute.vid_lex[VID_LEN] = '\0';
	}

	else {
		variable.code = KW_T;
		variable.attribute.kwt_idx = index;

	}
	return variable;

}


/***********************************************************************************

Purpose :			Convert the lexeme representing the string variable identifier(VID-SVID)
Author :			Ashish Padaniya
History/Version :	1.0
Called functions :
parameters :		lexeme: char[], takes the stream of character to produce token
Return value :		Returns SVID_T token
Algorithm :			It will return the token SVID and next store the total length of array vid_lex
					Run a loop to store all stream of characters in vid-lex one by one at end add '$'to make it SVID.
				    add '\0' after adding '$' to make it C-type String variable identifier and retur possible token.

***********************************************************************************/
Token aa_func03(char lexeme[]) {
	Token t;
	t.code = SVID_T;
	int length = VID_LEN - 1;
	int i = 0;

	for (i = 0; i < length; i++)
		t.attribute.vid_lex[i] = lexeme[i];

	if (t.attribute.vid_lex[i - 1] != '$') {
		t.attribute.vid_lex[i] = '$';
	}

	t.attribute.vid_lex[i + 1] = '\0';

	return t;
}

/***********************************************************************************

Purpose :			Check the String lexeme is a floating point String or not
Author :			Ashish Padaniya
History/Version :	1.0
Called functions :	aa_fun12(),atof();
parameters :		lexeme: char[], takes the stream of character to produce token
Return value :		Returns FPL_L token
Algorithm :         convert the lexeme into floating value and store it in num which as data type double
					next check if num value is in between the range of FLT min to FLT max. If not return error
					else return token FPL_T with attribute value num.

***********************************************************************************/
Token aa_func08(char lexeme[]) {
	
	Token t;
	double num = atof(lexeme);

	if ((num < FLT_MIN || num > FLT_MAX) && num != 0) {
		return aa_table[12](lexeme);
	}

	t.code = FPL_T;
	t.attribute.flt_value = (float)num;
	return t;
}

/***********************************************************************************

Purpose :			To find the lexeme we passing in the parameter is a INT literal or not
Author :			Ashish Padaniya
History/Version :	1.0
Called functions :	aa_fun12(),atoi();
parameters :		lexeme: char[], takes the stream of character to produce token
Return value :		Returns INT_L token
Algorithm :		    convert the lexeme into integer and store it in num which as data type long
					next check if num value is in between the range of short min to short max. If not return error
					else return token INL_T with attribute value num.

***********************************************************************************/
Token aa_func05(char lexeme[]) {
	
	Token t;
	long num = atoi(lexeme);

	if (num < 0 || num > 32767) {
		return aa_func12(lexeme);
	}

	t.code = INL_T;
	t.attribute.int_value = (int)num;
	return t;

}

/***********************************************************************************

Purpose :			To check lexeme representing an hexadecimal integer literal
Author :			Ashish Padaniya
History/Version :	1.0
Called functions :	aa_fun12(),atoh();
parameters :		lexeme: char[], takes the stream of character to produce token
Return value :		Returns INT_L token
Algorithm :		    convert the lexeme into hexadecimal and store it in num as data type long
				    check if num value is in between the range of short min to short max. 
					If not return error else return token INL_T with attribute value num.

***********************************************************************************/
Token aa_func11(char lexeme[]) {
	
	Token t;
	/*calling atolh function to change the lexeme into hexadecimal
	integer literal*/
	long num = atolh(lexeme);

	if (num < 0 || num > VALIDMAX) {
		return aa_func12(lexeme);
	}

	t.code = INL_T;
	t.attribute.int_value = (int)num;
	return t;
}
/***********************************************************************************
Purpose :			check the error from error[] table
Author :			Ashish Padaniya
History/Version :	1.0
Called functions :	strlen()
parameters :		lexeme: char[], takes the stream of character to produce token
Return value :		Returns error token
Algorithm :			check if length of lexeme is greater than err-len if yes then run a loop which starts from 
					0 and runs till err_len -3 t will save that character of streams in err_lex and exit the loop.
					
***********************************************************************************/
Token aa_func12(char lexeme[]) {
	int i = 0;
	Token t;
	t.code = ERR_T;
	
	if (strlen(lexeme) > ERR_LEN) {
		for (i = 0; i < ERR_LEN - 3; i++) {
			t.attribute.err_lex[i] = lexeme[i];
		}
		t.attribute.err_lex[17] = '.';
		t.attribute.err_lex[18] = '.';
		t.attribute.err_lex[19] = '.';
		t.attribute.err_lex[20] = '\0';
		return t;
	}


	for (i = 0; i < strlen(lexeme); i++)
		t.attribute.err_lex[i] = lexeme[i];

	t.attribute.err_lex[i] = '\0';
	return t;
}


/***********************************************************************************

Purpose :			converts string to Hexadecimal Integer literal
Author :			Ashish Padaniya
History/Version :	1.0
Called functions :	strol()
parameters :		lexeme: char*, takes the stream of character to produce token
Return value :		long, returns converted number
Algorithm :

***********************************************************************************/
long atolh(char * lexeme) {

	return strtol(lexeme, NULL, 0);
}

/***********************************************************************************

Purpose :			to get recgonize the keywords in lexeme
Author :			Ashish Padaniya
History/Version :	1.0
Called functions :	strcmp
parameters :		kw_lexeme: char*, takes the stream of character to produce token
Return value :		return the index number of that specific keyword if there is no error else -1
Algorithm :			compare lexeme with each and every keywords we have in keyword table and if it matches return index.
***********************************************************************************/
int iskeyword(char * kw_lexeme){
	int i = 0;
	for (i = 0; i < KWT_SIZE; i++) {
		if (strcmp(kw_table[i], kw_lexeme) == 0) {
			return i;
		}
	}
	return -1;
}