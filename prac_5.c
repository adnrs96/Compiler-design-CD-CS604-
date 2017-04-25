// Lexical analyser

/*
 Token : Keywords, identifiers, constants, operators.
*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/*
state table
 0 :-> Keyword
 1 :-> Left (
 2 :-> Right )
 3 :-> Left {
 4 :-> Right }
 5 :-> left [
 6 :-> Right ]
 7 :-> Literal
 8 :-> constant
 9 :-> identifiers
 10 :-> Operator
 11 :-> Puntuation

*/

struct token_return {
  int start;
  int forward;
  int token_no;
};
typedef struct token_return token_return;

char *keywords[33] = {"auto", "const", "double", "float", "int", "short", "struct", "unsigned",
                 "break", "continue", "else", "for", "long", "signed", "switch", "void", "case",
                 "default", "enum", "goto", "register", "sizeof", "typedef", "volatile",
                 "char", "do", "extern", "if", "return", "static", "union", "while", "main"
               };
char *synchronize[16] = {" ", "+", "-", "/", "*", "<", ">", "=", "(", ")", "{", "}", "[", "]", ";", ","};
char *operators[8] = {"+", "-", "/", "*", "<", ">", "=", ","};

int isKeyword(char *str, int start, int forward)
{
  char cur_char[256];
  for(int i=0;i<33;i++)
  {
    int g = 1;
    if(forward-start == strlen(keywords[i]))
    {
      for(int j=start;j<forward;j++)
      if (str[j] != keywords[i][j-start])
      g = 0;
    }
    else
    {
      g = 0;
    }

    if (g)
    return 1;
  }
  return 0;
}

int isOperator(char cur_char)
{
  for(int i=0;i<8; i++)
  {
    if(*operators[i] == cur_char)
    return 1;
  }
  return 0;
}

token_return *next_token(int start, int forward, char *source)
{
  while(source[forward] == ' ' || source[forward] == '\n' || source[forward] == '\t')
  {
    start++;
    forward++;
  }
  char cur_char = source[forward];
  token_return *tok = (token_return*)malloc(sizeof(token_return));
  tok->start = start;
  switch (cur_char) {
    case '(':
    tok->forward = forward + 1;
    tok->token_no = 1;
    return tok;
    case ')':
    tok->forward = forward + 1;
    tok->token_no = 2;
    return tok;
    case '{':
    tok->forward = forward + 1;
    tok->token_no = 3;
    return tok;
    case '}':
    tok->forward = forward + 1;
    tok->token_no = 4;
    return tok;
    case '[':
    tok->forward = forward + 1;
    tok->token_no = 5;
    return tok;
    case ']':
    tok->forward = forward + 1;
    tok->token_no = 6;
    return tok;
    case ';':
    tok->forward = forward + 1;
    tok->token_no = 11;
    return tok;
  }
  // Checking identifiers
  if(isalpha(cur_char))
  {
    while(isalpha(source[forward]) || isdigit(source[forward]))
    forward++;
    tok->forward = forward;
    if(isKeyword(source, start, forward))
    tok->token_no = 0;
    else
    tok->token_no = 9;
    return tok;
  }
  // Checking constants
  if(isdigit(cur_char))
  {
    while(isdigit(source[forward]))
    forward++;
    tok->forward = forward;
    tok->token_no = 8;
    return tok;
  }
  // Checking For Literal
  if(cur_char == '"')
  {
    forward++;
    while(source[forward] != '"')
    forward++;
    forward++;
    tok->forward = forward;
    tok->token_no = 7;
    return tok;
  }
  if(isOperator(cur_char))
  {
    while(isOperator(source[forward]))
    forward++;
    tok->forward = forward;
    tok->token_no = 10;
    return tok;
  }
}

void print_token(int start, int forward, char *source)
{
    for(int i=start;i<forward;i++)
    {
      printf("%c",source[i]);
    }
}

char* inputString(char *str, FILE* fp, size_t size, int lines){
    int ch;
    size_t len = 0;
    str = (char*) realloc(str, size*sizeof(char));//size is start size
    if(!str)return str;
    while(lines--){
    while(EOF!=(ch=fgetc(fp)) && ch != '\n'){
        str[len++]=ch;
        if(len==size){
            str = (char*) realloc(str, sizeof(char)*(size+=100));
            if(!str)return str;
        }
    }
    str[len++]='\n';
    }
    str[len++]='\0';
    return realloc(str, sizeof(char)*len);
}


int main() {
  printf("Enter no. of Lines in Source Code\n");
  int lines = 0;
  scanf("%d", &lines);
  printf("Enter The Program to analyse:\n");
  fgetc(stdin); // Removing \n from input buffer.
  char *source = NULL;
  source = inputString(source,stdin, 100,lines);
  int start = 0, forward = 0;
  int len = strlen(source);
  token_return *tok;
  for(int i=0;i<80;i++)
  printf("-");
  printf("\n");
  printf("\t\tToken\t\t\tLexeme\n\n");
  while(start != len - 1)
  {
    tok = next_token(start, forward, source);
    printf("\t\t");
    switch (tok->token_no) {
      case 0:
      printf("Keyword \t\t");
      break;
      case 1:
      printf("Left Paranthesis\t");
      break;
      case 2:
      printf("Right Paranthesis       ");
      break;
      case 3:
      printf("Left Paranthesis\t");
      break;
      case 4:
      printf("Right Paranthesis       ");
      break;
      case 5:
      printf("Left Paranthesis\t");
      break;
      case 6:
      printf("Right Paranthesis       ");
      break;
      case 7:
      printf("Literal \t\t");
      break;
      case 8:
      printf("Constant\t\t");
      break;
      case 9:
      printf("Identifier      \t");
      break;
      case 10:
      printf("Operator\t\t");
      break;
      case 11:
      printf("Puntuation      \t");
      break;
    }
    print_token(tok->start, tok->forward, source);
    printf("\n");
    start = tok->forward;
    forward = tok->forward;
  }
}
