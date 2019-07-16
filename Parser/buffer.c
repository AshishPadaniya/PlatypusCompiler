/******************************************************************************************************
File name 	:				buffer.c
Compiler 	:				MS Visual Studio 2015
Author 	    :				Ashish Padaniya, 040852591
Course 	    :				CST8152 - Compliers , Lab Section : 011
Assignment  :				1
Date 		:				09-02-2018
Professor 	:				Sv.Ranev
Purpose 	:				Creating a buffer and reading the file in the buffer and print in the file. 
							The buffer has different modes. One of the mode is fixed mode, 
							In fixed mode buffer has a fixed capacity and it does not change.
							the other mode is self-incrementing mode, In self-incrmenting mode the buffer capcity 
							increases until the file is read completely if it does not reach the maximum value
							after which it cannot be increased and the buffer stops reading.     


Function List :				b_allocate(), b_addc(), b_clear(), b_free(), b_isfull(), b_limit(), b_capacity(),  b_mark(),
							b_mode(), b_incfactor(), b_load(), b_isempty(), b_eob(), b_getc(), b_print(), b_compact(),
							b_rflag(), b_retract(), b_reset(), b_getcoffset(), b_rewind(), b_location().
*********************************************************************************************************/

#include "buffer.h"

/********************************************************************************************************
Purpose 	:				Allocating the memory for one buffer using calloc and using the init_capacity to
							malloc the array with the initial capacity given in the parameter of function. 
							o_mode tell the mode in which buffer going to work and then setting the value 
							of inc_factor and mode of the buffer acc. to the value passed in the parameter in 
							the function.
Author		:				Ashish Padaniya
History		:				09-02-2018
Called functions :
				 calloc() : Allocating memory for the buffer setting the value to zero for the other elements in the buffer structure.
				 malloc() : Allocating memory for array and checking if the memory is allocated if not return null.
Paramters :	init_capacity : Datatype short and it is the initial capacity of the
							buffer to store the data in buffer and the range for init_capacity
							should be in range.
inc_factor :			    If buffer is in additive mode, the range of inc_factor should be between 1 to 255.
							inclusive.
							If the buffer is in multiplicative mode range of inc_factor is
							1 to 100 inclusive.  Used to increase the capacity of the buffer. 
o_mode :					set the mode of the buffer
like :						self-increamenting mode or fixed mode.
Return Value :				It returns the pointer to the buffer.
Algorithm	 :				buffer = (Buffer *)calloc(1, sizeof(Buffer)) [allocating memory for Buffer]
********************************************************************************************************/

Buffer *b_allocate(short init_capacity, char inc_factor, char o_mode)
{
	/*Pointer to structure of buffer that i will return after allocation memory*/
	Buffer *pBD;

	if (o_mode == 'f' && init_capacity == 0)
	{
		return NULL;
	}

	if (init_capacity < 0 || init_capacity > SHRT_MAX - 1)
	{
		return NULL;
	}

	if ((unsigned char)inc_factor < 0) return NULL;

	if (o_mode != 'f' && o_mode != 'a' && o_mode != 'm') return NULL;

	pBD = (Buffer *)calloc(1, sizeof(Buffer));
	if (pBD == NULL) {
		return NULL;
	}

	pBD->cb_head = (char *)malloc(init_capacity * sizeof(short));
	if (pBD->cb_head == NULL)
	{
		free(pBD);
		return NULL;
	}

	else if (o_mode == 'f' || inc_factor == 0)
	{
		pBD->mode = 0;
		pBD->inc_factor = 0;
	}

	else if (o_mode == 'a' && ((unsigned char)inc_factor >= 1 && (unsigned char)inc_factor <= ADD_INC))
	{
		pBD->mode = 1;
		pBD->inc_factor = (unsigned char)inc_factor;
	}


	else if (o_mode == 'm' && (inc_factor >= 1 && inc_factor <= MUL_INC))
	{
		pBD->mode = -1;
		pBD->inc_factor = inc_factor;
	}
	else
	{
		free(pBD->cb_head);
		free(pBD);
		return NULL;
	}
	
	pBD->capacity = init_capacity;
	return pBD;

}


/********************************************************************************************************
Purpose :				To add the character in the array
Author	:				Ashish Padaniya
History :				09-02-2018

Called functions :		realloc() :	Using the reallocating function to allocate the new
					    memory for the cb_head by using the new capacity and then
						checking If the memory is allocated
Paramters :
pBD :					The buffer in which i am adding the character and incrementing
						the capacity if the buffer became full and the capacity is still less
						than the maximum capacity which can be allocated.

symbol:					The character which will be added in the buffer pBD.

Return Value :			It will return the buffer pointer.

Algorithm :  			pBD->cb_head[pBD->addc_offset] = symbol
						pBD->addc_offset++

********************************************************************************************************/
pBuffer b_addc(pBuffer const pBD, char symbol)
{
	
	char * temp;

	short tempcap = 0;

	pBD->r_flag = 0;

	if ((short)(pBD->addc_offset * sizeof(char)) != (pBD->capacity))
	{
		
		pBD->cb_head[pBD->addc_offset++] = symbol;
		return pBD;

	}

	else
	{
		if (pBD->mode == 0)
		{
			return NULL;
		}

		else if (pBD->mode == ADD)
		{
			if (pBD->capacity >= MAX) 	return NULL;

			tempcap = pBD->capacity + ((unsigned char)pBD->inc_factor * sizeof(char));

			if (tempcap > 0) {

				if (tempcap >= MAX)
				{
					tempcap = MAX;
				}

			}
			else
			{
				return NULL;
			}

		}

		else if (pBD->mode == MUL)
		{
			if (pBD->capacity >= MAX) return NULL;

			tempcap = pBD->capacity + ((float)((MAX - pBD->capacity) * (pBD->inc_factor)) / (float)100);

			if (tempcap <= MAX && tempcap > pBD->capacity) {
				pBD->capacity = tempcap;
			}

			else
			{
				tempcap = MAX;
			}
		}
	}

	temp = (char *)realloc(pBD->cb_head, tempcap * sizeof(short));

	if (temp == NULL) return NULL;

	if (pBD->cb_head != temp)
	{
		pBD->cb_head = temp;
		pBD->r_flag = SET_R_FLAG;
	}
	pBD->capacity = tempcap;
	pBD->cb_head[pBD->addc_offset++] = symbol;

	return pBD;
}

/*********************************************************************************************************
Purpose :				Re-initializes all the data members which are
						appropriate like addc_offset, getc_offset, 
						markc_offset, r_flag to zero
Author	:				Ashish Padaniya
History :				09-02-2018
Called functions :		No function calls
Paramters :				Passing the pointer to the buffer in the parameter.
Return Value :			It will return RT_FAIL1 if runtime error occur
						else will retun 0.
Algorithm :
*********************************************************************************************************/
int b_clear(Buffer * const pBD){

	if (pBD == NULL) return RT_FAIL1;

	pBD->addc_offset = 0;
	pBD->getc_offset = 0;
	pBD->markc_offset = 0;
	pBD->eob = 0;
	pBD->r_flag = 0;
	return 0;
}

/*********************************************************************************************************
Purpose :		 Free the memory
Author :		 Ashish Padaniya
History :		 09-02-2018
Called functions :
free()			 This function free the memory occupied.
Paramters :		 Passing the pointer of the buffer.
Return Value :   
Algorithm :
*********************************************************************************************************/
void b_free(Buffer * const pBD) {

	if (pBD == NULL) return;
	/**using free function to clear the memory*/
	free(pBD->cb_head);
	free(pBD);
}

/*********************************************************************************************************
Purpose :		Check whether buffer is full
Author	:		Ashish Padaniya
History :		09-02-2018
Called functions : No function calls
Paramters :		Passing the pointer of buffer.
Return Value :	Return 0 if buffer is not full
				Return 1 if buffer is full
				Return RT_FAIL1 if any run time error.
Algorithm :
*********************************************************************************************************/
int b_isfull(Buffer * const pBD) {

	if (pBD == NULL) return RT_FAIL1;

	if ((pBD->addc_offset) * sizeof(char) == (pBD->capacity)) return 1;

	else return 0;
}

/*********************************************************************************************************
Purpose :			To know amount of space of character we added to the buffer.
Author	:			Ashish Padaniya
History :			09-02-2018
Called functions :	No function calls
Paramters :			Pointer to the buffer.
Return Value :		addc_offset.
					Return RT_FAIL1: if pointer is pointing to NULL
Algorithm :
*********************************************************************************************************/
short b_limit(Buffer * const pBD) {

	if (pBD == NULL) return RT_FAIL1;

	return (pBD->addc_offset);

}

/*********************************************************************************************************
Purpose :			To get the value of capacity of the buffer.
Author :			Ashish Padaniya
History :			09-02-2018
Called functions :	No function calls
Paramters :			pBD :	pointer to the buffer
Return Value :		RT_FAIL1 : if pBd Pointing toward NULL
					pBD->capacity : Capacity of the buffer
Algorithm :
*********************************************************************************************************/
short b_capacity(Buffer * const pBD)
{
	if (pBD == NULL) return RT_FAIL1;
	return pBD->capacity;
}

/*********************************************************************************************************
Purpose :			set the markc_offset to the mark given in the parameter.
Author :			Ashish Padaniya
History :			09-02-2018
Called functions :	No function calls
Paramters :			pBD :	pointer to the buffer
Return Value :		RT_FAIL1 : if pBd Pointing toward NULL
					Return markc_offset : markc_offset of the pointer pBD
Algorithm :
*********************************************************************************************************/
short b_mark(Buffer * const pBD, short mark) {

	if (pBD == NULL) return RT_FAIL1;

	if (mark >= 0 && mark <= pBD->addc_offset)
	{
		pBD->markc_offset = mark;
		return pBD->markc_offset;
	}

	return RT_FAIL1;
}

/*********************************************************************************************************
Purpose :			To get the mode in which buffer is working.
Author :			Ashish Padaniya
History :			09-02-2018
Called functions :	No function calls
Paramters :			pBD :	pointer to the buffer
Return Value :		mode of the buffer
					if pBD is NULL return RT_FAIL2
Algorithm :
*********************************************************************************************************/
int b_mode(Buffer * const pBD) {

	if (pBD == NULL) return RT_FAIL2;

	return pBD->mode;
}

/*********************************************************************************************************
Purpose :			To find the non-negative value of inc_factor of the buffer.
Author  :			Ashish Padaniya
History :			09-02-2018
Called functions :	No function calls
Paramters :			pBD :	Pointer to the buffer.
Return Value :		return 0x100 if pBD is NULL
Algorithm :
*********************************************************************************************************/
size_t b_incfactor(Buffer * const pBD) {

	if (pBD == NULL) return 0x100;

	return (unsigned char)pBD->inc_factor;
}

/*********************************************************************************************************
Purpose :			Read the character in file character by character and add to buffer. 
Author	:			Ashish Padaniya
History :			09-02-2018
Called functions :	addc() 
					fgetc() 
					feof() 
Paramters :			pBD :	Pointer to the buffer
Return Value :		LOAD_FAIL
					RT_FAIL1 : if the pointer passed in the function parameter is NULL
Algorithm :
*********************************************************************************************************/
int b_load(FILE * const fi, Buffer * const pBD) {

	if (pBD == NULL) return RT_FAIL1;
	if (fi == NULL) return RT_FAIL1;
	
	char sym;
	int count = 0;

	while(1) {
		sym = (char)fgetc(fi);

		if (feof(fi)) break;

		if (b_addc(pBD, sym) == NULL) return RT_FAIL2;

		count++;
	}

	return count;
}

/*********************************************************************************************************
Purpose :			This function helps to know that the buffer is empty or not
					Buffer is considered empty if there is no character loaded in the buffer
					and it means addc_offset is zero.
Author :			Ashish Padaniya
History :			09-02-2018
Called functions :	No function calls
Paramters :			pBD : pointer of buffer about which we want to know that it is empty or not.
Return Value :		Return RT_FAIL1 : if the pBD is pointing to NULL
					Return 1 : If the buffer is empty else return 0
		
Algorithm :
*********************************************************************************************************/
int b_isempty(Buffer * const pBD) {

	if (pBD == NULL) return RT_FAIL1;

	if (pBD->addc_offset == 0)
		return 1;

	return 0;

}

/*********************************************************************************************************
Purpose :			It will provide the value of eob of the buffer.
Author :			Ashish Padaniya
History :			09-02-2018
Called functions :	No function calls
Paramters :			pBD : poiter to the buffer for which i am getting the value of eob
Return Value :	    RT_FAIL1 : if buffer is NULL
				    value of eob of the buffer
Algorithm :
*********************************************************************************************************/
int b_eob(Buffer * const pBD) {

	if (pBD == NULL) return RT_FAIL1;

	return pBD->eob;

}

/*********************************************************************************************************
Purpose :			This function will set the value for eob of the buffer
					if the getc_offset is equal to the addc_offset it means
					buffer reached the end. Then eob will be set to 1 otherwise
					it will set to 0.
Author :			Ashish Padaniya
History :			09-02-2018
Called functions :	No function calls
Paramters :
Return Value :		Return RT_FAIL2: if buffer is pointing to NULL
Return RT_FAIL1 :	if buffer reaches the end.
					Return the character located at the getc_offset of the array
Algorithm :
*********************************************************************************************************/
char b_getc(Buffer * const pBD) {

	if (pBD == NULL) return RT_FAIL2;

	if (pBD->getc_offset == pBD->addc_offset) {
		pBD->eob = 1;
		return RT_FAIL1;
	}
	else {
		pBD->eob = 0;
	}

	pBD->getc_offset++;

	return  (pBD->cb_head[pBD->getc_offset - 1]);

}

/*********************************************************************************************************
Purpose :			To print the character loaded in the buffer and it will check for every possible
					condition like if the buffer is NULL, If buffer is already empty. So, it will print
					empty buffer.
Author :			Ashish Padaniya
History :			09-02-2018
Called functions :	b_getc() : to get the character loaded in the buffer.
Paramters :			pointer to the buffer from which i am printing the data
Return Value :		Return RT_FAIL1 : if pBD pointing to NULL
Return 0 :			if buffer is empty it will ther is no charcter in the buffer
					Return the count of the charcter printed by the function
Algorithm :
*********************************************************************************************************/
int b_print(Buffer * const pBD) {

	if (pBD == NULL) return RT_FAIL1;

	int temp = 0;

	if (b_isempty(pBD) == 1)
	{
		printf("Empty buffer.\n");
		return temp;
	}

	char sym;
	
	while (1) {
		sym = (char)b_getc(pBD);

		if (b_eob(pBD)) break;

		printf("%c",sym);
	}
	printf("\n");
	return temp;
}


/*********************************************************************************************************
Purpose :			This function is to shrink or sometime expand the capacity of the buffer.
					
of buffer.
Author :			Ashish Padaniya
History :			09-02-2018
Called functions :	realloc () : to allocated the memory for the new capacity of the buffer.
Paramters :			pBD : passing the pointer to the buffer in which i am adding the symbol and
					increamenting the capacity.
Return Value :		Return NULL : if buffer is pointing to NULL.
					Return pointer to buffer (pBd) if operation is successful.
Algorithm :
*********************************************************************************************************/
Buffer * b_compact(Buffer * const pBD, char symbol)
{
	char * temp;
	if (pBD == NULL) return NULL;

	if (pBD->addc_offset < 0 || pBD->addc_offset == SHRT_MAX) return NULL;

	pBD->capacity = (pBD->addc_offset + 1) * sizeof(char);
	
	temp = (char *)realloc(pBD->cb_head, pBD->capacity);

	if (temp == NULL) return NULL;

	if (pBD->cb_head != temp) {
		pBD->cb_head = temp;
		pBD->r_flag = SET_R_FLAG;
	}
	pBD->cb_head[pBD->addc_offset++] = symbol;
	return pBD;


}

/*********************************************************************************************************
Purpose :			Return the value of r_flag of the buffer
Author :			Ashish Padaniya
History :			09-02-2018
Called functions :	No function calls
Paramters : pBD :	Pointer to the buffer
Return Value :		Return RT_FAIL1 : if Buffer is pointing to null
					Return r_flag of the buffer.
Algorithm :
*********************************************************************************************************/
char b_rflag(Buffer * const pBD) {

	if (pBD == NULL) return RT_FAIL1;

	return pBD->r_flag;
}

/*********************************************************************************************************
Purpose :			This function will give the value getc_offset by decreasing it by 1
Author :			Ashish Padaniya
History :			09-02-2018
Called functions :	No function calls
Paramters :			pBD : pointer to the buffer
Return Value :		Return RT_FAIL1 : if rum time error occur
					Return getc_offset of the buffer after decrementing it
Algorithm :
*********************************************************************************************************/
short b_retract(Buffer *const pBD) {

	if (pBD == NULL || pBD->getc_offset == 0) return RT_FAIL1;

	pBD->getc_offset--;
	return pBD->getc_offset;
}

/*********************************************************************************************************
Purpose :			This function will help to set the value of getc_offset to the
					current value of markc_offset.
Author :			Ashish Padaniya
History :			09-02-2018
Called functions :	no function calls
Paramters :			pBD: pointer to the buffer
Return Value :		Return RT_FAIL1 : if rum time error occur.
					Return getc_offset after setting the value to markc_offset
Algorithm :
*********************************************************************************************************/
short b_reset(Buffer *const pBD) {

	if (pBD == NULL) return RT_FAIL1;

	pBD->getc_offset = pBD->markc_offset;
	return pBD->getc_offset;
}

/*********************************************************************************************************
Purpose :			To know the value of getc_offset where I am going to call this function
Author :			Ashish Padaniya
History :			09-02-2018
Called functions :	no function calls
Paramters : pBd :	pointer to the buffer
Return Value :		Return RT_FAIL1 : if run time error occur
					Return getc_offset of the buffer which i am passing in the parameter
Algorithm :
*********************************************************************************************************/
short b_getcoffset(Buffer *const pBD) {

	if (pBD == NULL) return RT_FAIL1;

	return pBD->getc_offset;
}

/*********************************************************************************************************
Purpose :			This function will start the buffer to reread again by seting the values of
					get_offset and mark_offset to 0.
Author :			Ashish Padaniya
History :			09-02-2018
Called functions :	no function calls
Paramters :			pBD : pointer to the buffer
Return Value :		Return RT_FAIL1 :if any run time occur
					Return 0 : if it set the buffer to reread again.
Algorithm :
*********************************************************************************************************/
int b_rewind(Buffer *const pBD) {

	if (pBD == NULL) return RT_FAIL1;

	pBD->getc_offset = 0;
	pBD->markc_offset = 0;
	return 0;
}

/*********************************************************************************************************
Purpose :			This function will give the pointer to location of 
					the character which is indicated bythe loc_offset passed in the parameter.
Author :			Ashish Padaniya
History :			09-02-2018
Called functions :	no functions calls
Paramters :			pBD : Pointer to the buffer
Return Value :		Return NULL if the pBD is null or loc_offset is indicated no character
					Return the location of indicated character if everything is valid according to the operation needs.
Algorithm :
*********************************************************************************************************/
char *b_location(Buffer * const pBD, short loc_offset) {

	if (pBD == NULL || loc_offset > pBD->addc_offset) return NULL;

	return  &(pBD->cb_head[loc_offset]);

}
