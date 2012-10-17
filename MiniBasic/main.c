//	
//		main.c
//      
//      Copyright 2012 joaquim <mail@joaquim.org>
//      
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//      
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//      
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//      MA 02110-1301, USA.

#include "stm8s.h"
#include "s65.h"
#include "glib.h"
#include "i2c_ee.h"


#define STARTRUN 1

void DrawMain(void);
u16 ReadADC(u8 port);

static unsigned char buffer[5];
static unsigned char bufferpos = 0;

// VRef for ADC
#define ADCBASE 3300

// ASCII Characters
#define CR	'\r'
#define NL	'\n'
#define DEL	'\177'
#define SPACE   ' '
#define CTRLC	0x03
#define CTRLH	0x08


typedef short unsigned LINENUM;

/***********************************************************/
// Keyword table and constants - the last character has 0x80 added to it
extern unsigned char keywords[] = {
	'L','I','S','T'+0x80,
	'N','E','W'+0x80,
	'R','U','N'+0x80,
	'N','E','X','T'+0x80,
	'L','E','T'+0x80,
	'I','F'+0x80,
	'G','O','T','O'+0x80,
	'F','O','R'+0x80,
	'I','N','P','U','T'+0x80,
	'P','R','I','N','T'+0x80,
	'S','T','O','P'+0x80,
	'C','L','S'+0x80,
	'P','L','O','T'+0x80,
	'O','U','T'+0x80,
	'L','O','A','D'+0x80,
	'S','A','V','E'+0x80,
	'R','E','M'+0x80,
	0
};

#define KW_LIST		0
#define KW_NEW		1
#define KW_RUN		2
#define KW_NEXT		3
#define KW_LET		4
#define KW_IF		5
#define KW_GOTO		6
#define KW_FOR		7
#define KW_INPUT	8
#define KW_PRINT	9
#define KW_STOP		10
#define KW_CLS		11
#define KW_PLOT		12
#define KW_OUT		13
#define KW_LOAD		14
#define KW_SAVE		15
#define KW_REM		16
#define KW_DEFAULT	17

struct stack_for_frame {
	char frame_type;
	char for_var;
	short int terminal;
	short int step;
	unsigned char *current_line;
	unsigned char *txtpos;
};

extern unsigned char func_tab[] = {
	'A','B','S'+0x80,
	'D','I','N'+0x80,
	'A','I','N'+0x80,
	0
};

#define FUNC_ABS	 0
#define FUNC_DIN 	 1
#define FUNC_AIN	 2
#define FUNC_UNKNOWN 3

extern unsigned char to_tab[] = {
	'T','O'+0x80,
	0
};

extern unsigned char step_tab[] = {
	'S','T','E','P'+0x80,
	0
};

extern unsigned char relop_tab[] = {
	'>','='+0x80,
	'<','>'+0x80,
	'>'+0x80,
	'='+0x80,
	'<','='+0x80,
	'<'+0x80,
	0
};

#define RELOP_GE		0
#define RELOP_NE		1
#define RELOP_GT		2
#define RELOP_EQ		3
#define RELOP_LE		4
#define RELOP_LT		5
#define RELOP_UNKNOWN	6

#define VAR_SIZE sizeof(short int) // Size of variables in bytes

static unsigned char memory[1100 + 27];
static unsigned char *txtpos,*list_line;
static unsigned char expression_error;
static unsigned char *tempsp;
static unsigned char *stack_limit;
static unsigned char *program_start;
static unsigned char *program_end;
static unsigned char *stack; // Software stack for things that should go on the CPU stack
static unsigned char *variables_table;
static unsigned char *current_line;
static unsigned char *sp;
#define STACK_FOR_FLAG 'F'
static unsigned char table_index;
static LINENUM linenum;

extern const unsigned char okmsg[]			= "OK";
extern const unsigned char badlinemsg[]		= " Inv. line number";
extern const unsigned char invalidexprmsg[] = " Inv. expression";
extern const unsigned char syntaxmsg[] 		= " Syntax Error";
extern const unsigned char badinputmsg[] 	= " \nBad number";
extern const unsigned char nomemmsg[]		= " Not enough memory!";
extern const unsigned char initmsg[]		= "TinyBasic V0.1";
extern const unsigned char memorymsg[]		= " bytes free.";
extern const unsigned char breakmsg[]		= " break!";


// -------------------------------------------------------------------------
static int inchar(void);
static void outchar(unsigned char c);
static void line_terminator(void);
static short int expression(void);
static unsigned char breakcheck(void);

/***************************************************************************/
static void ignore_blanks(void) {
	while(*txtpos == SPACE) txtpos++;
}

/***************************************************************************/
static void scantable(unsigned char *table) {
	int i = 0;
	ignore_blanks();
	table_index = 0;
	while(1) {
		// Run out of table entries?
		if(table[0] == 0) return;

		// Do we match this character?
		if(txtpos[i] == table[0]) {
			i++;
			table++;
		} else {
			// do we match the last character of keywork (with 0x80 added)? If so, return
			if(txtpos[i]+0x80 == table[0]) {
				txtpos += i+1;  // Advance the pointer to following the keyword
				ignore_blanks();
				return;
			}

			// Forward to the end of this keyword
			while((table[0] & 0x80) == 0) table++;

			// Now move on to the first character of the next word, and reset the position index
			table++;
			table_index++;
			i = 0;
		}
	}
}

/***************************************************************************/
static void pushb(unsigned char b) {
	sp--;
	*sp = b;
}

/***************************************************************************/
static unsigned char popb(void) {
	unsigned char b;
	b = *sp;
	sp++;
	return b;
}

/***************************************************************************/
static void printnum(int num) {
	int digits = 0;

	if(num < 0) {
		num = -num;
		outchar('-');
	}

	do {
		pushb(num%10+'0');
		num = num/10;
		digits++;
	} while (num > 0);

	while(digits > 0) {
		outchar(popb());
		digits--;
	}
}
/***************************************************************************/
static unsigned short testnum(void) {
	unsigned short num = 0;
	ignore_blanks();
	
	while(*txtpos>= '0' && *txtpos <= '9' ) {
		// Trap overflows
		if(num >= 0xFFFF/10) {
			num = 0xFFFF;
			break;
		}

		num = num *10 + *txtpos - '0';
		txtpos++;
	}
	return	num;
}

/***************************************************************************/
unsigned char check_statement_end(void) {
	ignore_blanks();
	return (*txtpos == NL) || (*txtpos == ':');
}


/***************************************************************************/
static unsigned char print_quoted_string(void) {
	int i=0;
	unsigned char delim = *txtpos;
	if(delim != '"' && delim != '\'') return 0;
	txtpos++;

	// Check we have a closing delimiter
	while(txtpos[i] != delim) {
		if(txtpos[i] == NL) return 0;
		i++;
	}

	// Print the characters
	while(*txtpos != delim) {
		outchar(*txtpos);
		txtpos++;
	}
	txtpos++; // Skip over the last delimiter
	ignore_blanks();

	return 1;
}

/***************************************************************************/
static void printmsg(const unsigned char *msg, const unsigned char type) {
	u16 color = BLACK;
	u16 colorb = WHITE;
	if (type == 'E') {
		color = RGB(0xE0, 0x00, 0x00);
		//colorb = RGB(0x90, 0x90, 0x90);
	} else if (type == 'R') {
		color = RGB(0x00, 0xE0, 0x00);
	}
	Write(msg, color, colorb);
    line_terminator();
}

/***************************************************************************/
unsigned char getln(char prompt) {
	u8 charw;
	outchar(prompt);
	txtpos = program_end+sizeof(LINENUM);

	while(1) {
		char c = inchar();
		switch(c) {
			case CR:
			case NL:
                line_terminator();
				// Terminate all strings with a NL
				txtpos[0] = NL;
				return 1;
			case CTRLC:
				return 0;
			case DEL:
				if(txtpos == program_end) break;
				txtpos--;
				BackChar(*txtpos);				
				
				//printmsgNoNL(backspacemsg);
				break;
			default:
				// We need to leave at least one space to allow us to shuffle the line into order
				if(txtpos == sp-2) outchar(' ');
				else {
					txtpos[0] = c;
					txtpos++;
					outchar(c);
				}
		}
	}
}

/***************************************************************************/
static unsigned char *findline(void) {
	unsigned char *line = program_start;
	while(1) {
		if(line == program_end) return line;

		if(((LINENUM *)line)[0] >= linenum) return line;

		// Add the line length onto the current address, to get to the next line;
		line += line[sizeof(LINENUM)];
	}
}

/***************************************************************************/
static void toUppercaseBuffer(void) {
	unsigned char *c = program_end+sizeof(LINENUM);
	unsigned char quote = 0;

	while(*c != NL) {
		// Are we in a quoted string?
		if(*c == quote) quote = 0;
		else if(*c == '"' || *c == '\'') quote = *c;
		else if(quote == 0 && *c >= 'a' && *c <= 'z')
			*c = *c + 'A' - 'a';
		c++;
	}
}

/***************************************************************************/
void printline(void) {
	LINENUM line_num;
	
	line_num = *((LINENUM *)(list_line));
    list_line += sizeof(LINENUM) + sizeof(char);

	outchar(' ');
	// Output the line */
	printnum(line_num);
	outchar(' ');
	while(*list_line != NL) {
		outchar(*list_line);
		list_line++;
	}
	list_line++;
	line_terminator();
}

/***************************************************************************/
static short int expr4(void) {
	short int a = 0;
	unsigned char f = 0;

	if(*txtpos == '0') {
		txtpos++;
		a = 0;
		goto success;
	}

	if(*txtpos >= '1' && *txtpos <= '9') {
		do 	{
			a = a*10 + *txtpos - '0';
			txtpos++;
		} while(*txtpos >= '0' && *txtpos <= '9');
		goto success;
	}

	// Is it a function or variable reference?
	if(txtpos[0] >= 'A' && txtpos[0] <= 'Z') {
		// Is it a variable reference (single alpha)
		if(txtpos[1] < 'A' || txtpos[1] > 'Z') {
			a = ((short int *)variables_table)[*txtpos - 'A'];
			txtpos++;
			goto success;
		}

		// Is it a function with a single parameter
		scantable(func_tab);
		if(table_index == FUNC_UNKNOWN) goto expr4_error;

		f = table_index;
	
		// Pseudo Functions for things that need no parms
		if (f == FUNC_DIN) {
			a = GPIO_ReadInputData(GPIOB);
			goto success;
		}
		
		if (f == FUNC_AIN) {
			a = ReadADC(9);
			goto success;
		}

		if(*txtpos != '(') goto expr4_error;

		txtpos++;
		a = expression();
		if(*txtpos != ')') goto expr4_error;
		txtpos++;
		
		switch(f)
		{
			/*case FUNC_PEEK:
				a =  memory[a];
				goto success;*/
			case FUNC_ABS:
				if(a < 0) a = -a;
				goto success;
		}
	}

	if(*txtpos == '(') {
		txtpos++;
		a = expression();
		if(*txtpos != ')') goto expr4_error;
		txtpos++;
		goto success;
	}

expr4_error:
	expression_error = 1;
success:
	ignore_blanks();
	return a;
}

/***************************************************************************/
static short int expr3(void) {
	short int a,b;

	a = expr4();
	while(1) {
		if(*txtpos == '*') {
			txtpos++;
			b = expr4();
			a *= b;
		} else if(*txtpos == '/') {
			txtpos++;
			b = expr4();
			if(b != 0) a /= b;
			else expression_error = 1;
		}
		else return a;
	}
}

/***************************************************************************/
static short int expr2(void) {
	short int a,b;

	if(*txtpos == '-' || *txtpos == '+') a = 0;
	else a = expr3();

	while(1) {
		if(*txtpos == '-') {
			txtpos++;
			b = expr3();
			a -= b;
		} else if(*txtpos == '+') {
			txtpos++;
			b = expr3();
			a += b;
		} else return a;
	}
}
/***************************************************************************/
static short int expression(void) {
	short int a,b;

	a = expr2();
	// Check if we have an error
	if(expression_error) return a;

	scantable(relop_tab);
	if(table_index == RELOP_UNKNOWN) return a;
	
	switch(table_index)	{
		case RELOP_GE:
			b = expr2();
			if(a >= b) return 1;
			break;
		case RELOP_NE:
			b = expr2();
			if(a != b) return 1;
			break;
		case RELOP_GT:
			b = expr2();
			if(a > b) return 1;
			break;
		case RELOP_EQ:
			b = expr2();
			if(a == b) return 1;
			break;
		case RELOP_LE:
			b = expr2();
			if(a <= b) return 1;
			break;
		case RELOP_LT:
			b = expr2();
			if(a < b) return 1;
			break;
	}
	return 0;
}


void SaveProg(u8 prog) {
	u8 progData = 'S' + 0x80;		
	u16 baseAdd = prog * 1080;
	
	I2C_EE_ByteWrite(&progData, baseAdd);

	baseAdd++;
	
	// Program Start
	list_line = program_start;
	delay_ms(50);

	while(list_line != program_end) {			

		I2C_EE_ByteWrite(list_line, baseAdd);
		delay_ms(5);
		list_line++;
		baseAdd++;
	}

	delay_ms(50);
	progData = 'E' + 0x80;
	I2C_EE_ByteWrite(&progData, baseAdd);
	delay_ms(50);
}

u8 LoadProg(u8 prog) {
	u8 progToLoad[1];		
	u16 baseAdd = prog * 1080;
	
	I2C_EE_BufferRead(progToLoad, baseAdd, sizeof(u8));
	baseAdd++;
	program_end = program_start;
	delay_ms(50);
	if (progToLoad[0] == 'S' + 0x80) {
		
		do {			
			I2C_EE_BufferRead(progToLoad, baseAdd, sizeof(u8));
			baseAdd++;				
			//outchar(progToLoad[0]);
			delay_ms(5);
			if (progToLoad[0] != 'E' + 0x80) {
				*program_end = progToLoad[0];
				program_end++;
			}
			
		} while ((progToLoad[0] != 'E' + 0x80) && (baseAdd < ((prog * 1080) + 1024)));
		current_line = 0;
		return 0;
		
	} else {
		//goto warmstart;
		printnum(progToLoad[0]);
		outchar(':');
		outchar(progToLoad[0]);
		line_terminator();
		return 1;
	}

}

/***************************************************************************/
void RunBasic(void) {
	
	unsigned char *start;
	unsigned char *newEnd;
	unsigned char linelen;
	
	variables_table = memory;
	program_start = memory + 27 * VAR_SIZE;

	program_end = program_start;
	sp = memory + sizeof(memory);  // Needed for printnum
	//printmsg(initmsg);
	printnum(sp - program_end);
	printmsg(memorymsg, 'M');

	if (STARTRUN == 1) {
		if ( LoadProg(0) == 0) {
			current_line = program_start;
			goto execline;
		} else goto syntaxerror;
	}
	
warmstart:
	// this signifies that it is running in 'direct' mode.
	current_line = 0;
	sp = memory + sizeof(memory);  
	printmsg(okmsg, 'R');

prompt:
	while(!getln('>'))
		line_terminator();
	toUppercaseBuffer();

	txtpos = program_end + sizeof(unsigned short);

	// Find the end of the freshly entered line
	while(*txtpos != NL) txtpos++;

	// Move it to the end of program_memory
	{
		unsigned char *dest;
		dest = sp-1;
		while(1) {
			*dest = *txtpos;
			if(txtpos == program_end+sizeof(unsigned short)) break;
			dest--;
			txtpos--;
		}
		txtpos = dest;
	}

	// Now see if we have a line number
	linenum = testnum();
	ignore_blanks();
	if(linenum == 0) goto direct;

	if(linenum == 0xFFFF) goto badline;

	// Find the length of what is left, including the (yet-to-be-populated) line header
	linelen = 0;
	while(txtpos[linelen] != NL) linelen++;
	linelen++; // Include the NL in the line length
	linelen += sizeof(unsigned short) + sizeof(char); // Add space for the line number and line length

	// Now we have the number, add the line header.
	txtpos -= 3;
	*((unsigned short *)txtpos) = linenum;
	txtpos[sizeof(LINENUM)] = linelen;


	// Merge it into the rest of the program
	start = findline();

	// If a line with that number exists, then remove it
	if(start != program_end && *((LINENUM *)start) == linenum) {
		unsigned char *dest, *from;
		unsigned tomove;

		from = start + start[sizeof(LINENUM)];
		dest = start;

		tomove = program_end - from;
		while( tomove > 0) {
			*dest = *from;
			from++;
			dest++;
			tomove--;
		}	
		program_end = dest;
	}

	if(txtpos[sizeof(LINENUM)+sizeof(char)] == NL) // If the line has no txt, it was just a delete
		goto prompt;



	// Make room for the new line, either all in one hit or lots of little shuffles
	while(linelen > 0) {	
		unsigned int tomove;
		unsigned char *from,*dest;
		unsigned int space_to_make;
	
		space_to_make = txtpos - program_end;

		if(space_to_make > linelen) space_to_make = linelen;
		newEnd = program_end+space_to_make;
		tomove = program_end - start;


		// Source and destination - as these areas may overlap we need to move bottom up
		from = program_end;
		dest = newEnd;
		while(tomove > 0) {
			from--;
			dest--;
			*dest = *from;
			tomove--;
		}

		// Copy over the bytes into the new space
		for(tomove = 0; tomove < space_to_make; tomove++) {
			*start = *txtpos;
			txtpos++;
			start++;
			linelen--;
		}
		program_end = newEnd;
	}
	goto prompt;

badline:	
	printmsg(badlinemsg, 'E');
	goto prompt;
	
invalidexpr:
	printmsg(invalidexprmsg, 'E');
	goto prompt;
	
syntaxerror:
	printmsg(syntaxmsg, 'E');
	if(current_line != (void *)0) {
		unsigned char tmp = *txtpos;
		if(*txtpos != NL) *txtpos = '^';
		list_line = current_line;
		printline();
		*txtpos = tmp;
	}
    //line_terminator();
	goto prompt;

nomem:	
	printmsg(nomemmsg, 'E');
	goto warmstart;

run_next_statement:
	while(*txtpos == ':') txtpos++;
	ignore_blanks();
	if(*txtpos == NL) goto execnextline;
	goto interperateAtTxtpos;

direct: 
	txtpos = program_end+sizeof(LINENUM);
	if(*txtpos == NL) goto prompt;

interperateAtTxtpos:
	if(breakcheck()) {
		printmsg(breakmsg, 'E');
		goto warmstart;
	}

	scantable(keywords);
	ignore_blanks();

	switch(table_index)	{
		case KW_LIST:
			goto list;
		case KW_NEW:
			if(txtpos[0] != NL) goto syntaxerror;
			program_end = program_start;
			Cls();
			goto prompt;					
		case KW_RUN:
			current_line = program_start;
			goto execline;
		case KW_NEXT:
			goto next;
		case KW_LET:
			goto assignment;
		case KW_IF:
			{
				short int val;
				expression_error = 0;
				val = expression();
				if(expression_error || *txtpos == NL) goto invalidexpr;
				if(val != 0) goto interperateAtTxtpos;
				goto execnextline;
			}
		case KW_GOTO:
			expression_error = 0;
			linenum = expression();
			if(expression_error || *txtpos != NL) goto invalidexpr;
			current_line = findline();
			goto execline;
		case KW_FOR:
			goto forloop; 
		case KW_INPUT:
			goto input; 
		case KW_PRINT:
			goto print;
		case KW_OUT:
			goto out;
		case KW_STOP:
			// This is the easy way to end - set the current line to the end of program attempt to run it
			if(txtpos[0] != NL) goto syntaxerror;
			current_line = program_end;
			goto execline;
		case KW_CLS:
			goto cls;
		case KW_LOAD:
			goto load;
		case KW_SAVE:
			goto save;
		case KW_PLOT:
			goto plot;		
		case KW_REM:	
			goto execnextline;	// Ignore line completely			
		case KW_DEFAULT:
			goto assignment;
		default:
			break;
	}
	
execnextline:
	if(current_line == (void *)0)		// Processing direct commands?
		goto prompt;
	current_line +=	 current_line[sizeof(LINENUM)];

execline:
  	if(current_line == program_end) // Out of lines to run
		goto warmstart;
	txtpos = current_line+sizeof(LINENUM)+sizeof(char);
	goto interperateAtTxtpos;

input:
	{
		unsigned char isneg=0;
		unsigned char *temptxtpos;
		short int *var;
		ignore_blanks();
		if(*txtpos < 'A' || *txtpos > 'Z') goto syntaxerror;
		var = ((short int *)variables_table)+*txtpos-'A';
		txtpos++;
		if(!check_statement_end()) goto syntaxerror;
again:
		temptxtpos = txtpos;
		if(!getln('?')) goto warmstart;

		// Go to where the buffer is read
		txtpos = program_end + sizeof(LINENUM);
		if(*txtpos == '-') {
			isneg = 1;
			txtpos++;
		}

		*var = 0;
		do {
			*var = *var*10 + *txtpos - '0';
			txtpos++;
		} while(*txtpos >= '0' && *txtpos <= '9');
		ignore_blanks();
		if(*txtpos != NL) {
			printmsg(badinputmsg, 'E');
			goto again;
		}
	
		if(isneg) *var = -*var;

		goto run_next_statement;
	}
	
forloop:
	{
		unsigned char var;
		short int initial, step, terminal;

		if(*txtpos < 'A' || *txtpos > 'Z') goto syntaxerror;
		var = *txtpos;
		txtpos++;
		
		scantable(relop_tab);
		if(table_index != RELOP_EQ) goto syntaxerror;

		expression_error = 0;
		initial = expression();
		if(expression_error) goto invalidexpr;
	
		scantable(to_tab);
		if(table_index != 0) goto syntaxerror;
	
		terminal = expression();
		if(expression_error) goto invalidexpr;
	
		scantable(step_tab);
		if(table_index == 0) {
			step = expression();
			if(expression_error) goto invalidexpr;
		} else step = 1;
		if(!check_statement_end()) goto syntaxerror;

		if(!expression_error && *txtpos == NL) {
			struct stack_for_frame *f;
			if(sp + sizeof(struct stack_for_frame) < stack_limit) goto nomem;

			sp -= sizeof(struct stack_for_frame);
			f = (struct stack_for_frame *)sp;
			((short int *)variables_table)[var-'A'] = initial;
			f->frame_type = STACK_FOR_FLAG;
			f->for_var = var;
			f->terminal = terminal;
			f->step     = step;
			f->txtpos   = txtpos;
			f->current_line = current_line;
			goto run_next_statement;
		}
	}
	goto syntaxerror;

next:
	// Fnd the variable name
	ignore_blanks();
	if(*txtpos < 'A' || *txtpos > 'Z') goto syntaxerror;
	txtpos++;
	if(!check_statement_end()) goto syntaxerror;
	
gosub_return:
	// Now walk up the stack frames and find the frame we want, if present
	tempsp = sp;
	while(tempsp < memory+sizeof(memory)-1) {
		switch(tempsp[0]) {
			
			case STACK_FOR_FLAG:
				// Flag, Var, Final, Step
				if(table_index == KW_NEXT) {
					struct stack_for_frame *f = (struct stack_for_frame *)tempsp;
					// Is the the variable we are looking for?
					if(txtpos[-1] == f->for_var) {
						short int *varaddr = ((short int *)variables_table) + txtpos[-1] - 'A'; 
						*varaddr = *varaddr + f->step;
						// Use a different test depending on the sign of the step increment
						if((f->step > 0 && *varaddr <= f->terminal) || (f->step < 0 && *varaddr >= f->terminal)) {
							// We have to loop so don't pop the stack
							txtpos = f->txtpos;
							current_line = f->current_line;
							goto run_next_statement;
						}
						// We've run to the end of the loop. drop out of the loop, popping the stack
						sp = tempsp + sizeof(struct stack_for_frame);
						goto run_next_statement;
					}
				}
				// This is not the loop you are looking for... so Walk back up the stack
				tempsp += sizeof(struct stack_for_frame);
				break;
			default:
				goto invalidexpr;
		}
	}
	// Didn't find the variable we've been looking for
	goto syntaxerror;
	
assignment:
	{
		short int value;
		short int *var;

		if(*txtpos < 'A' || *txtpos > 'Z') goto syntaxerror;
		var = (short int *)variables_table + *txtpos - 'A';
		txtpos++;

		ignore_blanks();

		if (*txtpos != '=') goto syntaxerror;
		txtpos++;
		ignore_blanks();
		expression_error = 0;
		value = expression();
		if(expression_error) goto invalidexpr;
		// Check that we are at the end of the statement
		if(!check_statement_end()) goto syntaxerror;
		*var = value;
	}
	goto run_next_statement;
	
out:
	{
		short int value;
		// Now get the value to assign
		expression_error = 0;
		value = expression();
		if(expression_error) goto invalidexpr;

		GPIO_Write(GPIOB, value);

		if(!check_statement_end()) goto syntaxerror;
	}
	goto run_next_statement;
	
plot:
	{
		u8 px, py;
		u8 pcolor;
		
		// X
		expression_error = 0;
		px = expression();
		if(expression_error) goto invalidexpr;
		
		// check for a comma
		ignore_blanks();
		if (*txtpos != ',') goto syntaxerror;
		txtpos++;
		ignore_blanks();
		
		// Y
		expression_error = 0;
		py = expression();
		if(expression_error) goto invalidexpr;
		
		// check for a comma
		ignore_blanks();
		if (*txtpos != ',') goto syntaxerror;
		txtpos++;
		ignore_blanks();
		
		// Color
		expression_error = 0;
		pcolor = expression();
		if(expression_error) goto invalidexpr;
			
		if (pcolor == 1)
			LCD_DrawPixel(px, py + 12, BLACK);
		else
			LCD_DrawPixel(px, py + 12, WHITE);
		
		if(!check_statement_end()) goto syntaxerror;
	}
	goto run_next_statement;
	
cls:
	Cls();	
	goto run_next_statement;
	
load:
	{
		u8 progLoad;
		expression_error = 0;
		progLoad = expression();
		if(expression_error) goto invalidexpr;
		
		if ( LoadProg(progLoad) == 0 ) {
			//goto warmstart;
			current_line = program_start;
			goto execline;
		}
		else goto syntaxerror;
	}
	
save:
	{
		u8 progLoad;
		expression_error = 0;
		progLoad = expression();
		if(expression_error) goto invalidexpr;
		
		SaveProg(progLoad);
		goto warmstart;
	}
	
list:
	linenum = testnum(); // Retuns 0 if no line found.

	// Should be EOL
	if(txtpos[0] != NL) goto syntaxerror;

	// Find the line
	list_line = findline();
	while(list_line != program_end) printline();
	goto warmstart;

print:
	// If we have an empty list then just put out a NL
	if(*txtpos == ':' ) {
        line_terminator();
		txtpos++;
		goto run_next_statement;
	}
	if(*txtpos == NL) {
		goto execnextline;
	}

	while(1) {
		ignore_blanks();
		if(print_quoted_string()) {
			;
		}
		else if(*txtpos == '"' || *txtpos == '\'') goto syntaxerror;
		else {
			short int e;
			expression_error = 0;
			e = expression();
			if(expression_error) goto invalidexpr;
			printnum(e);
		}

		// At this point we have three options, a comma or a new line
		if(*txtpos == ',')
			txtpos++;	// Skip the comma and move onto the next
		else if(txtpos[0] == ';' && (txtpos[1] == NL || txtpos[1] == ':')) {
			txtpos++; // This has to be the end of the print - no newline
			break;
		} else if(check_statement_end()) {
			line_terminator();	// The end of the print statement
			break;
		} else goto syntaxerror;	
	}
	goto run_next_statement;
}


/***************************************************************************/
static void line_terminator(void) {
  	outchar(NL);
	//outchar(CR);
}


/***********************************************************/
static unsigned char breakcheck(void) {
	u8 c,i;
	if (bufferpos > 0) {
		c = buffer[0];
		
		for(i=1; i < 5; i++) {
			buffer[i - 1] = buffer[i];
		}
		bufferpos--;
		if (c == CTRLC) return 1;
		else return 0;
	}
	return 0;
}
/***********************************************************/
static int inchar() {
	u8 c,i;
	while(1) {
		if (bufferpos > 0) {
			c = buffer[0];
			
			for(i=1; i < 5; i++) {
				buffer[i - 1] = buffer[i];
			}
			bufferpos--;
			return c;
		}
	}
}

/***********************************************************/
static void outchar(unsigned char c) {
	DrawChar(c, BLACK, WHITE);	
	// No echo, output to LCD
	//UART2_SendData8(c);
}
/* ------------------------------------------------------------------------- */
/*  Main OS Function  */
/* ------------------------------------------------------------------------- */

@far @interrupt void UART2_RX_IRQHandler(void) {
	u8 c;
	if (UART2_GetFlagStatus(UART2_FLAG_RXNE) != RESET) {			
		
		/* Read one byte from the receive data register */
		c = UART2_ReceiveData8();
		
		if (bufferpos <= 5) {
			buffer[bufferpos] = c;
			bufferpos++;
		}		
	}
}

/* ------------------------------------------------------------------------- */

void CheckBat(void);

@far @interrupt void Tim2Update_isr(void) {

	TIM2_ClearFlag(TIM2_FLAG_UPDATE);
	
	// Blink Debug LED
	GPIO_WriteReverse(GPIOD, GPIO_PIN_0);
	
	CheckBat();
}

/* ------------------------------------------------------------------------- */

u16 ReadADC(u8 port) {
	u8 i;
	u16 value = 0;
	
	ADC1_DeInit();
	ADC1_Init(ADC1_CONVERSIONMODE_SINGLE, port, ADC1_PRESSEL_FCPU_D2,
		ADC1_EXTTRIG_TIM, DISABLE, ADC1_ALIGN_RIGHT, port, DISABLE);
		
	for( i = 0; i < 16; i++)
	{
		ADC1_StartConversion();
		while(!ADC1_GetFlagStatus(ADC1_FLAG_EOC));
		value += ADC1_GetConversionValue(); // read ADC conversion data, the first low, then high
	}
	
	ADC1_DeInit();

	return ADCBASE / 1024 * (value >> 4);
}

/* ------------------------------------------------------------------------- */


void InitIO(void) {

	// -------------------------------------------------------------------------
	//Debug LED	
	// Reset ("de-initialise") GPIO port D.
	GPIO_DeInit(GPIOD);
	// Initialise pin 0 of port D by setting it as:
	// - an output pin,
	// - using a push-pull driver,
	// - at a low logic level (0V), and
	// - 10MHz.
	GPIO_Init(GPIOD, GPIO_PIN_0, GPIO_MODE_OUT_PP_LOW_FAST);
	
	// -------------------------------------------------------------------------
	//IO Port	
	// Init Pin Port B - Data D0..D7
	GPIO_Init(GPIOB, 
			GPIO_PIN_0 | GPIO_PIN_1 | 
			GPIO_PIN_2 | GPIO_PIN_3 | 
			GPIO_PIN_4 | GPIO_PIN_5 | 
			GPIO_PIN_6 | GPIO_PIN_7, GPIO_MODE_OUT_PP_LOW_FAST); // D0..D7
	
	// -------------------------------------------------------------------------
	//ADC
	/*  Init GPIO for ADC1 */
    // Batt ADC
	GPIO_Init(GPIOE, GPIO_PIN_7, GPIO_MODE_IN_FL_NO_IT);	
	GPIO_Init(GPIOE, GPIO_PIN_6, GPIO_MODE_IN_FL_NO_IT);
	
	
	// -------------------------------------------------------------------------
	//UART for BlueTooth communication	
	UART2_DeInit();
	/* UART2 configuration ------------------------------------------------------*/
	/* UART2 configured as follow:
		- BaudRate = 9600 baud  
		- Word Length = 8 Bits
		- One Stop Bit
		- Even parity
		- Receive and transmit enabled
		- UART2 Clock disabled
	*/
	/* Configure the UART2 */
	UART2_Init((u32)9600, UART2_WORDLENGTH_8D, UART2_STOPBITS_1, UART2_PARITY_NO, 
		UART2_SYNCMODE_CLOCK_DISABLE, UART2_MODE_TXRX_ENABLE);
	
	/* Set Prescaler*/
    //UART2_SetPrescaler(0x1);

    //UART2_IrDAConfig(UART2_IRDAMODE_NORMAL);

    //UART2_IrDACmd(ENABLE);
	
	// -------------------------------------------------------------------------
	//Timer
	TIM2_DeInit ();
	TIM2_TimeBaseInit (TIM2_PRESCALER_8192 ,32000); // = 10Hz 16000000/512/3125 - 31250
	TIM2_ITConfig (TIM2_IT_UPDATE, ENABLE);
	TIM2_Cmd (ENABLE);
	
	// -------------------------------------------------------------------------
	//I2C for EEPROM	
    I2C_DeInit();
    /* Initialize the I2C */
    I2C_EEInit();
	
	UART2_ITConfig(UART2_IT_RXNE, ENABLE);
}

/* ------------------------------------------------------------------------- */

void CheckBat(void) {
	unsigned char bat[5];
	u16 BattVolt = ReadADC(8) * 3;
		
	u16tostr(BattVolt, bat, 0);
	if ( BattVolt > 3600) {
		Puts(150, 2, bat, RGB(0,255,0), BLACK);
	} else if ( BattVolt > 3300) {
		Puts(150, 2, bat, RGB(255,255,0), BLACK);
	} else {
		Puts(150, 2, bat, RGB(255,0,0), BLACK);
	}
}

/* ------------------------------------------------------------------------- */

void DrawMain(void) {
	
	Fill(RGB(255,255,255));
	
	// Barra de Topo
	FillRect(0, 0, _LCD_WIDTH - 1, 10, BLACK);
	
	// Icons
	DrawIcon(135,0, batt); 
	
	Puts(5, 2, initmsg, WHITE, BLACK);
	
	CheckBat();
}

/* ------------------------------------------------------------------------- */

int main(void) {

	// Initialise the clock to have a /1 prescaler and use the external crystal clock source for accuracy.
	CLK_DeInit();
	CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);
	CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSE, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE);
	
	enableInterrupts();
	
	InitIO();
	
	GLib_Init();
	
	DrawMain();
	
	// Infinite loop.
	//for(;;) {
	RunBasic();
	//}
}