#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

struct production
{
  char *prod_str;
  int total_first_set;
  char *first_set;
};
typedef struct production production;

struct first_set{
  char *first_set;
  int len;
};
typedef struct first_set first_Set;

struct follow_set{
  char *follow_set;
  int len;
};
typedef struct follow_set follow_Set;


struct production_rule
{
  char non_terminal;
  int total_first_set;
  int total_follow_set;
  char *first_set;
  char *follow_set;
  int total_productions;
  production *productions;
};
typedef struct production_rule production_rule;

struct cfg
{
  char *terminals;
  char *non_terminals;
  char start_symbol;
  int total_terminals;
  int total_non_terminals;
  int total_rules;
  production_rule *prod_rules;
};
typedef struct cfg cfg;

int is_Terminal(cfg* CFG, char cur_char)
{
   for(int i=0; i<CFG->total_terminals; i++)
   {
     if(CFG->terminals[i] == cur_char)
     return 1;
   }
   return 0;
}

production_rule* production_rule_for_non_terminal(cfg* CFG, char non_terminal)
{
  for(int i=0; i<CFG->total_rules; i++)
  {
    if(CFG->prod_rules[i].non_terminal == non_terminal)
    return &(CFG->prod_rules[i]);
  }
}

int null_in_set(char* set, int len)
{
  for(int i=0; i<len; i++)
  if(set[i] == '@')
  return 1;
  return 0;
}

int contains(char* a, int len, char b)
{
  //printf("%s Len %d  %c\n",a,len,b);
  for(int i=0; i<len; i++)
  if(a[i] == b)
  return i+1;
  return 0;
}

first_Set FIRST(cfg* CFG, char* symbol)
{
  char *first_set = NULL;
  int total = 0;
  if(strlen(symbol)==0)
  {
    first_set = (char*)malloc(sizeof(char));
    first_set[0] = '@';
    first_Set new_first_set;
    new_first_set.first_set = first_set;
    new_first_set.len = 1;
    return new_first_set;
  }
  if(is_Terminal(CFG, symbol[0]))
  {
    first_set = (char*)malloc(sizeof(char));
    first_set[0] = symbol[0];
    first_Set new_first_set;
    new_first_set.first_set = first_set;
    new_first_set.len = 1;
    return new_first_set;
  }
  if(symbol[0] == '@')
  {
    first_set = (char*)malloc(sizeof(char));
    first_set[0] = '@';
    first_Set new_first_set;
    new_first_set.first_set = first_set;
    new_first_set.len = 1;
    return new_first_set;
  }
  production_rule *prod_rule1 = production_rule_for_non_terminal(CFG, symbol[0]);
  if(prod_rule1->first_set != NULL)
  {
    first_Set t;
    t.first_set = prod_rule1->first_set;
    t.len = prod_rule1->total_first_set;
    return t;
  }
  int control = 1;
  while(control)
  {
    //printf("Reached1\n");
      production_rule *prod_rule = production_rule_for_non_terminal(CFG, symbol[control-1]);
      if (prod_rule->first_set==NULL)
      {
        for(int i=0; i<prod_rule->total_productions; i++)
        {
          first_Set temp = FIRST(CFG, prod_rule->productions[i].prod_str);
          prod_rule->productions[i].first_set = temp.first_set;
          prod_rule->productions[i].total_first_set = temp.len;
          prod_rule->first_set = (char*)realloc(prod_rule->first_set,sizeof(char)*(prod_rule->total_first_set + temp.len));
          int j=prod_rule->total_first_set;
          for(int k=0; k<temp.len;k++)
          {
            if(contains(prod_rule->first_set, prod_rule->total_first_set, prod_rule->productions[i].first_set[k]))
            {
              temp.len--;
            }
            else if(control != strlen(symbol) && prod_rule->productions[i].first_set[k] == '@')
            {
              temp.len--;
            }
            else
            {
              prod_rule->first_set[j] = prod_rule->productions[i].first_set[k];
              j++;
            }
          }
          prod_rule->total_first_set+=(j-prod_rule->total_first_set);
          prod_rule->first_set = (char*)realloc(prod_rule->first_set,sizeof(char)*(prod_rule->total_first_set));
        }
      }
      //printf("Reached2\n");
      first_set = (char*)realloc(first_set,sizeof(char)*(total+prod_rule->total_first_set));
      for(int i=total; i<total + prod_rule->total_first_set; i++)
      {
        first_set[i] = prod_rule->first_set[i-total];
      }
      //printf("Reached3\n");
      total+=prod_rule->total_first_set;
      //printf("12 num %d str '%s'\n",total, first_set);
      control++;
      if(null_in_set(prod_rule->first_set, prod_rule->total_first_set) == 0 || control > strlen(symbol))
      control = 0;
  }
  first_Set new_first_set;
  new_first_set.first_set = first_set;
  new_first_set.len = total;
  return new_first_set;
}

follow_Set FOLLOW(cfg* CFG, char non_terminal)
{
  //printf("Reached_FOLLOW 1\n");
  production_rule *prod_rule = production_rule_for_non_terminal(CFG, non_terminal);
  //printf("Reached_FOLLOW 2\n");
  if(prod_rule->follow_set != NULL)
  {
    follow_Set t;
    t.follow_set = prod_rule->follow_set;
    t.len = prod_rule->total_follow_set;
    return t;
  }
  production_rule *prod_rule_temp = production_rule_for_non_terminal(CFG, CFG->start_symbol);
  if(prod_rule_temp->follow_set == NULL)
  {
    prod_rule_temp->follow_set = (char*)malloc(sizeof(char));
    prod_rule_temp->follow_set[0] = '$';
    prod_rule_temp->total_follow_set = 1;
  }
  //printf("Reached_FOLLOW 3\n");
  for(int i=0; i<CFG->total_rules; i++)
  {
    for(int j=0; j<CFG->prod_rules[i].total_productions; j++)
    {
      int loc = contains(CFG->prod_rules[i].productions[j].prod_str, strlen(CFG->prod_rules[i].productions[j].prod_str), non_terminal);
      //printf("Reached_FOLLOW 4\n");
      if(loc)
      {
        char *str = (char*)malloc(sizeof(char)*(strlen(CFG->prod_rules[i].productions[j].prod_str)-loc+1));
        int l = 0;
        for(int k=loc;k<strlen(CFG->prod_rules[i].productions[j].prod_str);k++,l++)
        {
          str[l] = CFG->prod_rules[i].productions[j].prod_str[k];
        }
        str[l] = '\0';
        //printf("Reached_FOLLOW 5 String is %s\n",str);
        first_Set t = FIRST(CFG, str);
        int control = 0;
        //printf("While Calculating aac %s\n", prod_rule->follow_set);
        for(int k=0;k<t.len;k++)
        {
          if(t.first_set[k] == '@')
          {
            control = 1;
          }
          else
          {
            if(contains(prod_rule->follow_set, prod_rule->total_follow_set, t.first_set[k])==0)
            {
              prod_rule->follow_set = (char*)realloc(prod_rule->follow_set, sizeof(char)*(prod_rule->total_follow_set+1));
              prod_rule->follow_set[prod_rule->total_follow_set] = t.first_set[k];
              prod_rule->total_follow_set++;
            }
          }
        }
        //printf("While Calculating%s\n", prod_rule->follow_set);
        //printf("Reached_FOLLOW 6 %c\n",CFG->prod_rules[i].non_terminal);
        if(control)
        {
          follow_Set t1 = FOLLOW(CFG, CFG->prod_rules[i].non_terminal);
          //printf("While Calculating%s\n", prod_rule->follow_set);
          for(int k=0;k<t1.len;k++)
          {
            if(contains(prod_rule->follow_set, prod_rule->total_follow_set, t1.follow_set[k])==0)
            {
              prod_rule->follow_set = (char*)realloc(prod_rule->follow_set, sizeof(char)*(prod_rule->total_follow_set+1));
              prod_rule->follow_set[prod_rule->total_follow_set] = t1.follow_set[k];
              prod_rule->total_follow_set++;
            }
          }
        }
      }
    }
  }
  follow_Set t;
  t.follow_set = prod_rule->follow_set;
  t.len = prod_rule->total_follow_set;
  return t;
}

char* inputString(char *str, FILE* fp, size_t size){
    int ch;
    size_t len = 0;
    str = (char*) realloc(str, size*sizeof(char));//size is start size
    if(!str)return str;
    while(EOF!=(ch=fgetc(fp)) && ch != '\n'){
        str[len++]=ch;
        if(len==size){
            str = (char*) realloc(str, sizeof(char)*(size+=1));
            if(!str)return str;
        }
    }
    str[len++]='\0';
    return realloc(str, sizeof(char)*len);
}

void display_cfg(cfg* CFG)
{
  printf("\033[92m");
  for(int i=0; i<80; i++)
  printf("-");
  printf("\n\tGrammar\n\n");
  for(int i=0; i<CFG->total_rules; i++)
  {
    int used_space = 5;
    printf("%c -> ", CFG->prod_rules[i].non_terminal);
    for(int j=0; j<CFG->prod_rules[i].total_productions; j++)
    {
      printf("%s", CFG->prod_rules[i].productions[j].prod_str);
      used_space+=strlen(CFG->prod_rules[i].productions[j].prod_str);
      if(j != CFG->prod_rules[i].total_productions - 1)
      {
        printf("|");
        used_space++;
      }
      else
      {
        for (int spac = used_space; spac < 20; spac++)
        printf(" ");
        printf("First: {");
        for(int j=0; j< CFG->prod_rules[i].total_first_set; j++)
        {
          printf("%c", CFG->prod_rules[i].first_set[j]);
          if(j!=CFG->prod_rules[i].total_first_set - 1)
          printf(", ");
          else
          printf(" } ");
        }
        printf("Follow: {");
        for(int j=0; j<CFG->prod_rules[i].total_follow_set; j++)
        {
          printf("%c", CFG->prod_rules[i].follow_set[j]);
          if(j!=CFG->prod_rules[i].total_follow_set - 1)
          printf(", ");
          else
          printf(" }");
        }
        printf("\n");
      }

    }
  }
  printf("\033[0m");
}

cfg* initiate_cfg()
{
  cfg* CFG = (cfg*)malloc(sizeof(cfg));
  int terminals, non_terminals;
  printf("Enter the no. of terminals\n");
  scanf("%d", &terminals);
  CFG->total_terminals = terminals;
  CFG->terminals = (char*)malloc(sizeof(char)*terminals);
  printf("Enter all the terminals in CFG separated by space\n");
  for(int i=0;i<terminals;i++)
  {
    scanf(" %c", &CFG->terminals[i]);
  }
  printf("Enter the no. of non terminals\n");
  scanf("%d", &non_terminals);
  CFG->total_non_terminals = non_terminals;
  CFG->non_terminals = (char*)malloc(sizeof(char)*non_terminals);
  printf("Enter all the Non Terminals in CFG separated by space\n");
  for(int i=0;i<non_terminals;i++)
  {
    scanf(" %c", &CFG->non_terminals[i]);
  }
  CFG->prod_rules = (production_rule*)malloc(sizeof(production_rule)*non_terminals);
  CFG->total_rules = non_terminals;
  printf("Enter all the Production Rules. Use @ for representing a \u03D5 production.\n");
  fgetc(stdin);
  for(int i=0;i<non_terminals;i++)
  {
    CFG->prod_rules[i].non_terminal = CFG->non_terminals[i];
    CFG->prod_rules[i].total_first_set = 0;
    CFG->prod_rules[i].total_follow_set = 0;
    CFG->prod_rules[i].first_set = NULL;
    CFG->prod_rules[i].follow_set = NULL;
    printf("%c -> ", CFG->non_terminals[i]);
    char *str = NULL;
    str = inputString(str, stdin, 1);
    char *str1 = strtok(str, "|");
    int length = 1;
    CFG->prod_rules[i].productions = NULL;
    while(str1 != NULL)
    {
      CFG->prod_rules[i].productions = (production*)realloc(CFG->prod_rules[i].productions, sizeof(production)*length);
      CFG->prod_rules[i].productions[length - 1].prod_str = (char*)malloc(sizeof(char)*strlen(str1));
      strcpy(CFG->prod_rules[i].productions[length - 1].prod_str, str1);
      CFG->prod_rules[i].total_productions = length;
      CFG->prod_rules[i].productions[length - 1].total_first_set = 0;
      CFG->prod_rules[i].productions[length - 1].first_set = NULL;
      length++;
      str1 = strtok(NULL, "|");
    }
  }
  printf("Enter the start symbol\n");
  scanf(" %c", &CFG->start_symbol);
  return CFG;
}

void construct_display_ll1_parsing_table(cfg* CFG)
{
  printf("\033[92m");
  for(int i=0; i<80; i++)
  printf("-");
  printf("\nNon-Terminals\t\t\t\tInput Symbol\n\n");
  printf("\t\t");
  for(int i=0; i<CFG->total_terminals; i++)
  printf("%c\t\t", CFG->terminals[i]);
  printf("$\n\n");
  for(int i=0; i<CFG->total_non_terminals; i++)
  {
      printf("%c\t       ", CFG->non_terminals[i]);
      production_rule *prod_rule = production_rule_for_non_terminal(CFG, CFG->non_terminals[i]);
      for(int j=0; j<CFG->total_terminals; j++)
      {
        int space_used = 0;
        for(int k=0; k<prod_rule->total_productions; k++)
        {
          if(contains(prod_rule->productions[k].first_set, prod_rule->productions[k].total_first_set, CFG->terminals[j]))
          {
            printf("%c -> %s ", CFG->non_terminals[i], prod_rule->productions[k].prod_str);
            space_used+=6+strlen(prod_rule->productions[k].prod_str);
          }
          if(contains(prod_rule->productions[k].first_set, prod_rule->productions[k].total_first_set, '@') && contains(prod_rule->follow_set, prod_rule->total_follow_set, CFG->terminals[j]))
          {
            printf("%c -> %s ", CFG->non_terminals[i], prod_rule->productions[k].prod_str);
            space_used+=6+strlen(prod_rule->productions[k].prod_str);
          }
        }
        for(int k=0;k<16-space_used;k++)
        printf(" ");
      }
      if(contains(prod_rule->follow_set, prod_rule->total_follow_set, '$'))
      for(int k=0; k<prod_rule->total_productions; k++)
      if(contains(prod_rule->productions[k].first_set, prod_rule->productions[k].total_first_set, '@'))
      printf("%c -> %s ", CFG->non_terminals[i], prod_rule->productions[k].prod_str);
      printf("\n");
  }
  printf("\033[0m");
}

int main()
{
  cfg* CFG = initiate_cfg();
  for(int i=0; i<CFG->total_rules; i++)
  {
    if(CFG->prod_rules[i].first_set == NULL)
    {
      first_Set t = FIRST(CFG, &(CFG->prod_rules[i].non_terminal));
      CFG->prod_rules[i].total_first_set = t.len;
      CFG->prod_rules[i].first_set = t.first_set;
    }
  }
  for(int i=0; i<CFG->total_rules; i++)
  {
    if(CFG->prod_rules[i].follow_set == NULL)
    {
      follow_Set t = FOLLOW(CFG, CFG->prod_rules[i].non_terminal);
      CFG->prod_rules[i].total_follow_set = t.len;
      CFG->prod_rules[i].follow_set = t.follow_set;
    }
  }
  display_cfg(CFG);
  construct_display_ll1_parsing_table(CFG);
  return 0;
}
