/* Scanner
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "reader.h"
#include "charcode.h"
#include "token.h"
#include "error.h"
#include "scanner.h"


extern int lineNo;
extern int colNo;
extern int currentChar;
int intPart;
int i = 0;
extern CharCode charCodes[];
int maxNumberDigit = 10;

/***************************************************************/

void skipBlank() {
  while ((currentChar != EOF) && (charCodes[currentChar] == CHAR_SPACE))
    readChar();
}

void skipComment() {
  int state = 0;
  while ((currentChar != EOF) && (state < 2)) {
    switch (charCodes[currentChar]) {
    case CHAR_TIMES:
      state = 1;
      break;
    case CHAR_RPAR:
      if (state == 1) state = 2;
      else state = 0;
      break;
    default:
      state = 0;
    }
    readChar();
  }
  if (state != 2) 
    error(ERR_END_OF_COMMENT, lineNo, colNo);
}

Token* readIdentKeyword(void) {
  Token *token = makeToken(TK_NONE, lineNo, colNo);
  int count = 1;

  token->string[0] = toupper((char)currentChar);
  readChar();

  while ((currentChar != EOF) && 
	 ((charCodes[currentChar] == CHAR_LETTER) || (charCodes[currentChar] == CHAR_DIGIT))) {
    if (count <= MAX_IDENT_LEN) token->string[count++] = toupper((char)currentChar);
    readChar();
  }

  if (count > MAX_IDENT_LEN) {
    error(ERR_IDENT_TOO_LONG, token->lineNo, token->colNo);
    return token;
  }

  token->string[count] = '\0';
  token->tokenType = checkKeyword(token->string);

  if (token->tokenType == TK_NONE)
    token->tokenType = TK_IDENT;

  return token;
}

Token* readNumber(void) {
  //TODO
  Token* tokenNum = makeToken(TK_NUMBER, lineNo, colNo ) ;
  int i = 0;
  int checkDot = 0;
  if (intPart == 0)
  {
    tokenNum->string[0] = '0';
    tokenNum->string[1] = '.';
    checkDot = 1;
    i = 2;
    tokenNum->colNo -= 1;
  }
  else
    tokenNum = makeToken(TK_NUMBER, lineNo, colNo);
  do
  {
    if (charCodes[currentChar] == EOF) break;
    if ((charCodes[currentChar] == CHAR_PERIOD) && checkDot == 1) break;
    if ((charCodes[currentChar] == CHAR_PERIOD) && checkDot == 0) checkDot = 1;
      tokenNum->string[i] = (char)currentChar;
      i+= 1;
      readChar();
  } while (charCodes[currentChar] == CHAR_DIGIT || charCodes[currentChar] == CHAR_PERIOD);
  
  if (tokenNum->string[i-1] == '.')
  {
    tokenNum->string[i] = '0';
    i += 1;
  }
  tokenNum->string[i] = '\0';
  if (checkDot == 1)
    {
      tokenNum->tokenType = TK_DOUBLE;
      tokenNum->d_value = atof(tokenNum->string);
    }
  else 
      tokenNum->value = atoi(tokenNum->string);
  
  return tokenNum;
  }

Token* readConstChar(void) {
  Token *token = makeToken(TK_CHAR, lineNo, colNo);

  readChar();
  if (currentChar == EOF) {
    token->tokenType = TK_NONE;
    error(ERR_INVALID_CONSTANT_CHAR, token->lineNo, token->colNo);
    return token;
  }
    
  token->string[0] = currentChar;
  token->string[1] = '\0';

  readChar();
  if (currentChar == EOF) {
    token->tokenType = TK_NONE;
    error(ERR_INVALID_CONSTANT_CHAR, token->lineNo, token->colNo);
    return token;
  }

  if (charCodes[currentChar] == CHAR_SINGLEQUOTE) {
    readChar();
    return token;
  } else {
    token->tokenType = TK_NONE;
    error(ERR_INVALID_CONSTANT_CHAR, token->lineNo, token->colNo);
    return token;
  }
}
Token* readString(){

  int i = 0;
  readChar();
  Token* result = makeToken(TK_STRING, lineNo, colNo-1);

  do {
    if (charCodes[currentChar] == CHAR_DOUBLEQUOTE) break;
    result->string[i] = (char) currentChar;
    i++;
    readChar();
  } while (1);
  
  result->string[i] = '\0';
  readChar();
  return result;
}

Token* getToken(void) {
  Token *token;
  int ln, cn;

  if (currentChar == EOF) 
    return makeToken(TK_EOF, lineNo, colNo);

  switch (charCodes[currentChar]) {
  case CHAR_SPACE: skipBlank(); return getToken();
  case CHAR_LETTER: return readIdentKeyword();
  case CHAR_DIGIT: 
  {
    intPart = 1;
    return readNumber();
  }
  case CHAR_PLUS: {
    token = makeToken(SB_PLUS, lineNo, colNo);
    readChar(); 
    return token;
  }
  case CHAR_MINUS:
  {
  	ln = lineNo;
    cn = colNo;
    token = makeToken(SB_MINUS, ln, cn);
    readChar();
    return token;
  }
  case CHAR_SEMICOLON:
  {
    token = makeToken(SB_SEMICOLON, lineNo, colNo);
    readChar();
    return token;
  }
  case CHAR_COMMA:
  {
    token = makeToken(SB_COMMA, lineNo, colNo);
    readChar();
    return token;
  }
  case CHAR_EQ:
  {
    token = makeToken(SB_EQ, lineNo, colNo);
    readChar();
    return token;
  }
  case CHAR_RPAR:
  {
  	token = makeToken(SB_RPAR, lineNo, colNo);
  	readChar();
    return token;
  }
  
  case CHAR_SINGLEQUOTE:
  {
      return readConstChar();
  }
    
  case CHAR_DOUBLEQUOTE:
    return readString();

  case CHAR_LPAR:
    ln = lineNo;
    cn = colNo;
    readChar();

    if (currentChar == EOF) 
      return makeToken(SB_LPAR, ln, cn);

    switch (charCodes[currentChar]) {
    case CHAR_PERIOD:
      readChar();
      return makeToken(SB_LSEL, ln, cn);
    case CHAR_TIMES:
      readChar();
      skipComment();
      return getToken();
    default:
      return makeToken(SB_LPAR, ln, cn);
    }
  case CHAR_COLON:
    ln = lineNo;
    cn = colNo;
    readChar();
    if (charCodes[currentChar] == CHAR_EQ){
      token = makeToken(SB_ASSIGN, ln, cn);
      readChar();
      return token;
    }
    else 
    {
      token = makeToken(SB_COLON, ln, cn);
      return token;
    }
  case CHAR_PERIOD:
  
    ln = lineNo;
    cn = colNo;
    readChar();
    if ((currentChar != EOF) && (charCodes[currentChar] == CHAR_RPAR))
    {
      readChar();
      return makeToken(SB_RSEL, ln, cn);
    }
    else if(charCodes[currentChar] == CHAR_DIGIT)
    {
      intPart = 0;
      return readNumber();
    }
    else
      return makeToken(SB_PERIOD, ln, cn);
  case CHAR_EXCLAIMATION:
    ln = lineNo;
    cn = colNo;
    readChar();
    if ((currentChar != EOF) && (charCodes[currentChar] == CHAR_EQ))
    {
    	token = makeToken(SB_NEQ, ln, cn);
    	readChar();
      	return token;     
  }
    else
    {
 		error(ERR_INVALID_SYMBOL, ln, cn);

    }
  case CHAR_LT:
    ln = lineNo;
    cn = colNo;
    readChar();
    if ((currentChar != EOF) && charCodes[currentChar] == CHAR_EQ)
    {
      readChar();
      return makeToken(SB_LE, ln, cn);
    }
    else
    {
      return makeToken(SB_LT, ln, cn);
    }
  case CHAR_GT:
    ln = lineNo;
    cn = colNo;
    readChar();
    if ((currentChar != EOF) && charCodes[currentChar] == CHAR_EQ)
    {
      return makeToken(SB_GE, ln, cn);
    }
    else
    {
      return makeToken(SB_GT, ln, cn);
    }
  case CHAR_TIMES:
    
    ln = lineNo;
    cn = colNo;
    readChar();
    if ((charCodes[currentChar] == CHAR_RPAR) && (currentChar != EOF))
    {
      error(ERR_END_OF_COMMENT, ln, cn);
    }
    else
    {
      return makeToken(SB_TIMES, ln, cn);
    }
  case CHAR_SLASH:
    token = makeToken(SB_SLASH, lineNo, colNo);
    readChar(); 
    return token;
  default:
  {
    token = makeToken(TK_NONE, lineNo, colNo);
    error(ERR_INVALID_SYMBOL, lineNo, colNo);
    readChar(); 
    return token;
   }
  }
}

Token* getValidToken(void) {
  Token *token = getToken();
  while (token->tokenType == TK_NONE) {
    free(token);
    token = getToken();
  }
  return token;
}

/******************************************************************/

void printToken(Token *token) {

  printf("%d-%d:", token->lineNo, token->colNo);

  switch (token->tokenType) {
  case TK_NONE: printf("TK_NONE\n"); break;
  case TK_IDENT: printf("TK_IDENT(%s)\n", token->string); break;
  case TK_NUMBER: printf("TK_NUMBER(%s)\n", token->string); break;
  case TK_CHAR: printf("TK_CHAR(\'%s\')\n", token->string); break;
  case TK_EOF: printf("TK_EOF\n"); break;
  case TK_STRING: printf("TK_STRING(\'%s\')\n", token->string); break;
  case TK_DOUBLE: printf("TK_DOUBLE(%s)\n", token->string); break;

  case KW_PROGRAM: printf("KW_PROGRAM\n"); break;
  case KW_CONST: printf("KW_CONST\n"); break;
  case KW_TYPE: printf("KW_TYPE\n"); break;
  case KW_VAR: printf("KW_VAR\n"); break;
  case KW_INTEGER: printf("KW_INTEGER\n"); break;
  case KW_CHAR: printf("KW_CHAR\n"); break;
  case KW_DOUBLE: printf("KW_DOUBLE\n"); break;
  case KW_STRING: printf("KW_STRING\n"); break;
  case KW_ARRAY: printf("KW_ARRAY\n"); break;
  case KW_OF: printf("KW_OF\n"); break;
  case KW_FUNCTION: printf("KW_FUNCTION\n"); break;
  case KW_PROCEDURE: printf("KW_PROCEDURE\n"); break;
  case KW_BEGIN: printf("KW_BEGIN\n"); break;
  case KW_END: printf("KW_END\n"); break;
  case KW_CALL: printf("KW_CALL\n"); break;
  case KW_IF: printf("KW_IF\n"); break;
  case KW_THEN: printf("KW_THEN\n"); break;
  case KW_ELSE: printf("KW_ELSE\n"); break;
  case KW_WHILE: printf("KW_WHILE\n"); break;
  case KW_DO: printf("KW_DO\n"); break;
  case KW_FOR: printf("KW_FOR\n"); break;
  case KW_TO: printf("KW_TO\n"); break;

  case SB_SEMICOLON: printf("SB_SEMICOLON\n"); break;
  case SB_COLON: printf("SB_COLON\n"); break;
  case SB_PERIOD: printf("SB_PERIOD\n"); break;
  case SB_COMMA: printf("SB_COMMA\n"); break;
  case SB_ASSIGN: printf("SB_ASSIGN\n"); break;
  case SB_EQ: printf("SB_EQ\n"); break;
  case SB_NEQ: printf("SB_NEQ\n"); break;
  case SB_LT: printf("SB_LT\n"); break;
  case SB_LE: printf("SB_LE\n"); break;
  case SB_GT: printf("SB_GT\n"); break;
  case SB_GE: printf("SB_GE\n"); break;
  case SB_PLUS: printf("SB_PLUS\n"); break;
  case SB_MINUS: printf("SB_MINUS\n"); break;
  case SB_TIMES: printf("SB_TIMES\n"); break;
  case SB_SLASH: printf("SB_SLASH\n"); break;
  case SB_LPAR: printf("SB_LPAR\n"); break;
  case SB_RPAR: printf("SB_RPAR\n"); break;
  case SB_LSEL: printf("SB_LSEL\n"); break;
  case SB_RSEL: printf("SB_RSEL\n"); break;
  }
}

