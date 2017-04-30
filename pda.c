//Alim Ozdemir 150140807
//Merve Ecevit 150140115
//Firdevs Canli 150130802

#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <time.h>

#define CLOCKS_PER_MS (CLOCKS_PER_SEC / 1000)

typedef struct char_stack char_stack;
typedef struct int_stack int_stack;
typedef struct decision_pointer_stack decision_pointer_stack;
typedef struct decision decision;

//BEGIN OF char_stack

struct char_stack
{
    char elements[100];
    int index_top;
    int count;
};

int compareUntil(char_stack *s, char *input, int len)
{
    int result = -1;

    int i = 0;
    for (i = 0; i < s->count; i++)
    {
        if (input[i] == s->elements[i])
        {
            result = 1;
        }
        else
        {
            result = -1;
            break;
        }
    }

    return result;
}

int compare(char_stack *s, char *input, int len)
{
    int result = -1;
    if (len == s->count)
    {
        int i = 0;
        for (i = 0; i < len; i++)
        {
            if (input[i] == s->elements[i])
            {
                result = 1;
            }
            else
            {
                result = -1;
                break;
            }
        }
    }

    return result;
}


void push_c(char_stack *s, char param)
{
    s->elements[s->index_top++] = param;
    s->count++;
}

char pop_c(char_stack *s)
{
    s->count--;
    return s->elements[--s->index_top];
}

void clone_c(char_stack *s, char_stack *dest)
{
    int i = 0;
    for (i = 0; i < s->count; i++)
    {
        dest->elements[i] = s->elements[i];
    }
    dest->index_top = s->index_top;
    dest->count = s->count;
}

void print_c(char_stack *s)
{
    int i = 0;
    for (i = 0; i < s->count; i++)
    {
        printf("%c", s->elements[i]);
    }
}

void init_c(char_stack *s)
{
    s->count = 0;
    s->index_top = 0;
}

//END OF char_stack

//BEGIN OF int_stack
struct int_stack
{
    int count;
    int elements[100];
    int index_top;
};
int contains_i(int_stack *s, int param)
{
    int result = -1, i = 0;

    for (i = 0; i < s->count; i++)
    {
        if(s->elements[i] == param)
        {
            result = 1;
            break;
        }
    }

    return result;
}

void push_i(int_stack *s, int param)
{
    s->elements[s->index_top++] = param;
    s->count++;
}

int pop_i(int_stack *s)
{
    s->count--;
    return s->elements[--s->index_top];
}

void clone_i(int_stack *s, int_stack *dest)
{
    int i = 0;
    for (i = 0; i < s->count; i++)
    {
        dest->elements[i] = s->elements[i];
    }
    dest->index_top = s->index_top;
    dest->count = s->count;
}

void print_i(int_stack *s)
{
    int i = 0;
    for (i = 0; i < s->count; i++)
    {
        printf("%d", s->elements[i]);
    }
}

void init_i(int_stack *s)
{
    s->count = 0;
    s->index_top = 0;
}
//END OF int_stack

//BEGIN OF decision_pointer_stack

typedef struct rule_s
{
    int id;
    char nonterminal;
    char expression[100];
} rule;

struct decision
{
    char_stack *current;
    char_stack *output;
    int_stack *steps;
    int grammar_index;

    decision *next;
};

struct decision_pointer_stack
{
    decision *top;    
    int count;
};
void init_d(decision_pointer_stack *s){
    s->top = NULL;
    s->count = 0;
}

decision *pop_d(decision_pointer_stack *s)
{
    decision *temp = s->top;
    if(temp != NULL){
        s->top = temp->next;
        s->count--;
    }
    return temp;
}

void clear(decision_pointer_stack *s)
{
    decision *de = pop_d(s);
    while(de != NULL)
    {
        free(de->current);
        free(de->output);
        free(de->steps);

        free(de);

        de = pop_d(s);
    }
}

void push_d(decision_pointer_stack *s, decision *param)
{
    if (s->top == NULL)
    {
        s->top = param;
        s->top->next = NULL;
    }
    else
    {
        param->next = s->top;
        s->top = param;
    }
    s->count++;
}

//END OF decision_pointer_stack

typedef struct str_s
{
    char *str;
    int length;
} str;

rule rules[100];
str inputs[100];
int ruleCount = 0;
int inputCount = 0;
int inputSequence = 0;

char terminal[100];
int terminalCount = 0;
char nonterminal[100];
int nonterminalCount = 0;

char initialNonTerminal;

int isItTerminal(char);
int isItNonTerminal(char);
int isItOneTerminal(char *);

void createRule(int, char, char const *);
void createInput(char *);
void process();
int strLen(char *);
void nextInput();
void eliminateRules();
void readFiles();
void writeToFile();

void clearall();

struct char_stack current;
struct char_stack output;
struct int_stack steps;
struct int_stack ignored_grammers;
struct decision_pointer_stack decisions;

int inputLength = 0;
char *input;
int countOfProcess = 0;
int finalFlag = 0;

int main()
{
    //createInput("(((((1+0*1+1+1+1+0)))))"); seg fault
    //createInput("(((((1+0*1+1+1+1+0)))))"); seg fault
    
    init_i(&ignored_grammers);
    init_i(&steps);
    init_c(&current);
    init_c(&output);
    init_d(&decisions);

    readFiles();
    nextInput();
    return 0;
}

void readFiles()
{
    char c;
    // char index[10];
    int indexCount = 0;
    FILE *words, *grammar;

    if ((words = fopen("words.txt", "r")) == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }
    while (!feof(words))
    {
        char *string = (char *)malloc(sizeof(20));
        fscanf(words, "%s", string);
        if (feof(words))
            break;
        createInput(string);
    }
    fclose(words);
    if ((grammar = fopen("grammar.txt", "r")) == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }

    do
    {
        c = fgetc(grammar);
        if (c == '\n')
            break;
        if (c != ' ')
        {
            terminal[terminalCount] = c;
            terminalCount++;
        }
    } while (c != '\n');

    do
    {
        c = fgetc(grammar);
        if (c == '\n')
            break;
        if (c != ' ')
        {
            nonterminal[nonterminalCount] = c;
            nonterminalCount++;
        }
    } while (c != '\n');
    while (!feof(grammar))
    {
        char *arr2 = (char *)malloc(sizeof(100));
        char *string2 = (char *)malloc(sizeof(2));
        char *exp = (char *)malloc(sizeof(100));
        char nonT;
        int index;
        int i = 0, x = 0;

        if (feof(grammar))
            break;
        fscanf(grammar, "%d %c %s %[^\n]\n)", &index, &nonT, string2, exp);

        //initial non terminal
        if (index == 1)
        {
            initialNonTerminal = nonT;
        }

        for (i = 0; exp[i] != '\0'; i++)
        {
            if (exp[i] == ' ')
                continue;
            arr2[x] = exp[i];
            x++;
        }
        arr2[x] = '\0';
        indexCount++;
        createRule(index, nonT, arr2);

        free(arr2);
        free(exp);
        free(string2);
    }
    fclose(grammar);
}

void writeToFile()
{
    FILE *file = fopen("output.txt", "a+");
    if (file != NULL)
    {
        if (finalFlag == 1)
        {
            int i = 0;
            fprintf(file, "YES\n");

            for (i = 0; i < steps.count; i++)
            {
                fprintf(file, "%d", steps.elements[i]);

                if (i != steps.count - 1)
                    fprintf(file, " ");
            }
            fprintf(file, "\n");
        }
        else
        {
            fprintf(file, "NO\n");
        }

        fclose(file);
    }
}

void eliminateRules()
{
    int i = 0, j = 0, k = 0;

    for (i = 0; i < ruleCount; i++)
    {
        if (contains_i(&ignored_grammers, i) == -1)
        {
            int expLen = strLen(rules[i].expression);

            for (j = 0; j < expLen; j++)
            {
                if (isItTerminal(rules[i].expression[j]) != -1)
                {
                    int find = -1;

                    for (k = 0; k < inputLength; k++)
                    {
                        if (rules[i].expression[j] == input[k])
                        {
                            find = 1;
                            break;
                        }
                    }

                    if (find == -1 && contains_i(&ignored_grammers, i) == -1)
                    {
                        push_i(&ignored_grammers, i);
                    }
                }
            }
        }
    }
}

void nextInput()
{
    do
    {
        countOfProcess = 0;
        str i = inputs[inputSequence++];
        printf("[INFO] Started to INPUT %s\n", i.str);
        input = i.str;
        inputLength = i.length;
        push_c(&current, initialNonTerminal);
        eliminateRules();
        clock_t start, end;
        start = clock(); //start clock
        process();
        end = clock(); //end clock

        if (finalFlag == 0)
        {
            writeToFile();
            printf("[WARN] No result\n");
        }

        double duration = (end - start) / (double)CLOCKS_PER_MS; //calculate the total duration

        clearall();

        printf("[INFO] Total duration : %f\n", duration);
        printf("[INFO] Count of process %d \n", countOfProcess);

    } while (inputSequence < inputCount);
}
void process()
{
    countOfProcess++;
    //pop until get a nonterminal
    char val;
    int con = -1, i = 0;
    do
    {
        if (current.count > 0)
        {
            val = pop_c(&current);
            con = isItNonTerminal(val);
            if (con == -1)
            {
                push_c(&output, val);
            }
        }
    } while (con == -1 && current.count > 0);

    if (con == -1)
    {
        if (output.count == inputLength)
        {
            if (compare(&output, input, inputLength) == 1)
            {
                printf("[SUCC] Steps that found: \n");
                printf("[SUCC] ");
                print_i(&steps);
                printf("\n");

                finalFlag = 1;  

                writeToFile();

                clear(&decisions);
                return;
            }
        }
    }
    else
    {
        //before anything checkout if there exists any output and that same as real input
        int checkpoint = 1;
        if (output.count > 0)
        {
            checkpoint = compareUntil(&output, input, inputLength) == -1 ? 0 : 1;
        }

        if (checkpoint == 1)
        {
            //get all grammars belongs to that terminal
            for (i = 0; i < ruleCount; i++)
            {
                if (contains_i(&ignored_grammers, i) == -1 && rules[i].nonterminal == val)
                {
                    if (current.count + output.count >= inputLength - 1 && isItOneTerminal(rules[i].expression) != -1)
                        continue;

                    decision *dec = (decision *)malloc(sizeof(decision));
                    char_stack *current_clone = (char_stack *)malloc(sizeof(char_stack));
                    char_stack *output_clone = (char_stack *)malloc(sizeof(char_stack));
                    int_stack *steps_clone = (int_stack *)malloc(sizeof(int_stack));

                    clone_c(&current, current_clone);
                    clone_c(&output, output_clone);
                    clone_i(&steps, steps_clone);

                    dec->current = current_clone;
                    dec->output = output_clone;
                    dec->steps = steps_clone;

                    dec->grammar_index = i;
                    push_d(&decisions, dec);
                }
            }
        }
    }

    if (decisions.count > 0)
    {
        decision *de = pop_d(&decisions);
        clone_c(de->current, &current);
        clone_c(de->output, &output);
        clone_i(de->steps, &steps);

        int index = de->grammar_index;

        free(de->current);
        free(de->output);
        free(de->steps);

        free(de);

        push_i(&steps, rules[index].id);
        int strlen = strLen(rules[index].expression);
        for (i = strlen - 1; i >= 0; i--)
        {
            push_c(&current, rules[index].expression[i]);
        }

        process();
    }
}
int strLen(char *input)
{
    int i = 0;
    for (i = 0; input[i] != '\0'; ++i)
    {
    }
    return i;
}
int isItOneTerminal(char *input)
{
    int result = -1;

    if (input[1] == '\0')
    {
        result = isItNonTerminal(input[0]);
    }

    return result;
}
int isItNonTerminal(char key)
{
    int result = -1, i = 0;
    for (i = 0; i < nonterminalCount; ++i)
    {
        if (nonterminal[i] == key)
        {
            result = i;
            break;
        }
    }
    return result;
}

int isItTerminal(char key)
{
    int result = -1, i = 0;
    for (i = 0; i < terminalCount; ++i)
    {
        if (terminal[i] == key)
        {
            result = i;
            break;
        }
    }
    return result;
}

void createRule(int id, char terminal, char const *exp)
{
    rule r;
    r.id = id;
    r.nonterminal = terminal;
    strcpy(r.expression, exp);
    rules[ruleCount++] = r;
}

void createInput(char *in)
{
    str i;
    i.str = in;
    i.length = strLen(in);
    inputs[inputCount++] = i;
}

void clearall()
{
    current.index_top = 0;
    current.count = 0;
    output.index_top = 0;
    output.count = 0;
    steps.index_top = 0;
    steps.count = 0;
    ignored_grammers.index_top = 0;
    ignored_grammers.count = 0;
    finalFlag = 0;
}
