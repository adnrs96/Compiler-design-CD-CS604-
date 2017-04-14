// Write a program that converts NFA given as input, to corresponding DFA.

#include<stdlib.h>
#include<stdio.h>
#include <string.h>

struct transition{
  char transition_alphabet;
  int total_transitions;
  int *transition_state;
};
typedef struct transition transition;

struct state{
  int *state_label;
  int total_states_inlabel;
  transition trans [];
};
typedef struct state state;

struct fa{
  int state_count;
  int input_symbol_count;
  int final_state_count;
  int *final_states;
  char *input_alphabet;
  state *states[];
};
typedef struct fa fa;

fa* fa_get_input_symbols(fa *FA)
{
  for (int i=0; i<FA->input_symbol_count; i++)
  {
    printf("Enter %d(st|nd|th)input symbol\n",(i+1) );
    scanf(" %c",&FA->input_alphabet[i]);
  }
  return FA;
}

fa* fa_get_final_stateSymbols(fa *FA)
{
  for (int i=0; i < FA->final_state_count; i++)
  {
    printf("Enter %d(st|nd|th) final state in final state set :q",(i) );
    scanf("%d",&FA->final_states[i]);
  }
  return FA;
}

fa* nfa_get_transitionFunction(fa *NFA)
{
  for(int i=0; i < NFA->state_count; i++)
  {
    for(int j=0; j < NFA->input_symbol_count; j++)
    {
      printf("Enter the number of Transitions for alphabet %c from state q%d: ",NFA->input_alphabet[j],i);
      int tot_trans = 0;
      scanf("%d",&tot_trans);
      NFA->states[i]->trans[j].transition_state = calloc(tot_trans, sizeof(int));
      NFA->states[i]->trans[j].transition_alphabet = NFA->input_alphabet[j];
      NFA->states[i]->trans[j].total_transitions = tot_trans;
      if(tot_trans >= 1)
      {
        printf("Enter the value of Transition function at:\n");
        printf("\u03B4(q%d,%c) = ",i,NFA->input_alphabet[j]);
        for(int k=0;k<tot_trans;k++)
        {
            scanf("%d",&NFA->states[i]->trans[j].transition_state[k]);
        }
      }
    }
  }
  return NFA;
}

fa* fa_intialise()
{
  int state_count,input_symbol_count;
  fa *FA;
  printf("Enter the number of states\n");
  scanf("%d",&state_count);
  printf("Enter the number of input symbol\n");
  scanf("%d",&input_symbol_count);
  FA = (fa*)malloc(sizeof(fa) + sizeof(void*)*state_count);
  FA->state_count = state_count;
  FA->input_symbol_count = input_symbol_count;
  printf("Enter the number of final state set\n");
  scanf("%d",&FA->final_state_count);
  FA->final_states = calloc(FA->final_state_count, sizeof(int));
  FA->input_alphabet = calloc(input_symbol_count, sizeof(char));
  for(int i=0; i<FA->state_count; i++)
  {
    FA->states[i]=(state *)malloc(sizeof(state) + (FA->input_symbol_count)*sizeof(transition));
  }
  return FA;
}

void nfa_display(fa *NFA)
{
  printf("\033[92m");
  for(int i=0;i<80;i++)
  printf("-");
  printf("\n");
  printf("\t\tNFA\n\n");
  for(int i=0;i< NFA->input_symbol_count; i++)
  {
    printf("%16c",NFA->input_alphabet[i]);
  }
  printf("\n\n");
  int is_final[NFA->state_count];
  for(int j=0; j < NFA->final_state_count; j++)
  {
    is_final[NFA->final_states[j]] = 1;
  }
  for(int j=0; j < NFA->state_count; j++)
  {
    if(is_final[j] != 1)
    is_final[j] = 0;
  }
  for(int i=0;i< NFA->state_count; i++)
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
    for(int j=0;j< NFA->input_symbol_count; j++)
    {
        int space_count = 4;
        printf("{ ");
        for(int k=0; k<NFA->states[i]->trans[j].total_transitions; k++)
        {
          printf("q%d",NFA->states[i]->trans[j].transition_state[k]);
          space_count+=2;
          if(k != NFA->states[i]->trans[j].total_transitions - 1)
          {
            printf(", ");
            space_count+=2;
          }
        }
        if(NFA->states[i]->trans[j].total_transitions == 0)
        {
          printf("\u03D5");
          space_count++;
        }
        printf(" }");
        for(int k=0; k<(16 - space_count); k++)
        printf(" ");
    }
    printf("\n");
  }
  printf("-> represents intial state and * represents final state(s)\n");
  for(int i=0;i<80;i++)
  printf("-");
  printf("\n\033[0m");

}

int comp (const void * elem1, const void * elem2)
{
    int f = *((int*)elem1);
    int s = *((int*)elem2);
    if (f > s) return  1;
    if (f < s) return -1;
    return 0;
}

int cmp_state_label(int *a, int len1, int *b, int len2)
{
  if(len1 == len2)
  {
    for(int i=0;i<len1;i++)
    {
      if(a[i]!=b[i])
      return 0;
    }
    return 1;
  }
  return 0;
}

int is_final_state(int state, fa* FA)
{
  for(int i=0;i<FA->final_state_count;i++)
  {
    if(FA->final_states[i] == state)
    {
      return 1;
    }
  }
  return 0;
}

int is_already_present(int state_to_check, int *label_list, int list_length)
{
  for(int i=0;i<list_length;i++)
  {
    if(label_list[i] == state_to_check)
    return 1;
  }
  return 0;
}

fa* nfa_to_dfa_convert(fa* NFA)
{
  int new_state_count = 0;
  int cur_state = -1;
  state **new_states = (state **)malloc(sizeof(void*));
  new_states[0] = (state *)malloc(sizeof(state) + (NFA->input_symbol_count)*sizeof(transition));
  new_states[0]->state_label = (int*)malloc(sizeof(int));
  new_states[0]->state_label[0] = 0;
  new_states[0]->total_states_inlabel = 1;
  new_state_count++;
  cur_state++;

  while(cur_state != new_state_count){
    for(int j=0; j < NFA->input_symbol_count; j++)
    {
        int *new_state_label = NULL;
        int new_total_states_inlabel = 0;
        int cur_state_label = -1;
        for(int i=0; i < new_states[cur_state]->total_states_inlabel; i++)
        {
          for(int k=0; k < NFA->states[new_states[cur_state]->state_label[i]]->trans[j].total_transitions; k++)
          {
            if(!is_already_present(NFA->states[new_states[cur_state]->state_label[i]]->trans[j].transition_state[k], new_state_label, new_total_states_inlabel)){
              new_total_states_inlabel += 1;
              new_state_label = (int*)realloc(new_state_label, sizeof(int)*new_total_states_inlabel);
              cur_state_label++;
              new_state_label[cur_state_label] = NFA->states[new_states[cur_state]->state_label[i]]->trans[j].transition_state[k];
            }
          }
        }
        qsort(new_state_label,new_total_states_inlabel,sizeof(int),comp);

        for(int i=0;i<new_state_count + 1;i++)
        {
          if(i == new_state_count)
          {
            new_state_count++;
            new_states = (state **)realloc(new_states,sizeof(void*)*new_state_count);
            new_states[i] = (state *)malloc(sizeof(state) + (NFA->input_symbol_count)*sizeof(transition));
            new_states[i]->state_label = new_state_label;
            new_states[i]->total_states_inlabel = new_total_states_inlabel;

            new_states[cur_state]->trans[j].transition_state = (int*)malloc(sizeof(int));
            new_states[cur_state]->trans[j].transition_state[0] = i;
            new_states[cur_state]->trans[j].transition_alphabet = NFA->input_alphabet[j];
            new_states[cur_state]->trans[j].total_transitions = 1;

            break;
          }
          if(cmp_state_label(new_state_label, new_total_states_inlabel, new_states[i]->state_label, new_states[i]->total_states_inlabel))
          {
            new_states[cur_state]->trans[j].transition_state = (int*)malloc(sizeof(int));
            new_states[cur_state]->trans[j].transition_state[0] = i;
            new_states[cur_state]->trans[j].transition_alphabet = NFA->input_alphabet[j];
            new_states[cur_state]->trans[j].total_transitions = 1;
            free(new_state_label);
            break;
          }
        }

    }
    cur_state++;
  }
  int new_final_state_count = 0;
  int *new_final_states = NULL;
  for(int i=0;i<new_state_count;i++)
  {
    for(int j=0;j<new_states[i]->total_states_inlabel;j++)
    {
      if(is_final_state(new_states[i]->state_label[j],NFA))
      {
        new_final_state_count++;
        new_final_states = (int*)realloc(new_final_states,sizeof(int)*new_final_state_count);
        new_final_states[new_final_state_count - 1] = i;
        break;
      }
    }
  }
  fa *DFA = (fa*)malloc(sizeof(fa) + sizeof(void*)*new_state_count);
  DFA->state_count = new_state_count;
  DFA->input_symbol_count = NFA->input_symbol_count;
  DFA->final_state_count = new_final_state_count;
  DFA->input_alphabet = NFA->input_alphabet;
  DFA->final_states = new_final_states;
  for(int i=0;i<new_state_count;i++)
  {
    DFA->states[i] = new_states[i];
  }
  return DFA;

}

int print_dfa_state(state *dfa_state)
{
  int space_count = 0;
  if(dfa_state->total_states_inlabel > 0)
  {
    printf("[ ");
    space_count+=2;
    for(int i=0;i<dfa_state->total_states_inlabel;i++)
    {
      if(i != dfa_state->total_states_inlabel - 1)
      {
        printf("q%d, ",dfa_state->state_label[i]);
      }
      else
      {
        printf("q%d ]",dfa_state->state_label[i]);
      }
      space_count+=4;
    }
  }
  else
  {
    printf("[ \u03D5 ]");
    space_count+=5;
  }
  return space_count;
}

void dfa_display(fa *DFA)
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
    int space_count = 0;
    if (i==0 && is_final[i])
    {
      printf("->*");
      space_count+=3;
    }
    else if (i==0)
    {
      printf("->");
      space_count+=2;
    }
    else if(is_final[i])
    {
      printf("*");
      space_count+=1;
    }
    else
    {
      printf(" ");
      space_count+=1;
    }
    space_count += print_dfa_state(DFA->states[i]);
    for(int k=0; k<(14 - space_count); k++)
    printf(" ");
    for(int j=0;j< DFA->input_symbol_count; j++)
    {
      int space_count = print_dfa_state(DFA->states[DFA->states[i]->trans[j].transition_state[0]]);
      for(int k=0; k<(16 - space_count); k++)
      printf(" ");
    }
    printf("\n");
  }
  printf("-> represents intial state, * represents final state(s) and [\u03D5] represents a dead state\n");
  for(int i=0;i<80;i++)
  printf("-");
  printf("\n\033[0m");

}

int main()
{
  fa *NFA = NULL;
  fa *DFA = NULL;
  while(1)
  {
    printf("--------------NFA to DFA CONVERSION Wizard------------------\n");
    int choice = 0;
    printf("1) Convert a New NFA to DFA.\n");
    printf("2) Display Current NFA and its corresponding DFA.\n");
    printf("3) Exit\n");
    scanf("%d",&choice);
    if(choice == 1)
    {
      NFA = fa_intialise();
      NFA = fa_get_input_symbols(NFA);
      NFA = fa_get_final_stateSymbols(NFA);
      NFA = nfa_get_transitionFunction(NFA);
      DFA = nfa_to_dfa_convert(NFA);
    }
    else if(choice == 2)
    {
      if(NFA != NULL)
      {
          nfa_display(NFA);
          dfa_display(DFA);
      }
      else
      {
          printf("Error Displaying NFA: No NFA entered yet\n");
      }
    }
    else if(choice == 3)
    {
      return 0;
    }
    else
    {
      printf("Wrong Input please select correct option by entering option number between 1-3\n");
    }
  }
  return 0;
}
