#include<stdlib.h>
#include<stdio.h>
#include <string.h>

struct transition{
  char transition_alphabet;
  int transition_state;
};
typedef struct transition transition;

struct state{
  char state_label;
  transition trans [];
};

typedef struct state state;
struct dfa{
  int state_count;
  int input_symbol_count;
  int final_state_count;
  int *final_states;
  char *input_alphabet;
  state *states[];
};
typedef struct dfa dfa;

int accepting_engine(dfa* DFA, char input_string[], int num)
{
    int current_state = 0;
    int transition_exits = 0,acceptance = 1;
    for(int i=0; i<num; i++)
    {
        transition_exits = 0;
        for(int j=0; j<DFA->input_symbol_count; j++)
        {
          if (DFA->states[current_state]->trans[j].transition_alphabet == input_string[i])
          {
            current_state = DFA->states[current_state]->trans[j].transition_state;
            transition_exits = 1;
            break;
          }
        }
        if(transition_exits == 0)
        {
          acceptance=0;
          break;
        }
    }

    if(acceptance == 1)
    {
        acceptance = 0;
        for(int j=0; j<DFA->final_state_count; j++)
        {
            if(current_state == DFA->final_states[j])
            {
              acceptance = 1;
              break;
            }
        }
    }

    if(acceptance == 0)
    {
      return 0;
    }
    else
    {
      return 1;
    }
}

char* inputString(FILE* fp, size_t size){
    char *str;
    int ch;
    size_t len = 0;
    str = (char*) realloc(NULL, size*sizeof(char));//size is start size
    if(!str)return str;
    while(EOF!=(ch=fgetc(fp)) && ch != ' ' &&ch != '\n'){
        str[len++]=ch;
        if(len==size){
            str = (char*) realloc(str, sizeof(char)*(size+=1));
            if(!str)return str;
        }
    }
    str[len++]='\0';
    return realloc(str, sizeof(char)*len);
}

dfa* dfa_get_input_symbols(dfa *DFA)
{
  for (int i=0;i<DFA->input_symbol_count;i++)
  {
    printf("Enter %d(st|nd|th)input symbol\n",(i+1) );
    scanf(" %c",&DFA->input_alphabet[i]);
  }
  return DFA;
}
void dfa_display(dfa *DFA)
{
  printf("\033[92m");
  for(int i=0;i<80;i++)
  printf("-");
  printf("\n");
  printf("\t\tDFA\n\n");
  for(int i=0;i< DFA->input_symbol_count; i++)
  {
    printf("%16c",DFA->input_alphabet[i]);
  }
  printf("\n\n");
  int is_final[DFA->state_count];
  for(int j=0; j < DFA->final_state_count; j++)
  {
    is_final[DFA->final_states[j]] = 1;
  }
  for(int j=0; j < DFA->state_count; j++)
  {
    if(is_final[j] != 1)
    is_final[j] = 0;
  }
  for(int i=0;i< DFA->state_count; i++)
  {
    if (i==0 && is_final[i])
    {
      printf("    ->*");
    }
    else if (i==0)
    {
      printf("     ->");
    }
    else if(is_final[i])
    {
      printf("      *");
    }
    else
    {
      printf("       ");
    }
    printf("q%d     ",i);
    for(int j=0;j< DFA->input_symbol_count; j++)
    {
        printf("q%d              ",DFA->states[i]->trans[j].transition_state);

    }
    printf("\n");
  }
  for(int i=0;i<80;i++)
  printf("-");
  printf("\n\033[0m");

}
dfa* dfa_intialise(int state_count,int final_state_count)
{
  int input_symbol_count;
  dfa *DFA;
  printf("Enter the number of input symbol\n");
  scanf("%d",&input_symbol_count);
  DFA = (dfa*)malloc(sizeof(dfa) + sizeof(state)*state_count + (input_symbol_count)*sizeof(transition) );
  DFA->state_count = state_count;
  DFA->input_symbol_count = input_symbol_count;
  DFA->final_state_count = final_state_count;
  DFA->final_states = calloc(final_state_count, sizeof(int));
  DFA->input_alphabet = calloc(input_symbol_count, sizeof(char));
  for(int i=0;i<DFA->state_count;i++)
  {
    DFA->states[i]=(state *)malloc(sizeof(state) + (DFA->input_symbol_count)*sizeof(transition));
  }
  return DFA;
}
dfa* generate_suffix_dfa(dfa *DFA,char *string,int num)
{
  printf("Enter some details about the required DFA\n");
  DFA = dfa_intialise(num+1,1);
  char first_char = string[0];
  char cur_char = string[0];
  DFA = dfa_get_input_symbols(DFA);
  for (int i=0;i < DFA->final_state_count;i++)
  {
    DFA->final_states[i] = num;
  }
  for(int i=0;i < DFA->state_count;i++)
  {
    if(i == DFA->state_count-1)
    {
      for(int j=0;j < DFA->input_symbol_count;j++)
      {
        if(DFA->input_alphabet[j] == first_char)
        {
          DFA->states[i]->trans[j].transition_state = 1;
          DFA->states[i]->trans[j].transition_alphabet = DFA->input_alphabet[j];
        }
        else
        {
          DFA->states[i]->trans[j].transition_state = 0;
          DFA->states[i]->trans[j].transition_alphabet = DFA->input_alphabet[j];
        }
      }
    }
    else
    {
      cur_char = string[i];
      for(int j=0;j < DFA->input_symbol_count;j++)
      {
        if(DFA->input_alphabet[j] == cur_char)
        {
          DFA->states[i]->trans[j].transition_state = i+1;
          DFA->states[i]->trans[j].transition_alphabet = DFA->input_alphabet[j];
        }
        else if(DFA->input_alphabet[j] == first_char)
        {
          DFA->states[i]->trans[j].transition_state = 1;
          DFA->states[i]->trans[j].transition_alphabet = DFA->input_alphabet[j];
        }
        else
        {
          DFA->states[i]->trans[j].transition_state = 0;
          DFA->states[i]->trans[j].transition_alphabet = DFA->input_alphabet[j];
        }
      }
    }
  }
  return DFA;
}

dfa* generate_prefix_dfa(dfa *DFA, char *string, int num)
{
  printf("Enter some details about the required DFA\n");
  DFA = dfa_intialise(num+2, 1);
  char cur_char = string[0];
  DFA = dfa_get_input_symbols(DFA);
  for (int i=0;i < DFA->final_state_count;i++)
  {
    DFA->final_states[i] = num;
  }
  for(int i=0;i < DFA->state_count;i++)
  {
    if(i == num + 1) // Sink is num+1
    {
      for(int j=0;j < DFA->input_symbol_count;j++)
      {
        DFA->states[i]->trans[j].transition_state = num + 1;
        DFA->states[i]->trans[j].transition_alphabet = DFA->input_alphabet[j];
      }
    }
    else if(i == num)
    {
      for(int j=0;j < DFA->input_symbol_count;j++)
      {
        DFA->states[i]->trans[j].transition_state = num;
        DFA->states[i]->trans[j].transition_alphabet = DFA->input_alphabet[j];
      }
    }
    else
    {
      cur_char = string[i];
      for(int j=0;j < DFA->input_symbol_count;j++)
      {
        if(DFA->input_alphabet[j] == cur_char)
        {
          DFA->states[i]->trans[j].transition_state = i+1;
          DFA->states[i]->trans[j].transition_alphabet = DFA->input_alphabet[j];
        }
        else
        {
          DFA->states[i]->trans[j].transition_state = num + 1;
          DFA->states[i]->trans[j].transition_alphabet = DFA->input_alphabet[j];
        }
      }
    }
  }
  return DFA;
}

dfa* generate_substring_dfa(dfa *DFA, char *string, int num)
{
  printf("Enter some details about the required DFA\n");
  DFA = dfa_intialise(num+1,1);
  char first_char = string[0];
  char cur_char = string[0];
  DFA = dfa_get_input_symbols(DFA);
  for (int i=0;i < DFA->final_state_count;i++)
  {
    DFA->final_states[i] = num;
  }
  for(int i=0;i < DFA->state_count;i++)
  {
    if(i == DFA->state_count-1)
    {
      for(int j=0;j < DFA->input_symbol_count;j++)
      {
        DFA->states[i]->trans[j].transition_state = num;
        DFA->states[i]->trans[j].transition_alphabet = DFA->input_alphabet[j];
      }
    }
    else
    {
      cur_char = string[i];
      for(int j=0;j < DFA->input_symbol_count;j++)
      {
        if(DFA->input_alphabet[j] == cur_char)
        {
          DFA->states[i]->trans[j].transition_state = i+1;
          DFA->states[i]->trans[j].transition_alphabet = DFA->input_alphabet[j];
        }
        else if(DFA->input_alphabet[j] == first_char)
        {
          DFA->states[i]->trans[j].transition_state = 1;
          DFA->states[i]->trans[j].transition_alphabet = DFA->input_alphabet[j];
        }
        else
        {
          DFA->states[i]->trans[j].transition_state = 0;
          DFA->states[i]->trans[j].transition_alphabet = DFA->input_alphabet[j];
        }
      }
    }
  }
  return DFA;
}

int ask_for_acceptance_testing(dfa* DFA)
{
  printf("Do you want to continue to (with) test strings for acceptance?\n");
  int choice = 0;
  printf("1) Yes\n");
  printf("2) No, Please exit to DFA Generation Wizard\n");
  scanf("%d",&choice);
  if(choice == 1)
  {
    char *string = NULL;
    printf("Enter the string to be proccesed\n");
    fgetc(stdin); // Removing \n from input buffer.
    string = inputString(stdin, 1);
    int num = strlen(string);
    int ans = accepting_engine(DFA, string, num);
    if(ans){
        printf("\033[1;32mAccepted\n\033[0m" );
    }
    else
    {
        printf("\033[1;31mRejected\n\033[0m" );
    }
    return 1;
  }
  else if(choice == 2)
  {
    return 0;
  }
  else
  {
    printf("Wrong Input please select correct option by entering option number between 1-4\n");
  }
}

int main()
{
  dfa *DFA = NULL;
  while(1)
  {
    printf("--------------DFA Generation Wizard------------------\n");
    int choice = 0;
    printf("1) Generate DFA for a given suffix\n");
    printf("2) Generate DFA for a given prefix\n");
    printf("3) Generate DFA for a given substring\n");
    printf("4) Exit\n");
    scanf("%d",&choice);
    if(choice == 1)
    {
      char *suffix_string = NULL;
      printf("Enter suffix string for which DFA should be generated.\n");
      fgetc(stdin); // Removing \n from input buffer.
      suffix_string = inputString(stdin, 1);
      int num = strlen(suffix_string);
      DFA = generate_suffix_dfa(DFA, suffix_string, num);
      dfa_display(DFA);
      while(ask_for_acceptance_testing(DFA));
    }
    else if(choice == 2)
    {
      char *prefix_string = NULL;
      printf("Enter prefix string for which DFA should be generated.\n");
      fgetc(stdin); // Removing \n from input buffer.
      prefix_string = inputString(stdin, 1);
      int num = strlen(prefix_string);
      DFA = generate_prefix_dfa(DFA, prefix_string, num);
      dfa_display(DFA);
      while(ask_for_acceptance_testing(DFA));
    }
    else if(choice ==3)
    {
      char *substring_string = NULL;
      printf("Enter substring string for which DFA should be generated.\n");
      fgetc(stdin); // Removing \n from input buffer.
      substring_string = inputString(stdin, 1);
      int num = strlen(substring_string);
      DFA = generate_substring_dfa(DFA, substring_string, num);
      dfa_display(DFA);
      while(ask_for_acceptance_testing(DFA));
    }
    else if(choice == 4)
    {
      return 0;
    }
    else
    {
      printf("Wrong Input please select correct option by entering option number between 1-4\n");
    }
  }
  return 0;
}
