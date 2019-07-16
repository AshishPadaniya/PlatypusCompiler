/************************************************************************
File Name :		parser.c
Compiler :		MS Visual Studio 2015
Author :		Ashish Padaniya 040852591
Course :		CST 8152 – Compilers
Lab Section:    011
Assignment :	3
Date :			10 April 2018
Professor :		Sv. Ranev
Purpose :       Implementing different kind of functions using productions given in grammer
				by creating the parser by using existing lexical analyzer and symbol table

Function List : Token malar_next_token(),void parser(), match(),syn_eh()
				gen_incode(char* print),syn_printe(),program(),opt_statements(),assignmentStatement(),selectionStatement(),assignment_Expression();
				conditional_Expression(),logical_OR_expression(),logical_AND_expression(),rel_op(),relationalExpression(),primary_s_relational();
				primary_a_relational(),logical_AND_expression_prime(),logical_OR_expression_prime(),statements(),statement(),statements_prime(),
				inputStatement(),iterationStatement(),outputStatement(),pre_condition(),variable_list(),output_list(),match(int, int),parser(Buffer *),
				syn_printe(),syn_eh(),gen_incode(),arithmatic_Expression(),unary_Arithmatic_Expression(),additive_Arithmatic_Expression(),
				multiplicative_Arithmatic_Expression(),multiplicative_Arithmatic_Expression_prime(),additive_Arithmatic_Expression_prime(),primary_Arithmatic_Expression();
**************************************************************************/

#include "parser.h"
/*******************************************************************************************************
Purpose :			 This function will get the file and parse it.
Author	:			 Svillen Ranev
History :			 18 April 2018
Called functions :	 malar_next_token(),
					 program(),
					 match().
Paramters :			 Passing the pointer to the buffer in the parameter.
Return Value :
Algorithm :
********************************************************************************************************/


void parser(Buffer * in_buf) {
	sc_buf = in_buf;
	lookahead = malar_next_token(sc_buf);
	program(); match(SEOF_T, NO_ATTR);
	gen_incode("PLATY: Source file parsed");
}


/*******************************************************************************************************
Purpose :			This function basically used to matches the current input token and the token required by parser.
Author	:			Ashish Padaniya
History :			18 April 2018
Called functions :	syn_printe(),malar_next_token() and syn_eh()
Paramters :			Passing the token code and attributes as int parameters
Return Value :
Algorithm :	
********************************************************************************************************/

void match(int pr_token_code, int pr_token_attribute) {
	
	bool check = false;

	if (lookahead.code == pr_token_code) {
		if (pr_token_attribute == NO_ATTR) {
			check = true;
		}

		else
			switch (pr_token_code) {
			case KW_T:
				if (kw_table[lookahead.attribute.get_int] == kw_table[pr_token_attribute]) check = true; break;
			case REL_OP_T:
				if (lookahead.attribute.rel_op == pr_token_attribute)  check = true; break;
			case ART_OP_T:
				if (lookahead.attribute.arr_op == pr_token_attribute) check = true; break;
			case LOG_OP_T:
				if (lookahead.attribute.log_op == pr_token_attribute) check = true; break;

			}
	}
	else check = false;

	if (check == true) {

		if (lookahead.code == SEOF_T) {
			return;
		}

		else {
			lookahead = malar_next_token(sc_buf);

			if (lookahead.code == ERR_T) {
				syn_printe();
				lookahead = malar_next_token(sc_buf);
				synerrno++;
			}
			return;
		}
		return;
	}
	else {
		syn_eh(pr_token_code);
		return;
	}

}

/*******************************************************************************************************
Purpose :			This function is takes care of error handing. It implements simple panic mode error recovery
History :			18 April 2018
Called functions :	syn_printe(),malar_next_token(),
Paramters :			Passing the token code of type int.
Return Value :
Algorithm :
********************************************************************************************************/

void syn_eh(int sync_token_code) {

	syn_printe();
	synerrno++;

	while (lookahead.code != sync_token_code) {

		if (lookahead.code == SEOF_T) exit(synerrno);
		lookahead = malar_next_token(sc_buf);

	}

	if (lookahead.code == SEOF_T) return;

	lookahead = malar_next_token(sc_buf);
	return;


}
/*******************************************************************************************************
Purpose :		    This function prints			
History :		    18 April 2018
Called functions :  No function calls
Paramters :		    Passing the pointer to the char in the parameter.
Return Value :		None
Algorithm :         It takes string as an argument and prints it.
********************************************************************************************************/

void gen_incode(char* print) {

	printf("%s\n", print);
}

/*******************************************************************************************************
Purpose :				This function prints error according to the error
Author	:				Svillen Ranev
History :				18 April 2018
Called functions :		b_location()
Paramters :				No Parameters
Return Value :
Algorithm :
********************************************************************************************************/

void syn_printe() {
	Token t = lookahead;

	printf("PLATY: Syntax error:  Line:%3d\n", line);
	printf("*****  Token code:%3d Attribute: ", t.code);
	switch (t.code) {
	case  ERR_T: /* ERR_T     0   Error token */
		printf("%s\n", t.attribute.err_lex);
		break;
	case  SEOF_T: /*SEOF_T    1   Source end-of-file token */
		printf("NA\n");
		break;
	case  AVID_T: /* AVID_T    2   Arithmetic Variable identifier token */
	case  SVID_T:/* SVID_T    3  String Variable identifier token */
		printf("%s\n", t.attribute.vid_lex);
		break;
	case  FPL_T: /* FPL_T     4  Floating point literal token */
		printf("%5.1f\n", t.attribute.flt_value);
		break;
	case INL_T: /* INL_T      5   Integer literal token */
		printf("%d\n", t.attribute.get_int);
		break;
	case STR_T:/* STR_T     6   String literal token */
		printf("%s\n", b_location(str_LTBL, t.attribute.str_offset));
		break;

	case SCC_OP_T: /* 7   String concatenation operator token */
		printf("NA\n");
		break;

	case  ASS_OP_T:/* ASS_OP_T  8   Assignment operator token */
		printf("NA\n");
		break;
	case  ART_OP_T:/* ART_OP_T  9   Arithmetic operator token */
		printf("%d\n", t.attribute.get_int);
		break;
	case  REL_OP_T: /*REL_OP_T  10   Relational operator token */
		printf("%d\n", t.attribute.get_int);
		break;
	case  LOG_OP_T:/*LOG_OP_T 11  Logical operator token */
		printf("%d\n", t.attribute.get_int);
		break;

	case  LPR_T: /*LPR_T    12  Left parenthesis token */
		printf("NA\n");
		break;
	case  RPR_T: /*RPR_T    13  Right parenthesis token */
		printf("NA\n");
		break;
	case LBR_T: /*    14   Left brace token */
		printf("NA\n");
		break;
	case RBR_T: /*    15  Right brace token */
		printf("NA\n");
		break;

	case KW_T: /*     16   Keyword token */
		printf("%s\n", kw_table[t.attribute.get_int]);
		break;

	case COM_T: /* 17   Comma token */
		printf("NA\n");
		break;
	case EOS_T: /*    18  End of statement *(semi - colon) */
		printf("NA\n");
		break;
	default:
		printf("PLATY: Scanner error: invalid token code: %d\n", t.code);
	}/*end switch*/
}/* end syn_printe()*/
 /*******************************************************************************************************
 Production:<program>->PLATYPUS {<opt_statements>}
 First Set:FIRST(<program>) ={KW_T(PLATYPUS)}
 Author Name: Svillen Ranev
 ********************************************************************************************************/
void program() {

	match(KW_T, PLATYPUS);
	match(LBR_T, NO_ATTR);
	opt_statements();
	match(RBR_T, NO_ATTR);
	gen_incode("PLATY: Program parsed");
}
/*******************************************************************************************************
Production:opt_statements>-><statements>| empty sting(epsilon)
First Set:FIRST(<opt_statements>)={AVID,SVID,EOS_T,KW_T(IF), KW_T(WHILE),
KW_T(READ), KW_T(WRITE), empty sting(epsilon)}
Author Name: Svillen Ranev
********************************************************************************************************/
void opt_statements() {
	switch (lookahead.code) {
	case AVID_T:
	case SVID_T:  statements(); break;
	case KW_T:

		if (lookahead.attribute.get_int != PLATYPUS
			&&lookahead.attribute.get_int != ELSE
			&&lookahead.attribute.get_int != THEN
			&&lookahead.attribute.get_int != REPEAT
			&&lookahead.attribute.get_int != TRUE
			&&lookahead.attribute.get_int != FALSE) {

			statements();
			break;
		}
	default:
		gen_incode("PLATY: Opt_statements parsed");
	}
}
/*******************************************************************************************************
Production:<statements> -><statement><statements’>
First Set:FIRST(<statements>)={FIRST(<statement>)}={AVID,SVID,EOS_T,KW_T(IF), KW_T(WHILE), KW_T(READ), KW_T(WRITE)}
********************************************************************************************************/
void statements() {
	statement();
	statements_prime();
}
/*******************************************************************************************************
Production:<statement> -><assignment statement>
| <selection statement>
| <iteration statement>
| <input statement>
| <output statement>

First Set:FIRST(<statement>) ={FIRST(<assignment statement>), FIRST(<selection
statement>), FIRST(<iteration statement>), FIRST(<input statement>),
FIRST(<output statement>)} ={AVID,SVID,EOS_T,KW_T(IF), KW_T(WHILE),
KW_T(READ), KW_T(WRITE)}
********************************************************************************************************/
void statement() {

	switch (lookahead.code) {
	case AVID_T:
	case SVID_T:  assignmentStatement(); break;
	case KW_T:

		if (lookahead.attribute.get_int == IF) selectionStatement();

		if (lookahead.attribute.get_int == WHILE) iterationStatement();

		if (lookahead.attribute.get_int == WRITE) outputStatement();

		if (lookahead.attribute.get_int == READ) inputStatement();

		break;
	default:
		syn_printe();
		break;
	}
}
/*******************************************************************************************************
Production:<statements’>-> <statement>< statements’>| empty sting(epsilon)
First Set:FIRST(<statements’>)={AVID,SVID,EOS_T,KW_T(IF), KW_T(WHILE),
KW_T(READ), KW_T(WRITE),empty sting(epsilon)}
********************************************************************************************************/
void statements_prime() {

	switch (lookahead.code) {
	case AVID_T:
	case SVID_T:
		statements(); break;
	case KW_T:

		if (lookahead.attribute.get_int != PLATYPUS
			&&lookahead.attribute.get_int != ELSE
			&&lookahead.attribute.get_int != THEN
			&&lookahead.attribute.get_int != REPEAT)
		{
			statements();
		}
		break;
	default:
		break;
	}

}
/*******************************************************************************************************
Production:<assignment statement>-><assignment expression>;
First Set:FIRST(<assignment statement >)={ AVID ,SVID}
********************************************************************************************************/
void  assignmentStatement() {
	assignment_Expression();
	match(EOS_T, NO_ATTR);
	gen_incode("PLATY: Assignment statement parsed");
}
/*******************************************************************************************************
Production:< assignment expression> ->  AVID| SVID
First Set:FIRST(<assignment expression >) ={ AVID ,SVID}
********************************************************************************************************/
void assignment_Expression() {

	switch (lookahead.code) {

	case AVID_T:
		match(AVID_T, NO_ATTR);
		match(ASS_OP_T, NO_ATTR);
		arithmatic_Expression();
		gen_incode("PLATY: Assignment expression (arithmetic) parsed");
		break;

	case SVID_T:
		match(SVID_T, NO_ATTR);
		match(ASS_OP_T, NO_ATTR);
		string_Expression();
		gen_incode("PLATY: Assignment expression (string) parsed");
		break;
	}
}
/*******************************************************************************************************
Production:<selection statement> ->
IF <pre-condition> (<conditional expression>) THEN { <opt_statements> }
ELSE { <opt_statements> } ;
First Set: FIRST(<selection statement >) ={ KW_T(IF)}

********************************************************************************************************/
void  selectionStatement() {

	match(KW_T, IF);
	pre_condition();
	match(LPR_T, NO_ATTR);
	conditional_Expression();
	match(RPR_T, NO_ATTR);
	match(KW_T, THEN);
	match(LBR_T, NO_ATTR);
	opt_statements();
	match(RBR_T, NO_ATTR);
	match(KW_T, ELSE);
	match(LBR_T, NO_ATTR);
	opt_statements();
	match(RBR_T, NO_ATTR);
	match(EOS_T, NO_ATTR);
	gen_incode("PLATY: Selection statement parsed");

}
/*******************************************************************************************************
Production:<iteration statement> ->
WHILE <pre-condition> (<conditional expression>)
REPEAT { <statements>};
First Set:FIRST(<iteration statement>) ={ KW_T(WHILE)}
********************************************************************************************************/
void  iterationStatement() {

	match(KW_T, WHILE);
	pre_condition();
	match(LPR_T, NO_ATTR);
	conditional_Expression();
	match(RPR_T, NO_ATTR);
	match(KW_T, REPEAT);
	match(LBR_T, NO_ATTR);
	statements();
	match(RBR_T, NO_ATTR);
	match(EOS_T, NO_ATTR);
	gen_incode("PLATY: Iteration statement parsed");
}
/*******************************************************************************************************
Production:<string expression> ->
<primary string expression><string expression’>
First Set:FIRST(<string expression>)={ FIRST(<primary string expression>)}={ SVID_T,STR_T}
********************************************************************************************************/
void string_Expression() {
	primary_string_expression();
	string_Expression_prime();
	gen_incode("PLATY: String expression parsed");
}
/*******************************************************************************************************
Production:<primary string expression> -> SVID_T| STR_T
First Set:FIRST(<primary string expression>)={ SVID_T,STR_T}
********************************************************************************************************/
void primary_string_expression() {
	switch (lookahead.code) {
	case SVID_T:
		match(SVID_T, NO_ATTR);
		gen_incode("PLATY: Primary string expression parsed");
		break;

	case STR_T:
		match(STR_T, NO_ATTR);
		gen_incode("PLATY: Primary string expression parsed");
		break;

	default:
		syn_printe();
		break;
	}

}
/*******************************************************************************************************
Production:<string expression’>->#<primary string expression><string expression’>|empty sting(epsilon)
First Set:FIRST(<string expression’>)={SCC_OP_T ,empty sting(epsilon) }
********************************************************************************************************/
void string_Expression_prime() {
	switch (lookahead.code) {

	case SCC_OP_T:
		match(SCC_OP_T, NO_ATTR);
		primary_string_expression();
		string_Expression_prime();

	default:
		break;
	}
}
/*******************************************************************************************************
Production:<conditional expression> ->
<logical OR  expression>
First Set:FIRST(<conditional expression>) ={ FIRST(<logical OR expression>)={ AVID_T, FPL_T ,INL_T , SVID_T, STR_T }
********************************************************************************************************/
void conditional_Expression() {
	logical_OR_expression();
	gen_incode("PLATY: Conditional expression parsed");

}
/*******************************************************************************************************
Production:<logical  OR expression> ->
<logical AND expression><logical OR expression’>
First Set:FIRST(<logical OR expression>)={FIRST(<logical AND expression>)}={ AVID_T, FPL_T ,INL_T , SVID_T, STR_T }
********************************************************************************************************/
void logical_OR_expression() {

	logical_AND_expression();
	logical_OR_expression_prime();
}
/*******************************************************************************************************
Production:<logical OR expression’>  ->.OR. <logical AND expression><logical OR expression’>  |empty sting(epsilon)
First Set:FIRST(<logical OR expression’>)={ LOG_OP_T(OR),empty sting(epsilon)}
********************************************************************************************************/
void logical_OR_expression_prime() {
	switch (lookahead.code) {
	case LOG_OP_T:
		if (lookahead.attribute.log_op == OR) {
			match(LOG_OP_T, OR);
			logical_AND_expression();
			logical_OR_expression_prime();
			gen_incode("PLATY: Logical OR expression parsed");
		}

	default:
		break;
	}
}
/*******************************************************************************************************
Production:<logical AND expression> ->
<relational expression><logical AND expression’>
First Set:FIRST(<logical AND expression >) = {FIRST(<relational expression>)} ={ AVID_T, FPL_T ,INL_T , SVID_T, STR_T }
********************************************************************************************************/
void logical_AND_expression() {
	relationalExpression();
	logical_AND_expression_prime();
}
/*******************************************************************************************************
Production:<logical AND expression’>->.AND.  <relational expression><logical AND expression’>|empty sting(epsilon)
First Set:FIRST(<logical AND expression’>)={ LOG_OP_T(AND) ,empty sting(epsilon)}
********************************************************************************************************/
void logical_AND_expression_prime() {

	switch (lookahead.code) {
	case LOG_OP_T:
		if (lookahead.attribute.log_op == AND) {
			match(LOG_OP_T, AND);
			relationalExpression();
			logical_AND_expression_prime();
			gen_incode("PLATY: Logical AND expression parsed");
		}

	default:
		break;
	}
}
/*******************************************************************************************************
Production:<relational expression> ->
<primary a_relational expression>><rel_op>  <primary a_relational expression>
|<primary s_relational expression>><rel_op>  <primary s_relational expression>
First Set:FIRST(<relational expression>)={FIRST(<primary a_relational expression>),
FIRST(<primary s_relational expression>)} ={ AVID_T, FPL_T ,INL_T , SVID_T, STR_T }
********************************************************************************************************/
void relationalExpression() {

	switch (lookahead.code) {

	case AVID_T:
	case FPL_T:
	case INL_T:
		primary_a_relational();
		rel_op();
		primary_a_relational();
		break;

	case STR_T:
	case SVID_T:
		primary_s_relational();
		rel_op();
		primary_s_relational();
		break;

	default:
		syn_printe();
		break;
	}
	gen_incode("PLATY: Relational expression parsed");

}
/*******************************************************************************************************
Production:<rel_op>-> ==  |  <>  |  >   |  <
First Set:FIRST(<rel_op>)={ REL_OP_T (EQ), REL_OP_T (NE), REL_OP_T (GT), REL_OP_T( LT )}
********************************************************************************************************/
void rel_op() {

	switch (lookahead.code) {
	case REL_OP_T:
		if (lookahead.attribute.rel_op == EQ) match(REL_OP_T, EQ);
		else if (lookahead.attribute.rel_op == NE) match(REL_OP_T, NE);
		else if (lookahead.attribute.rel_op == GT) match(REL_OP_T, GT);
		else if (lookahead.attribute.rel_op == LT) match(REL_OP_T, LT);

		break;

	default:
		syn_printe();
		break;
	}

}
/*******************************************************************************************************
Production:<primary a_relational expression> -> AVID_T| FPL_T| INL_T
First Set:FIRST(<primary a_relational expression>)={ AVID_T,FPL_T,INL_T}
********************************************************************************************************/

void primary_a_relational() {

	switch (lookahead.code) {
	case AVID_T:
		match(AVID_T, NO_ATTR);
		break;

	case FPL_T:
		match(FPL_T, NO_ATTR);
		break;

	case INL_T:
		match(INL_T, NO_ATTR);
		break;

	default:
		syn_printe();
		break;
	}
	gen_incode("PLATY: Primary a_relational expression parsed");

}
/*******************************************************************************************************
Production:<primary s_relational expression> -><primary string expression>
First Set:FIRST(<primary s_relational expression>) = {FRIST(<primary string expression>)} ={SVID_T, STR_T}
********************************************************************************************************/
void primary_s_relational() {

	primary_string_expression();
	gen_incode("PLATY: Primary s_relational expression parsed");

}
/*******************************************************************************************************
Production<pre-condition> ->TRUE | FALSE
First Set:FIRST(<pre-condition>) ={ TRUE,FALSE}
********************************************************************************************************/
void pre_condition() {

	switch (lookahead.code)
	{
	case KW_T:
		if (lookahead.attribute.get_int == TRUE) match(KW_T, TRUE);
		if (lookahead.attribute.get_int == FALSE) match(KW_T, FALSE);
		break;
	}
}
/*******************************************************************************************************
Production:<output statement> -> WRITE (<output list>);
First Set:FIRST(<output statement>) = { KW_T(WRITE)}
********************************************************************************************************/
void  outputStatement() {

	match(KW_T, WRITE);
	match(LPR_T, NO_ATTR);
	output_list();
	match(RPR_T, NO_ATTR);
	match(EOS_T, NO_ATTR);
	gen_incode("PLATY: Output statement parsed");
}
/*******************************************************************************************************
Production:<input statement> ->READ (<variable list>);
First Set:FIRST(<input statement>) ={ KW_T(READ)}
********************************************************************************************************/
void  inputStatement() {

	match(KW_T, READ);
	match(LPR_T, NO_ATTR);
	variable_list();
	gen_incode("PLATY: Variable list parsed");
	match(RPR_T, NO_ATTR);
	match(EOS_T, NO_ATTR);
	gen_incode("PLATY: Input statement parsed");
}
/*******************************************************************************************************
Production:<output list>-><variable list>|STR_T|empty sting(epsilon)
First Set:FIRST(<output list>) = {FIRST(<variable list>),STR_T,epsilon}={ AVID_T,SVID_T ,STR_T,empty sting(epsilon)}
********************************************************************************************************/
void output_list() {

	switch (lookahead.code) {
	case SVID_T:
		match(SVID_T, NO_ATTR);
		variablelist_prime();
		gen_incode("PLATY: Variable list parsed");
		break;

	case AVID_T:
		match(AVID_T, NO_ATTR);
		variablelist_prime();
		gen_incode("PLATY: Variable list parsed");
		break;

	case STR_T:
		match(STR_T, NO_ATTR);
		gen_incode("PLATY: Output list (string literal) parsed");
		break;

	default:
		gen_incode("PLATY: Output list (empty) parsed");
	}

}
/*******************************************************************************************************
Production:<variable list> -><variable identifier><variable list’>
First Set:FIRST(<variable list>) ={ FIRST (<variable identifier>)}={AVID_T,SVID_T}
********************************************************************************************************/
void variable_list() {
	variable_Identifier();
	variablelist_prime();
}
/*******************************************************************************************************
Production:<variable identifier> -> AVID_T | SVID_T
First Set:FIRST(<variable identifier>) ={ AVID_T,SVID_T}
********************************************************************************************************/
void variable_Identifier() {
	switch (lookahead.code) {
	case SVID_T:
		match(SVID_T, NO_ATTR);
		break;

	case AVID_T:
		match(AVID_T, NO_ATTR);
		break;

	default:
		syn_printe();
		break;
	}

}
/*******************************************************************************************************
Production:<variable list’>->,<variable Identifier><variable list’>|empty sting(epsilon)
First Set:FIRST(<variable list’>) ={ COM_T, empty sting(epsilon)}
********************************************************************************************************/
void variablelist_prime() {

	switch (lookahead.code) {
	case COM_T:
		match(COM_T, NO_ATTR);
		variable_list();
		break;

	default:
		break;

	}

}

/*******************************************************************************************************
Production:<arithmetic expression> - >
<unary arithmetic expression>|<additive arithmetic expression>
First Set:	FIRST(< arithmetic expression>)={ FIRST( <unary arithmetic expression>),FIRST(additive arithmetic expression)}
= {ARR_OP(MINUS),ARR_OP( PLUS) , AVID_T,FPL_T,INL_T,LPR_T  }
********************************************************************************************************/
void arithmatic_Expression() {

	switch (lookahead.code) {

	case ART_OP_T:
		if (lookahead.attribute.arr_op == PLUS || lookahead.attribute.arr_op == MINUS) {
			unary_Arithmatic_Expression();
			gen_incode("PLATY: Arithmetic expression parsed");
		}
		break;

	case AVID_T:
	case FPL_T:
	case INL_T:
	case LPR_T:
		additive_Arithmatic_Expression();
		gen_incode("PLATY: Arithmetic expression parsed");
		break;
	}

}
/*******************************************************************************************************
Production:	<additive arithmetic expression> -><multiplicative arithmetic expression>
<additive arithmetic expression’>
First Set:	FIRST(<additive arithmetic expression>)={ FIRST( <multiplicative arithmetic expression>)}=
{AVID_T,FPL_T,INL_T,LPR_T }
********************************************************************************************************/
void additive_Arithmatic_Expression() {
	multiplicative_Arithmatic_Expression();
	additive_Arithmatic_Expression_prime();
}
/*******************************************************************************************************
Production:	<multiplicative arithmetic expression> -><primary arithmetic expression>
<multiplicative arithmetic expression’>
First Set: FIRST(<multiplicative arithmetic expression>)={FIRST<primary arithmetic expression>}=
{AVID_T,FPL_T,INL_T,LPR_T}
********************************************************************************************************/
void multiplicative_Arithmatic_Expression() {
	primary_Arithmatic_Expression();
	multiplicative_Arithmatic_Expression_prime();

}
/*******************************************************************************************************
Production:<multiplicative arithmetic expression’>->
*<primary arithmetic expression><multiplicative arithmetic expression’>
|/<primary arithmetic expression><multiplicative arithmetic expression’>|empty sting(epsilon)
First Set: FIRST(<multiplicative arithmetic expression’>)={ARR_OP_T(MULTI),ARR_OP_T(DIV),empty sting(epsilon)}
*******************************************************************************************************/
void multiplicative_Arithmatic_Expression_prime() {
	switch (lookahead.code) {

	case ART_OP_T:
		if (lookahead.attribute.arr_op == MULT) {
			match(ART_OP_T, MULT);
			multiplicative_Arithmatic_Expression();
			gen_incode("PLATY: Multiplicative arithmetic expression parsed");
		}
		if (lookahead.attribute.arr_op == DIV) {
			match(ART_OP_T, DIV);
			multiplicative_Arithmatic_Expression();
			gen_incode("PLATY: Multiplicative arithmetic expression parsed");
		}
		break;

	default:
		break;
	}
}
/*******************************************************************************************************
Production:<additive arithmetic expression'>->+<multiplicative arithmetic expression><additive arithmetic expression'>
|-<multiplicative arithmetic expression><additive arithmetic expression'>|empty sting(epsilon)
First Set:FIRST(<additive arithmetic expression’>)={ARR_OP_T(PLUS),ARR_OP_T(MINUS),empty sting(epsilon)}
********************************************************************************************************/
void additive_Arithmatic_Expression_prime() {

	switch (lookahead.code) {

	case ART_OP_T:
		if (lookahead.attribute.arr_op == PLUS) {
			match(ART_OP_T, PLUS);
			additive_Arithmatic_Expression();
			gen_incode("PLATY: Additive arithmetic expression parsed");
		}
		if (lookahead.attribute.arr_op == MINUS) {
			match(ART_OP_T, MINUS);
			additive_Arithmatic_Expression();
			gen_incode("PLATY: Additive arithmetic expression parsed");
		}
		break;

	default:
		break;
	}
}
/*******************************************************************************************************
Production: <unary arithmetic expression>->-<primary arithmetic expression>|+<primary arithmetic expression>
First Set:FIRST(<unary arithmetic expression>)={ARR_OP(MINUS),ARR_OP(PLUS)}
********************************************************************************************************/
void unary_Arithmatic_Expression() {

	switch (lookahead.code) {

	case ART_OP_T:
		if (lookahead.attribute.arr_op == PLUS) {
			match(ART_OP_T, PLUS);
			primary_Arithmatic_Expression();
			gen_incode("PLATY: Unary arithmetic expression parsed");
		}
		if (lookahead.attribute.arr_op == MINUS) {
			match(ART_OP_T, MINUS);
			primary_Arithmatic_Expression();
			gen_incode("PLATY: Unary arithmetic expression parsed");
		}
		break;
	}
}
/*******************************************************************************************************
Production: <primary arithmetic expression>->AVID_T|FPL_T|INL_T|(<arithmetic expression>)
First Set: FIRST(<primary arithmetic expression>)={AVID_T,FPL_T,INL_T,FIRST((<arithmetic expression>))}
={AVID_T,FPL_T,INL_T,LPR_T }
********************************************************************************************************/
void primary_Arithmatic_Expression() {

	switch (lookahead.code) {

	case AVID_T:
		match(AVID_T, NO_ATTR);
		gen_incode("PLATY: Primary arithmetic expression parsed");
		break;

	case FPL_T:
		match(FPL_T, NO_ATTR);
		gen_incode("PLATY: Primary arithmetic expression parsed");
		break;

	case INL_T:
		match(INL_T, NO_ATTR);
		gen_incode("PLATY: Primary arithmetic expression parsed");
		break;

	case LPR_T:
		match(LPR_T, NO_ATTR);
		arithmatic_Expression();
		match(RPR_T, NO_ATTR);
		gen_incode("PLATY: Primary arithmetic expression parsed");
		break;
	}
}