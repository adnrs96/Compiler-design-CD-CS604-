#include<stdlib.h>
#include<stdio.h>
#include<string.h>

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

int main()
{
  printf("Enter no. of Lines in Source Code\n");
  int lines = 0;
  scanf("%d", &lines);
  printf("Enter The Program to count comments for:\n");
  fgetc(stdin); // Removing \n from input buffer.
  char *source = NULL;
  source = inputString(source,stdin, 100,lines);
  int i = 0;
  int single_line = 0, multi_line = 0;
  printf("\n");
  while(source[i]!='\0'){
    if(source[i]=='/'){
      i++;
      if(source[i]=='/'){
        single_line++;
        i++;
        printf("Single Line Comment: ");
        while(source[i]!='\n' && source[i]!='\0'){
          printf("%c",source[i]);
          i++;
        }
        printf("\n");
      }
      else if(source[i]=='*'){
        i++;
        printf("Multi Line Comment: ");
        while(source[i]!='*' && source[i+1]!='/'){
          printf("%c",source[i]);
          if(source[i] == '\n')
          printf("                    ");
          i++;
        }
        printf("\n");
        i+=1;
        multi_line++;
        continue;
      }
    }
    i++;
  }
  printf("Single Line Comments: %d\n", single_line);
  printf("Multi Line Comments: %d\n", multi_line);
  return 0;
}
