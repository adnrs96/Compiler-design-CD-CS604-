#include<stdlib.h>
#include<stdio.h>

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

dfa* dfa_get_input_symbols(dfa *DFA)
{
  for (int i=0; i<DFA->input_symbol_count; i++)
  {
    printf("Enter %d(st|nd|th)input symbol\n",(i+1) );
    scanf(" %c",&DFA->input_alphabet[i]);
  }
  return DFA;
}
dfa* dfa_get_final_stateSymbols(dfa *DFA)
{
  for (int i=0; i < DFA->final_state_count; i++)
  {
    printf("Enter %d(st|nd|th) final state in final state set :q",(i) );
    scanf("%d",&DFA->final_states[i]);
  }
  return DFA;
}
dfa* dfa_get_transitionFunction(dfa *DFA)
{
  for(int i=0; i < DFA->state_count; i++)
  {
    for(int j=0; j < DFA->input_symbol_count; j++)
    {
      printf("Enter transition State for input alphabet %c for state q%d\n",DFA->input_alphabet[j],i);
      printf("q");
      scanf("%d",&DFA->states[i]->trans[j].transition_state);
      DFA->states[i]->trans[j].transition_alphabet = DFA->input_alphabet[j];
    }
  }
  return DFA;
}
void dfa_display(dfa *DFA)
{
  printf("\033[92m");
  for(int i=0;i<80;i++)
  printf("-");
  printf("\n");
  for(int i=0; i< DFA->input_symbol_count; i++)
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
  for(int i=0; i< DFA->state_count; i++)
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
      for(int j=0; j< DFA->input_symbol_count; j++)
      {
          printf("q%d              ",DFA->states[i]->trans[j].transition_state);

      }
      printf("\n");
  }
  printf("-> represents intial state and * represents final state(s)\n", );
  for(int i=0;i<80;i++)
  printf("-");
  printf("\n\033[0m");

}
dfa* dfa_intialise()
{
  int state_count,input_symbol_count,final_state_count;
  dfa *DFA;
  printf("Enter the number of states\n");
  scanf("%d",&state_count);
  printf("Enter the number of input symbol\n");
  scanf("%d",&input_symbol_count);
  DFA = (dfa*)malloc(sizeof(dfa) + sizeof(state)*state_count + (input_symbol_count)*sizeof(transition) );
  DFA->state_count = state_count;
  DFA->input_symbol_count = input_symbol_count;
  DFA->final_state_count = final_state_count;
  printf("Enter the number of final state set\n");
  scanf("%d",&DFA->final_state_count);
  DFA->final_states = calloc(final_state_count, sizeof(int));
  DFA->input_alphabet = calloc(input_symbol_count, sizeof(char));
  for(int i=0; i<DFA->state_count; i++)
  {
    DFA->states[i]=(state *)malloc(sizeof(state) + (DFA->input_symbol_count)*sizeof(transition));
  }
  return DFA;
}

int main()
{
  dfa *DFA = NULL;
  while(1)
  {
    printf("--------------DFA Wizard------------------\n");
    int choice = 0;
    printf("1) Enter a New DFA\n");
    printf("2) Display the Current DFA\n");
    printf("3) Run DFA as a String Accepting/Rejecting Machine\n");
    printf("4) Exit\n");
    scanf("%d",&choice);
    if(choice == 1)
    {
        DFA = dfa_intialise();
        DFA = dfa_get_input_symbols(DFA);
        DFA = dfa_get_final_stateSymbols(DFA);
        DFA = dfa_get_transitionFunction(DFA);
    }
    else if(choice == 2)
    {
        if(DFA != NULL)
        {
            dfa_display(DFA);
        }
        else
        {
            printf("Error Displaying DFA: No DFA entered yet\n");
        }
    }
    else if(choice ==3)
    {
      if(DFA != NULL)
      {
        while(1)
        {
          printf("--------------DFA Accepting Machine------------------\n");
          int choice1 = 0;
          printf("1) Enter a New String To Check\n");
          printf("2) Exit to previous menu\n");
          printf("3) Exit to terminal\n");
          scanf("%d",&choice1);
          if(choice1 == 1)
          {
            int num;
            printf("Enter no. of chars in string\n");
            scanf("%d",&num);
            printf("Enter the string to be proccesed\n");
            char input_string[num];
            scanf("%s",input_string);
            int ans = accepting_engine(DFA,input_string,num);
            if(ans){
                printf("\033[1;32mAccepted\n\033[0m" );
            }
            else
            {
                printf("\033[1;31mRejected\n\033[0m" );
            }
          }
          else if(choice1 == 2)
          {
              break;
          }
          else if(choice1 ==3)
          {
            return 0;
          }
          else
          {
            printf("Wrong Input please select correct option by entering option number between 1-4\n");
          }
        }
      }
      else
      {
          printf("Error Starting DFA Machine: No DFA entered yet\n");
      }
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
