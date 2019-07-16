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

#ifndef  TABLE_H_
#define  TABLE_H_ 

#define  VALIDMAX 32767
#define  VALIDMIN -32768

#ifndef BUFFER_H_
#include "buffer.h"
#endif

#ifndef NULL
#include <_null.h> /* NULL pointer constant is defined there */
#endif

/*   Source end-of-file (SEOF) sentinel symbol
 *    '\0' or one of 255,0xFF,EOF
 */

/*  Special case tokens processed separately one by one
 *  in the token-driven part of the scanner
 *  '=' , ' ' , '(' , ')' , '{' , '}' , == , <> , '>' , '<' , ';',
 *  white space
 *  !!comment , ',' , '"' , ';' , '-' , '+' , '*' , '/', # ,
 *  .AND., .OR. , SEOF, 'illigal symbol',
 */
 

/*REPLACE *ESN* WITH YOUR ERROR STATE NUMBER */
#define ES  12 /* Error state */
#define ER  13 /* Error state */
#define IS -1    /* Inavalid state */

/* State transition table definition */

/*REPLACE *CN* WITH YOUR COLUMN NUMBER  */

#define TABLE_COLUMNS 14
/*transition table - type of states defined in separate table */
int  st_table[ ][TABLE_COLUMNS] = {
	          /* { A-F , x , [a-w] [y-z] [G-Z] , [0] , [1-9] , [.] , [$] , [other]*/

	/* State 0 */{ 1 , 1 , 1 , 6 , 4 , ES , ES , ES },
	/* State 1 */{ 1 , 1 , 1 , 1 , 1 , 2 , 3 , 2 },
	/* State 2 */{ IS , IS ,IS , IS , IS , IS , IS , IS },
	/* State 3 */{ IS , IS , IS , IS , IS , IS , IS , IS },
	/* State 4 */{ ES , ES , ES , 4 , 4 , 7 , 5 , 5 },
	/* State 5 */{ IS , IS , IS , IS , IS , IS , IS , IS },
	/* State 6 */{ ES , 9 , ES , 6 , ES , 7 , ES , 5 },
	/* State 7 */{ ER , ER , 8 , 7 , 7 , 8 , 8 , 8 },
	/* State 8 */{ IS , IS , IS , IS , IS , IS , IS , IS },
	/* State 9 */{ 10 , ER , ER , 10 , 10 , ER , ES , ER },
	/* State 10 */{ 10 , ES , ES , 10 , 10 , 11 , 11 , 11 },
	/* State 11 */{ IS , IS , IS , IS , IS , IS , IS , IS },
	/* State 12 */{ IS , IS , IS , IS , IS , IS , IS , IS },
	/* State 13 */{ IS , IS , IS , IS , IS , IS , IS , IS } 
};

 
/* Accepting state table definition */
/*REPLACE *N1*, *N2*, and *N3* WITH YOUR NUMBERS*/
#define ASWR     2  /* accepting state with retract */
#define ASNR     1  /* accepting state with no retract */
#define NOAS     0  /* not accepting state */

int as_table[] = { NOAS , NOAS , ASWR , ASNR , NOAS , ASWR , NOAS , NOAS , ASWR , NOAS , NOAS , ASWR , ASNR , ASWR };

/* Accepting action function declarations */
/*
FOR EACH OF YOUR ACCEPTING STATES YOU MUST PROVIDE
ONE FUNCTION PROTOTYPE. THEY ALL RETURN Token AND TAKE
ONE ARGUMENT: A string REPRESENTING A TOKEN LEXEME. 

Token aa_funcXX(char *lexeme); 

Replace XX with the number of the accepting state: 02, 03 and so on.
*/
/* defining a new type: pointer to function (of one char * argument) 
   returning Token
*/  


typedef Token (*PTR_AAF)(char *lexeme);


Token aa_func02(char lexeme[]);
Token aa_func03(char lexeme[]);
Token aa_func05(char lexeme[]);
Token aa_func08(char lexeme[]);
Token aa_func11(char lexeme[]);
Token aa_func12(char lexeme[]);

/* Accepting function (action) callback table (array) definition */
/* If you do not want to use the typedef, the equvalent declaration is:
 * Token (*aa_table[])(char lexeme[]) = {
 */

PTR_AAF aa_table[ ] ={
	NULL ,
	NULL ,
	aa_func02,
	aa_func03,
	NULL ,
	aa_func05,
	NULL ,
	NULL ,
	aa_func08,
	NULL,
	NULL,
	aa_func11,
	aa_func12,
	aa_func12
};

/* Keyword lookup table (.AND. and .OR. are not keywords) */

#define KWT_SIZE  10

char * kw_table []=
	{
	"ELSE",
	"FALSE",
	"IF",
	"PLATYPUS",
	"READ",
	"REPEAT",
	"THEN",
	"TRUE",
	"WHILE",
	"WRITE"   
	};

#endif
                     