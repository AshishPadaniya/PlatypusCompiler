/******************************************************************************************************
File name 	:				parser.h
Compiler 	:				MS Visual Studio 2015
Author 	    :				Ashish Padaniya 040852591
Course 	    :				CST8152 - Compliers , Lab Section : 11
Assignment  :				3
Date 		:				10 April 2018
Professor 	:				Sv.Ranev
Purpose 	:				This is a header file for parser.c that has all the variable definitions, constant defination and function prototype.

Function List :             Token malar_next_token(),void parser(), match(),syn_eh()
							gen_incode(char* print),syn_printe(),program(),opt_statements(),assignmentStatement(),selectionStatement(),assignment_Expression();
							conditional_Expression(),logical_OR_expression(),logical_AND_expression(),rel_op(),relationalExpression(),primary_s_relational();
							primary_a_relational(),logical_AND_expression_prime(),logical_OR_expression_prime(),statements(),statement(),statements_prime(),
							inputStatement(),iterationStatement(),outputStatement(),pre_condition(),variable_list(),output_list(),match(int, int),parser(Buffer *),
							syn_printe(),syn_eh(),gen_incode(),arithmatic_Expression(),unary_Arithmatic_Expression(),additive_Arithmatic_Expression(),
							multiplicative_Arithmatic_Expression(),multiplicative_Arithmatic_Expression_prime(),additive_Arithmatic_Expression_prime(),primary_Arithmatic_Expression();

*********************************************************************************************************/

#include "buffer.h"
#include "token.h"

#define NO_ATTR  -1
#define ELSE	 0
#define FALSE    1
#define IF       2
#define PLATYPUS 3
#define READ     4
#define REPEAT   5
#define THEN     6
#define TRUE     7
#define WHILE    8
#define WRITE    9

/*declare global variables*/
static Token lookahead;/*current input token*/
static Buffer* sc_buf;/*pointer to buffer*/

typedef enum { false, true } bool;

extern Token malar_next_token(Buffer * sc_buf);/*function used to get next token*/
extern int line;
extern Buffer * str_LTBL;
extern char *kw_table[];

int synerrno = 0;/*to keep record of counter */

void parser(Buffer * in_buf);
void match(int pr_token_code, int pr_token_attribute);
void syn_eh(int sync_token_code);
void gen_incode(char* print);
void syn_printe();
void program(void);
void opt_statements(void);
void assignmentStatement(void);
void selectionStatement(void);
void assignment_Expression(void);
void conditional_Expression(void);
void logical_OR_expression(void);
void logical_AND_expression(void);
void rel_op(void);
void relationalExpression(void);
void primary_s_relational(void);
void primary_a_relational(void);
void logical_AND_expression_prime(void);
void logical_OR_expression_prime(void);
void statements(void);
void statement(void);
void statements_prime(void);
void inputStatement(void);
void iterationStatement(void);
void outputStatement(void);
void pre_condition(void);
void variable_list(void);
void variable_Identifier();
void variablelist_prime();
void output_list(void);
void match(int, int);
void parser(Buffer *);
void syn_printe(void);
void syn_eh(int);
void gen_incode(char *);
void string_Expression();
void primary_string_expression();
void string_Expression_prime();
void arithmatic_Expression(void);
void unary_Arithmatic_Expression(void);
void additive_Arithmatic_Expression(void);
void multiplicative_Arithmatic_Expression(void);
void multiplicative_Arithmatic_Expression_prime(void);
void additive_Arithmatic_Expression_prime(void);
void primary_Arithmatic_Expression(void);