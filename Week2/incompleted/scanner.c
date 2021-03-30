/* Scanner
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "reader.h"
#include "charcode.h"
#include "token.h"
#include "error.h"


extern int lineNo;
extern int colNo;
extern int currentChar;

int intPart;
int i = 0 ;
extern CharCode charCodes[];
int maxNumberDigit = 10;

/***************************************************************/

void skipBlank() {
  // TODO
  while (charCodes[currentChar] == CHAR_SPACE && currentChar != EOF)
  {
    readChar();
  }
  
}

void skipComment() {
  int checkEndOfComment = 0;
  // checkEndOfComment = 1- > char now get: *, char =2: ) => finish skip comment
  // Go to end of comment, have model: *)
  while ((currentChar != EOF) && (checkEndOfComment < 2))
  {
    switch (charCodes[currentChar])
    {
    case CHAR_TIMES:
    {
      checkEndOfComment++;
      break;
    }
    case CHAR_RPAR:
    {
      // if we had '*', now when get '(' => ok we have all conditions for an comment
      if (checkEndOfComment == 1)
      {
        checkEndOfComment = 2;
      }
      else
      {
        checkEndOfComment = 0;
      }
      break;
    }
    default:
      checkEndOfComment = 0;
    }
    readChar();
  }
  if (checkEndOfComment != 2)
  {
    error(ERR_ENDOFCOMMENT, lineNo, colNo);
  }

}
void skipDoubleSlash(void)
{
  while ((currentChar != EOF) && (currentChar != '\n'))
  {
    readChar();
  }
}

Token* readIdentKeyword(void) {
  Token *tokenReadIdentKeyword = makeToken(TK_CHAR, lineNo, colNo);

  i = 0;
  tokenReadIdentKeyword->string[i] = currentChar;
  currentChar = readChar();
  while (charCodes[currentChar] == CHAR_LETTER || charCodes[currentChar] == CHAR_DIGIT)
  {
    i++;
    tokenReadIdentKeyword->string[i] = currentChar;
    currentChar = readChar();
  }
  tokenReadIdentKeyword->string[i+1] = '\0';
  if (i > MAX_IDENT_LEN)
  {
    error(ERR_IDENTTOOLONG, tokenReadIdentKeyword->lineNo, tokenReadIdentKeyword->colNo);
  }

  TokenType valueHere = checkKeyword(tokenReadIdentKeyword->string);
  if (valueHere != TK_NONE)
  {
    tokenReadIdentKeyword->tokenType = valueHere;
  }
  else
  {
    tokenReadIdentKeyword->tokenType = TK_IDENT;
  }
  return tokenReadIdentKeyword;

}

Token* readNumber(void) {
  //TODO
  Token* tokenNum = makeToken(TK_NUMBER, lineNo, colNo);
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
    tokenNum->tokenType = TK_FLOAT;
  
  return tokenNum;
    i = 2;
    readChar();
  }



Token* readConstChar(void) {
  // TODO
  Token *token = makeToken(TK_CHAR, lineNo, colNo);
  readChar();
  if (currentChar == EOF)
  {   //  
    token->tokenType = TK_NONE;
    error(ERR_INVALIDCHARCONSTANT, lineNo, colNo);
    return token;
  }
  // get const char
  token->string[0] = currentChar;
  token->value = currentChar;

  readChar();
  if (currentChar == EOF)
  {
    token->tokenType = TK_NONE;
    error(ERR_INVALIDCHARCONSTANT, lineNo, colNo);
    return token;
  }
  if (charCodes[currentChar] == CHAR_SINGLEQUOTE)
  {
    token->tokenType = TK_CHAR;
    readChar();
    return token;
  }
  else
  {
    token->tokenType = TK_NONE;
    error(ERR_INVALIDCHARCONSTANT, token->lineNo, token->colNo);
    return token;
  }
}
Token* readString(){

  int i = 0;
  readChar();
  Token* result = makeToken(TK_STRING, lineNo, colNo);

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
    switch (charCodes[currentChar])
    {
    case CHAR_TIMES:
      readChar();
      skipComment();
      // AFter skip comment, we again get token
      return getToken();
      break;
    case CHAR_PERIOD:
      readChar();
      return makeToken(SB_RSEL, ln, cn);
      break;
    default:
      return makeToken(SB_LPAR, ln, cn);
      break;
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
 		error(ERR_INVALIDSYMBOL, ln, cn);

    }
  case CHAR_LT:
    ln = lineNo;
    cn = colNo;
    readChar();
    if ((currentChar != EOF) && charCodes[currentChar] == CHAR_EQ)
    {

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
      error(ERR_ENDOFCOMMENT, ln, cn);
    }
    else
    {
      return makeToken(SB_TIMES, ln, cn);
    }
  case CHAR_SLASH:
    cn = colNo;
    ln = lineNo;
    readChar();
    if ((currentChar != EOF) && charCodes[currentChar] == CHAR_SLASH)
    {
      skipDoubleSlash();
      return getToken();
    }
    else
      return makeToken(SB_SLASH, cn, ln);
    
    // ....
    // TODO
    // ....
  default:
  {
    token = makeToken(TK_NONE, lineNo, colNo);
    error(ERR_INVALIDSYMBOL, lineNo, colNo);
    readChar(); 
    return token;
   }
  }
}


/******************************************************************/

void printToken(Token *token) {

  printf("%d-%d:", token->lineNo, token->colNo);

  switch (token->tokenType) {
  case TK_NONE: printf("TK_NONE\n"); break;
  case TK_IDENT: printf("TK_IDENT(%s)\n", token->string); break;
  case TK_NUMBER: printf("TK_NUMBER(%s)\n", token->string); break;
  case TK_CHAR: printf("TK_CHAR(\'%s\')\n", token->string); break;
  case TK_STRING: printf("TK_STRING(\'%s\')\n", token->string); break;
  case TK_EOF: printf("TK_EOF\n"); break;
  case TK_FLOAT: printf("TK_FLOAT(%s)\n", token->string); break;

  case KW_PROGRAM: printf("KW_PROGRAM\n"); break;
  case KW_CONST: printf("KW_CONST\n"); break;
  case KW_TYPE: printf("KW_TYPE\n"); break;
  case KW_VAR: printf("KW_VAR\n"); break;
  case KW_INTEGER: printf("KW_INTEGER\n"); break;
  case KW_CHAR: printf("KW_CHAR\n"); break;
  case KW_ARRAY: printf("KW_ARRAY\n"); break;
  case KW_FLOAT: printf("KW_FLOAT\n"); break;
  case KW_STRING: printf("KW_STRING\n"); break;
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

int scan(char *fileName) {
  Token *token;

  if (openInputStream(fileName) == IO_ERROR)
    return IO_ERROR;

  token = getToken();
  while (token->tokenType != TK_EOF) {
    printToken(token);
    free(token);
    token = getToken();
  }

  free(token);
  closeInputStream();
  return IO_SUCCESS;
}

/******************************************************************/

int main(int argc, char *argv[]) {
  if (argc <= 1) {
    printf("scanner: no input file.\n");
    return -1;
  }

  if (scan(argv[1]) == IO_ERROR) {
    printf("Can\'t read input file!\n");
    return -1;
  }
    
  return 0;
}



