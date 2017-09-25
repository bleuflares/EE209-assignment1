/**********************
* EE209 Assignment 1 *
**********************/
/*20130242*/
/*박기성 Ki Sung Park*/
/* client.c */

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_NUM_NAME_CHARS 63
#define MAX_NUM_ID_CHARS 63
#define MAX_NUM_DIGITS   10

/*--------------------------------------------------------------------*/
/* Pre-defined error messages */
#define ERR_UNDEF_CMD   "ERROR: Undefined Command\n"
#define ERR_UNDEF_OPT   "ERROR: Undefined Option\n"
#define ERR_NEED_OPT    "ERROR: Need More Option\n"
#define ERR_SAME_OPT    "ERROR: Multiple Same Options\n"
#define ERR_AMBIG_ARG   "ERROR: Ambiguous Argument\n"
#define ERR_INVALID_ARG "ERROR: Invalid Option Argument\n"

/*--------------------------------------------------------------------*/
enum { FALSE = 0, TRUE };

typedef enum {
	C_EXIT,       /* exit */
	C_REG,        /* register */
	C_UNREG,      /* unregister */
	C_FIND,       /* find */
	C_FAIL,       /* failure */
	C_EOF         /* end of file */
} Command_T;

/*--------------------------------------------------------------------*/
/* Check whether an exit command valid.                               */
/* Input: no argument                                                 */
/* Return value: TRUE(valid) or FALSE(invalid)                        */
/*--------------------------------------------------------------------*/
static int
ValidateExitCommand(void)
{
	int c;

	/* only blank command is allowed */
	while ((c = getchar()) != EOF && c != '\n' && isspace(c))
		;
	if (c == EOF)
		exit(0);
	if (c != '\n' && c != EOF) {
		fprintf(stderr, "%s", ERR_UNDEF_OPT);

		/* eat the line */
		while ((c = getchar()) != EOF && c != '\n')
			;
		if (c == EOF)
			exit(0);
		return FALSE;
	}
	return TRUE;
}
/*--------------------------------------------------------------------*/
/* Check whether a reg(register) command valid.                       */
/* Input: no argument                                                 */
/* Return value: TRUE(valid) or FALSE(invalid)                        */
/*--------------------------------------------------------------------*/
static int
ValidateRegisterCommand(void)
{
  typedef enum { TF, NPI, N, P, I }DFAState;
  DFAState state = TF;
  int c;//instant variable for input character
  int NF = 0;//instnat variable set when Name option is get
  int PF = 0;//instant variable set when Purchase option is get
  int IF = 0;//instant variable set when ID option is get 
  int count = 0;//instant variable counting the number of characters
  int quote = 0;//instant variable set when more than one quote is get
  int end = 0;//instant variable set when the input reaches \n or EOF
  int instate = 1;//Instant variable set when the user is currently in the state
  while (instate){
    switch (state){
      //State TF checking whether the input is valid
      //moves to state NPI when more option is added
      //otherwise, determine whether there is an error or not
    case TF:
      instate = 0;
      if (end == 1){

	//all options valid
		  if (NF == 1 && PF == 1 && IF == 1){
			  if (c == EOF)
				  exit(0);
			  return TRUE;
		  }
	// lack of option
	else{
	  fprintf(stderr, "%s", ERR_NEED_OPT);
	  if (c == EOF)
		  exit(0);
	  return FALSE;
	}
      }
      while ((c = getchar()) != EOF && c != '\n' && isspace(c))
	;
      if (c == EOF || c == '\n'){
	
	//all options valid
		  if (NF == 1 && PF == 1 && IF == 1){
			  if (c == EOF)
				  exit(0);
			  return TRUE;
		  }
	//lack of option
	else
	  fprintf(stderr, "%s", ERR_NEED_OPT);
      }
      //get a new input
      else if (c == '-'){
	state = NPI;
	instate = 1;
      }
      //other inappropriate input for option
      else
	fprintf(stderr, "%s", ERR_UNDEF_OPT);
      break;
      //state NPI determines the option
      //it directs to the state N when option n and at least one space is input
      //it directs to the state I when option i and at least one space is input
      //it directs to the state P when option p and at least one space is input
      //if other values are input, it returns error
    case NPI:
      instate = 0;
      c = getchar();
      if (c == 'n'){
	if (NF == 1){
	  while ((c = getchar()) != EOF && c != '\n' && isspace(c))
	    ;
	  //option n not finished
	  if (c == EOF || c == '\n')
	    fprintf(stderr, "%s", ERR_UNDEF_OPT);
	  //option n already got
	  else
	    fprintf(stderr, "%s", ERR_SAME_OPT);
	}
	// space exists after the option move to state N
	else if ((c = getchar()) != EOF && c != '\n' && isspace(c)){
	  state = N;
	  instate = 1;
	}
	//undefined option n
	else
	  fprintf(stderr, "%s", ERR_UNDEF_OPT);
      }
      else if (c == 'p'){
	if (PF == 1){
	  while ((c = getchar()) != EOF && c != '\n' && isspace(c))
	    ;
	  //option n not finished
	  if (c == EOF || c == '\n')
	    fprintf(stderr, "%s", ERR_UNDEF_OPT);
	  //option n already got
	  else
	    fprintf(stderr, "%s", ERR_SAME_OPT);
	}
	//space exists after the option move to state P
	else if ((c = getchar()) != EOF && c != '\n' && isspace(c)){
	  state = P;
	  instate = 1;
	}
	//undefined option p
	else
	  fprintf(stderr, "%s", ERR_UNDEF_OPT);
      }
      else if (c == 'i'){
	
	if (IF == 1){
	  while ((c = getchar()) != EOF && c != '\n' && isspace(c))
	    ;
	  //option i not finished
	  if (c == EOF || c == '\n')
	    fprintf(stderr, "%s", ERR_UNDEF_OPT);
	  //option i already exists
	  else
	    fprintf(stderr, "%s", ERR_SAME_OPT);
	}
	//space after option i move to state I
	else if ((c = getchar()) != EOF && c != '\n' && isspace(c)){
	  state = I;
	  instate = 1;
	}
	//undefined option p
	else
	  fprintf(stderr, "%s", ERR_UNDEF_OPT);
      }
      //other undefined option
      else
	fprintf(stderr, "%s", ERR_UNDEF_OPT);
      break;
      //state N checks whether the input value for name is valid
    case N:
      instate = 0;
      count = 0;
      while ((c = getchar()) != EOF && c != '\n' && isspace(c))
	;
      
      if (isalpha(c) || c == '-' || c == '_' || c == '.'){
	count++;
	while(isalpha(c)||c=='-'||c=='_'||c=='.'||c=='\''||c=='\\'){
	  if (c == '\\'){
	    c = getchar();
	    //argument not allowed for name as an input
	    if(!(isalpha(c)||c=='-'||c=='_'||c=='.'||c=='\''||c==' '))
	      fprintf(stderr, "%s", ERR_INVALID_ARG);
	  }
	  c = getchar();
	  count++;
	}
	//more than the Max input allowed
	if (--count > MAX_NUM_NAME_CHARS)
	  fprintf(stderr, "%s", ERR_INVALID_ARG);
	//valid argument as an input move to state TF
	else if (isspace(c) || c == EOF){
	  NF = 1;
	  state = TF;
	  instate = 1;
	  if (c == EOF || c == '\n')
	    end = 1;
	}
	else
	  fprintf(stderr, "%s", ERR_INVALID_ARG);
      }
      
      else if (c == '\''){
		c = getchar();
		if ( (c == EOF|| c == '\n')){
	  
	  //quote not finished
			fprintf(stderr, "%s", ERR_INVALID_ARG);
			if (c == EOF)
				exit(0);
			return FALSE;
	}
	while(isalpha(c)||c=='-'||c=='_'||c=='.'||c==' '||c=='\''||c== '\\'){
	  if (c == '\''){
	    c = getchar();
	    count--;
	    quote++;
	    break;
	  }
	  else if (c == '\\'){
	    c = getchar();
	    if (!(isalpha(c)||c=='-'||c=='_'||c=='.'||c=='\''||c==' '))
	      //argument not allowed for the name as an input
	      fprintf(stderr, "%s", ERR_INVALID_ARG);
	  }
	  c = getchar();
	  count++;
	}
	//length of argument longer than the max number allowed
	if (count > MAX_NUM_NAME_CHARS || count<0)
	  fprintf(stderr, "%s", ERR_INVALID_ARG);
	//valid argument as an input more to state TF
	else if ((isspace(c) || c == EOF)&&quote==1){
	  NF = 1;
	  state = TF;
	  instate = 1;
	  if (c == EOF || c == '\n')
	    end = 1;
	}
	//invalid argument
	else
	  fprintf(stderr, "%s", ERR_INVALID_ARG);
      }
      
      else if (c == '\\'){
	c = getchar();
	//argument not allowed for the name as an input
	if(!(isalpha(c)||c=='-'||c=='_'||c=='.'||c=='\''||c==' '))
	  fprintf(stderr, "%s", ERR_INVALID_ARG);
	else{
	  c = getchar();
	  //valid argument as an input move to state TF
	  if (isspace(c) || c == EOF){
	    NF = 1;
	    state = TF;
	    instate = 1;
	    if (c == EOF || c == '\n')
	      end = 1;
	  }
	  else{
	    if (c == '\'')
	      quote = 1;
	    count++;
	    state = N;
	    instate = 1;
	    ungetc(c, stdin);
	  }
	}
      }
      //no argument after the option
      else if (c == EOF || c == '\n')
	fprintf(stderr, "%s", ERR_UNDEF_OPT);
      //invalid argument
      else
	fprintf(stderr, "%s", ERR_INVALID_ARG);
      break;
      //state I checks whether the input value for ID is valid
    case I:
      instate = 0;
      count = 0;
      while ((c = getchar()) != EOF && c != '\n' && isspace(c))
	;
      if (isalpha(c) || isdigit(c) || c == '-' || c == '_' || c == '.'){
	while (isalpha(c)||isdigit(c)||c=='-'||c=='_'||c=='.'){
	  c = getchar();
	  count++;
	}
	//the length of argument longer than the max length
	if (count > MAX_NUM_ID_CHARS)
	  fprintf(stderr, "%s", ERR_INVALID_ARG);
	//valid argument as an input move to TF state
	else if (isspace(c)||c==EOF){
	  IF = 1;
	  state = TF;
	  instate = 1;
	  if (c == EOF || c == '\n')
	    end = 1;
	}
	else
	  fprintf(stderr, "%s", ERR_INVALID_ARG);
      }
      //option finished without an argument
      else if (c == EOF || c == '\n')
	fprintf(stderr, "%s", ERR_UNDEF_OPT);
      //other invalid argument
      else
	fprintf(stderr, "%s", ERR_INVALID_ARG);
      break;
      //state P checks whether the input value for Purchase is valid
    case P:
      instate = 0;
      count = 0;
      while ((c = getchar()) != EOF && c != '\n' && isspace(c))
	;
      if (isdigit(c) && c != '0'){
	while (isdigit(c)){
	  c = getchar();
	  count++;
	}
	//argument long than the max length
	if (count > MAX_NUM_DIGITS)
	  fprintf(stderr, "%s", ERR_INVALID_ARG);
	//valid argument input move to state TF
	else if (isspace(c)||c==EOF){
	  PF = 1;
	  state = TF;
	  instate = 1;
	  if (c == EOF || c == '\n')
	    end = 1;
	}
	//invalid input argument
	else
	  fprintf(stderr, "%s", ERR_INVALID_ARG);
      }
      else if (c == '0'){
	c = getchar();
	//valid input argument 0
	if (isspace(c)||c==EOF){
	  PF = 1;
	  state = TF;
	  instate = 1;
	  if (c == EOF || c == '\n')
	    end = 1;
	}
	//invalid argument as more argument after 0
	else
	  fprintf(stderr, "%s", ERR_INVALID_ARG);
      }
      //no argument input
      else if (c == EOF || c == '\n')
	fprintf(stderr, "%s", ERR_UNDEF_OPT);
      //other invalid arguments
      else
	fprintf(stderr, "%s", ERR_INVALID_ARG);
      break;
    }
  }
  //no more input for the line
  if (c == EOF || c == '\n'){
    if (c == EOF)
      exit(0);
    return FALSE;
  }
  //Eat the line and return
  while ((c = getchar()) != EOF && c != '\n')
    ;
  if (c == EOF)
    exit(0);
  return FALSE;
}
/*--------------------------------------------------------------------*/
/* Check whether an unreg(unregister) or a find(search) command valid.*/
/* Input: no argument                                                 */
/* Return value: TRUE(valid) or FALSE(invalid)                        */
/*--------------------------------------------------------------------*/
static int
ValidateUnregisterOrFindCommand(void)
{
  typedef enum { TF, NI, N, I }DFAState;
  DFAState state = TF;
  int c;
  int NF = 0;
  int IF = 0;
  int count = 0;
  int end = 0;
  int instate = 1;
  int quote = 0;
  while (instate){
    switch (state){
      //State TF checking whether the input is valid
      //moves to state NPI when more option is added
      //otherwise, determine whether there is an error or not
    case TF:
      instate = 0;
      if (end == 1){
	//all option valid
	if ((NF == 1) ^ (IF == 1)){
		if (c == EOF)
			exit(0);
	  return TRUE;
	}
	//too much option
	else if (NF == 1 && IF == 1)
	  fprintf(stderr, "%s", ERR_AMBIG_ARG);
	//lack of option
	else
	  fprintf(stderr, "%s", ERR_NEED_OPT);
	if (c == EOF)
	  exit(0);
	return FALSE;
      }
      while ((c = getchar()) != EOF && c != '\n' && isspace(c))
	;
      if (c == EOF || c == '\n'){
	//all option valid
	if ((NF == 1) ^ (IF == 1)){
		if (c == EOF)
			exit(0);
		return TRUE;
	}
	//too much option
	else if (NF == 1 && IF == 1)
	  fprintf(stderr, "%s", ERR_AMBIG_ARG);
	//lack of option
	else if (NF == 0 && IF == 0)
	  fprintf(stderr, "%s", ERR_NEED_OPT);
	if (c == EOF)
		exit(0);
	return FALSE;
      }
      //too much option
      else if (NF == 1 && IF == 1)
	fprintf(stderr, "%s", ERR_AMBIG_ARG);
      //input - move to state NI
      else if (c == '-'){
	state = NI;
	instate = 1;
      }
      //undefined option input
      else
	fprintf(stderr, "%s", ERR_UNDEF_OPT);
      break;
      //state NI determines the option
      //it directs to the state N when option n and at least one space is input
      //it directs to the state I when option i and at least one space is input
      //if other values are input, it returns error
	case NI:
		instate = 0;
		c = getchar();
		if (c == 'n'){
			if (NF == 1){
				while ((c = getchar()) != EOF && c != '\n' && isspace(c))
					;
				//option n not finished
				if (c == EOF || c == '\n')
					fprintf(stderr, "%s", ERR_UNDEF_OPT);
				//option n already got
				else
					fprintf(stderr, "%s", ERR_SAME_OPT);
			}
			else if (IF ==1)
			{
				while ((c = getchar()) != EOF && c != '\n' && isspace(c))
					;
				//option n not finished
				if (c == EOF || c == '\n')
					fprintf(stderr, "%s", ERR_UNDEF_OPT);
				//option i already got
				else
					fprintf(stderr, "%s", ERR_AMBIG_ARG);
			}
			// space exists after the option move to state N
			else if ((c = getchar()) != EOF && c != '\n' && isspace(c)){
				state = N;
				instate = 1;
			}
			//undefined option n
			else
				fprintf(stderr, "%s", ERR_UNDEF_OPT);
		}

		else if (c == 'i'){

			if (IF == 1){
				while ((c = getchar()) != EOF && c != '\n' && isspace(c))
					;
				//option i not finished
				if (c == EOF || c == '\n')
					fprintf(stderr, "%s", ERR_UNDEF_OPT);
				//option i already exists
				else
					fprintf(stderr, "%s", ERR_SAME_OPT);
			}
			else if (NF==1)
			{
				while ((c = getchar()) != EOF && c != '\n' && isspace(c))
					;
				//option i not finished
				if (c == EOF || c == '\n')
					fprintf(stderr, "%s", ERR_UNDEF_OPT);
				//option n already exists
				else
					fprintf(stderr, "%s", ERR_AMBIG_ARG);
			}
			//space after option i move to state I
			else if ((c = getchar()) != EOF && c != '\n' && isspace(c)){
				state = I;
				instate = 1;
			}
			//undefined option i
			else
				fprintf(stderr, "%s", ERR_UNDEF_OPT);
		}
		//other undefined option
		else
			fprintf(stderr, "%s", ERR_UNDEF_OPT);
		break;
		//state N checks whether the input value for name is valid
    case N:
      instate = 0;
      count = 0;
      while ((c = getchar()) != EOF && c != '\n' && isspace(c))
	;
      
      if (isalpha(c) || c == '-' || c == '_' || c == '.'){
	count++;
	while(isalpha(c)||c=='-'||c=='_'||c=='.'||c=='\''||c=='\\'){
	  if (c == '\\'){
	    c = getchar();
	    //argument not allowed for name as an input
	    if(!(isalpha(c)||c=='-'||c=='_'||c=='.'||c=='\''||c==' '))
	      fprintf(stderr, "%s", ERR_INVALID_ARG);
	  }
	  c = getchar();
	  count++;
	}
	//argument longer than the max length
	if (--count > MAX_NUM_NAME_CHARS)
	  fprintf(stderr, "%s", ERR_INVALID_ARG);
	//valid argument as an input move to state TF
	else if (isspace(c) || c == EOF){
	  NF = 1;
	  state = TF;
	  instate = 1;
	  if (c == EOF || c == '\n')
	    end = 1;
	}
	else
	  fprintf(stderr, "%s", ERR_INVALID_ARG);
      }
      
      else if (c == '\''){
	c = getchar();
	if (quote == 0 && (c == EOF || c == '\n')){
	  //quote not finished
	  fprintf(stderr, "%s", ERR_INVALID_ARG);
	  if (c == EOF)
		  exit(0);
	  return FALSE;
	}
	while(isalpha(c)||c=='-'||c =='_'||c=='.'||c==' '||c=='\''||c=='\\'){
	  if (c == '\''){
	    c = getchar();
	    count--;
	    quote++;
	    break;
	  }
	  else if (c == '\\'){
	    c = getchar();
	    //argument not allowed for the name as an input
	    if(!(isalpha(c)||c=='-'||c=='_'||c=='.'||c=='\''||c==' '))
	      fprintf(stderr, "%s", ERR_INVALID_ARG);
	  }
	  c = getchar();
	  count++;
	}
	//length of argument longer than the max number allowed
	if (count > MAX_NUM_NAME_CHARS || count<0)
	  fprintf(stderr, "%s", ERR_INVALID_ARG);
	//valid argument as an input more to state TF
	else if ((isspace(c) || c == EOF) && quote == 1){
	  NF = 1;
	  state = TF;
	  instate = 1;
	  if (c == EOF || c == '\n')
	    end = 1;
	}
	//argument not allowed as an input
	else
	  fprintf(stderr, "%s", ERR_INVALID_ARG);
      }
      
      else if (c == '\\'){
	c = getchar();
	//argument not allowed for the name as an input
	if (!(isalpha(c)||c=='-'||c=='_'||c=='.'||c=='\''||c==' '))
	  fprintf(stderr, "%s", ERR_INVALID_ARG);
	else{
	  c = getchar();
	  //valid argument as an input move to state TF
	  if (isspace(c) || c == EOF){
	    NF = 1;
	    state = TF;
	    instate = 1;
	    if (c == EOF || c == '\n')
	      end = 1;
	  }
	  else{
	    if (c == '\'')
	      quote = 1;
	    count++;
	    state = N;
	    instate = 1;
	    ungetc(c, stdin);
	  }
	}
      }
      //no argument after the option
      else if (c == EOF || c == '\n')
	fprintf(stderr, "%s", ERR_UNDEF_OPT);
      //argument not allowed as an input
      else
	fprintf(stderr, "%s", ERR_INVALID_ARG);
      break;
      //state I checks whether the input value for ID is valid
    case I:
      instate = 0;
      count = 0;
      while((c=getchar()) != EOF && c != '\n' && isspace(c))
	;
      if (isalpha(c) || isdigit(c) || c == '-' || c == '_' || c == '.'){
	while (isalpha(c) || isdigit(c) || c == '-' || c == '_' || c == '.'){
	  c = getchar();
	  count++;
	}
	//the length of argument longer than the max length
	if (count > MAX_NUM_ID_CHARS)
	  fprintf(stderr, "%s", ERR_INVALID_ARG);
	//valid argument as an input move to TF state
	else if (isspace(c) || c == EOF){
	  IF = 1;
	  state = TF;
	  instate = 1;
	  if (c == EOF || c == '\n')
	    end = 1;
	}
	//argument not allowed as an input
	else
	  fprintf(stderr, "%s", ERR_INVALID_ARG);
      }
      //option finished without an argument
      else if (c == EOF || c == '\n')
	fprintf(stderr, "%s", ERR_UNDEF_OPT);
      //argument not allowed as an input
      else
	fprintf(stderr, "%s", ERR_INVALID_ARG);
      break;
      
    }
  }
  //no more input for the line
  if (c == EOF || c == '\n'){
    if (c == EOF)
      exit(0);
    return FALSE;
  }
  //Eat the line and return
  while ((c = getchar()) != EOF && c != '\n')
    ;
  if (c == EOF)
    exit(0);
  return FALSE;
  /* TODO: Implement this function. */
}
/*--------------------------------------------------------------------*/
/* Read the first word, and figure out and return the command type.   */
/* Input: no argument                                                 */
/* Return value: Command_T value                                      */
/*  - In case of an error, it eats the entire line and returns C_FAIL */
/*  - In case there's no more input (EOF), it returns C_EOF           */
/*--------------------------------------------------------------------*/
static Command_T
GetCommandType(void)
{
	Command_T type = C_FAIL;
	const char *cmds[] = {
		"exit",   /* exit */
		"reg",    /* reg */
		"unreg",  /* unreg */
		"find",   /* find */
	};
	int i, len;
	int c;

	/* eat space */
	while ((c = getchar()) != EOF && c != '\n' && isspace(c))
		;

	switch (c) {
	case '\n':return C_FAIL;  /* no command */
	case EOF: return C_EOF;   /* no more input */
	case 'e': type = C_EXIT;  break;
	case 'r': type = C_REG;   break;
	case 'u': type = C_UNREG; break;
	case 'f': type = C_FIND;  break;
	default:
		fprintf(stderr, "%s", ERR_UNDEF_CMD);
		goto EatLineAndReturn;
	}

	/* see the rest of the command chars actually match */
	len = strlen(cmds[type]);
	for (i = 1; i < len; i++) {
		c = getchar();
		if (c == '\n' || c == EOF) {   /* line finished too early */
			fprintf(stderr, "%s", ERR_UNDEF_CMD);
			return (c == EOF) ? C_EOF : C_FAIL;
		}
		if (c != cmds[type][i]) {    /* wrong command */
			fprintf(stderr, "%s", ERR_UNDEF_CMD);
			goto EatLineAndReturn;
		}
	}

	/* check the following character of a command */
	c = getchar();
	if (c != '\n' && isspace(c)) {
		return type;
	}
	else if (c == '\n' || c == EOF) {
		/* only exit can be followed by '\n' */
		if (type != C_EXIT)
			fprintf(stderr, "%s", ERR_NEED_OPT);
		if (c == EOF)
			return C_EOF;
		else
			ungetc(c, stdin);
		if (type == C_EXIT)
			return type;
	}
	else {
		fprintf(stderr, "%s", ERR_UNDEF_CMD);
	}

EatLineAndReturn:
	while ((c = getchar()) != EOF && (c != '\n'))
		;
	return (c == EOF) ? C_EOF : C_FAIL;
}
/*--------------------------------------------------------------------*/
int
main(int argc, const char *argv[])
{
	Command_T command;
	int res;
	printf("======================================\n" \
		"  Customer Manager Program\n" \
		"======================================\n\n");

	/* start prompt */
	while (TRUE) {
	
		printf("\n> ");
		/* check command type */
		command = GetCommandType();

		/* command validation */
		switch (command) {
		case C_EOF:
			return 0;
		case C_FAIL:
			res = FALSE;
			break;
		case C_EXIT:
			res = ValidateExitCommand();
			break;
		case C_REG:
			res = ValidateRegisterCommand();
			break;
		case C_FIND:
			res = ValidateUnregisterOrFindCommand();
			break;
		case C_UNREG:
			res = ValidateUnregisterOrFindCommand();
			break;
		default:
			assert(0); /* can't reach here */
			break;
		}

		if (res == FALSE) {

			/* validation fail */
			continue;
		}

		/* command functionalities */
		switch (command) {
		case C_EXIT:
			exit(0);
			return 0;
		default:
			/* This will be expanded in assignment 3. */
			break;
		}

	}
	assert(0);  /* can't reach here */
	return 0;
}
