#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define STATES 99
#define SYMBOLS 20

int N_symbols;               
int N_DFA_states;            
char DFA_finals[STATES + 1]; 
int DFAtab[STATES][SYMBOLS];

char StateName[STATES][STATES + 1]; 

int N_optDFA_states; 
int OptDFA[STATES][SYMBOLS];
char NEW_finals[STATES + 1];

void print_dfa_table(
    int tab[][SYMBOLS], 
    int nstates,        
    int nsymbols,       
    char *finals)
{
    int i, j;

    puts("\nDFA: STATE TRANSITION TABLE");

    printf("     | ");
    for (i = 0; i < nsymbols; i++)
        printf("  %c  ", '0' + i);

    printf("\n-----+--");
    for (i = 0; i < nsymbols; i++)
        printf("-----");
    printf("\n");

    for (i = 0; i < nstates; i++)
    {
        printf("  %c  | ", 'A' + i); 
        for (j = 0; j < nsymbols; j++)
            printf("  %c  ", tab[i][j]);
        printf("\n");
    }
    printf("Final states = %s\n", finals);
}

void load_DFA_table()
{
    int i, j;

    printf("Enter the number of DFA states: ");
    scanf("%d", &N_DFA_states);

    printf("Enter the number of input symbols: ");
    scanf("%d", &N_symbols);

    printf("Enter the transition table (each row separated by new line):\n");
    for (i = 0; i < N_DFA_states; i++)
    {
        for (j = 0; j < N_symbols; j++)
        {
            char state;
            printf("Transition for state %c with symbol %c: ", 'A' + i, '0' + j);
            scanf(" %c", &state);
            DFAtab[i][j] = state;
        }
    }

    printf("Enter the final states (e.g., 'EF'): ");
    scanf("%s", DFA_finals);
}

void get_next_state(char *nextstates, char *cur_states,
                    int dfa[STATES][SYMBOLS], int symbol)
{
    int i;

    for (i = 0; i < strlen(cur_states); i++)
        *nextstates++ = dfa[cur_states[i] - 'A'][symbol];
    *nextstates = '\0';
}

char equiv_class_ndx(char ch, char stnt[][STATES + 1], int n)
{
    int i;

    for (i = 0; i < n; i++)
        if (strchr(stnt[i], ch))
            return i + '0';
    return -1; 
}

char is_one_nextstate(char *s)
{
    char equiv_class; 

    while (*s == '@')
        s++;
    equiv_class = *s++; 

    while (*s)
    {
        if (*s != '@' && *s != equiv_class)
            return 0;
        s++;
    }

    return equiv_class; 
}

int state_index(char *state, char stnt[][STATES + 1], int n, int *pn,
                int cur) 
{
    int i;
    char state_flags[STATES + 1]; /* next state info. */

    if (!*state)
        return -1; /* no next state */

    for (i = 0; i < strlen(state); i++)
        state_flags[i] = equiv_class_ndx(state[i], stnt, n);
    state_flags[i] = '\0';

    printf("   %d:[%s]\t--> [%s] (%s)\n",
           cur, stnt[cur], state, state_flags);

    if (i = is_one_nextstate(state_flags))
        return i - '0'; /* deterministic next states */
    else
    {
        strcpy(stnt[*pn], state_flags); /* state-division info */
        return (*pn)++;
    }
}

/*
    Divide DFA states into finals and non-finals.
*/
int init_equiv_class(char statename[][STATES + 1], int n, char *finals)
{
    int i, j;

    if (strlen(finals) == n)
    { /* all states are final states */
        strcpy(statename[0], finals);
        return 1;
    }

    strcpy(statename[1], finals); /* final state group */

    for (i = j = 0; i < n; i++)
    {
        if (i == *finals - 'A')
        {
            finals++;
        }
        else
            statename[0][j++] = i + 'A';
    }
    statename[0][j] = '\0';

    return 2;
}

/*
    Get optimized DFA 'newdfa' for equiv. class 'stnt'.
*/
int get_optimized_DFA(char stnt[][STATES + 1], int n,
                      int dfa[][SYMBOLS], int n_sym, int newdfa[][SYMBOLS])
{
    int n2 = n; /* 'n' + <num. of state-division info> */
    int i, j;
    char nextstate[STATES + 1];

    for (i = 0; i < n; i++)
    { /* for each pseudo-DFA state */
        for (j = 0; j < n_sym; j++)
        { /* for each input symbol */
            get_next_state(nextstate, stnt[i], dfa, j);
            newdfa[i][j] = state_index(nextstate, stnt, n, &n2, i) + 'A';
        }
    }

    return n2;
}

/*
    char 'ch' is appended at the end of 's'.
*/
void chr_append(char *s, char ch)
{
    int n = strlen(s);

    *(s + n) = ch;
    *(s + n + 1) = '\0';
}

void sort(char stnt[][STATES + 1], int n)
{
    int i, j;
    char temp[STATES + 1];

    for (i = 0; i < n - 1; i++)
        for (j = i + 1; j < n; j++)
            if (stnt[i][0] > stnt[j][0])
            {
                strcpy(temp, stnt[i]);
                strcpy(stnt[i], stnt[j]);
                strcpy(stnt[j], temp);
            }
}

/*
    Divide first equivalent class into subclasses.
        stnt[i1] : equiv. class to be segmented
        stnt[i2] : equiv. vector for next state of stnt[i1]
    Algorithm:
        - stnt[i1] is splitted into 2 or more classes 's1/s2/...'
        - old equiv. classes are NOT changed, except stnt[i1]
        - stnt[i1]=s1, stnt[n]=s2, stnt[n+1]=s3, ...
    Return value: number of NEW equiv. classses in 'stnt'.
*/
int split_equiv_class(char stnt[][STATES + 1],
                      int i1,    /* index of 'i1'-th equiv. class */
                      int i2,    /* index of equiv. vector for 'i1'-th class */
                      int n,     /* number of entries in 'stnt' */
                      int n_dfa) /* number of source DFA entries */
{
    char *old = stnt[i1], *vec = stnt[i2];
    int i, n2, flag = 0;
    char newstates[STATES][STATES + 1]; /* max. 'n' subclasses */

    for (i = 0; i < STATES; i++)
        newstates[i][0] = '\0';

    for (i = 0; vec[i]; i++)
        chr_append(newstates[vec[i] - '0'], old[i]);

    for (i = 0, n2 = n; i < n_dfa; i++)
    {
        if (newstates[i][0])
        {
            if (!flag)
            { /* stnt[i1] = s1 */
                strcpy(stnt[i1], newstates[i]);
                flag = 1; /* overwrite parent class */
            }
            else /* newstate is appended in 'stnt' */
                strcpy(stnt[n2++], newstates[i]);
        }
    }

    sort(stnt, n2); /* sort equiv. classes */

    return n2; /* number of NEW states(equiv. classes) */
}

/*
    Equiv. classes are segmented and get NEW equiv. classes.
*/
int set_new_equiv_class(char stnt[][STATES + 1], int n,
                        int newdfa[][SYMBOLS], int n_sym, int n_dfa)
{
    int i, j, k;

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n_sym; j++)
        {
            k = newdfa[i][j] - 'A'; /* index of equiv. vector */
            if (k >= n)             /* equiv. class 'i' should be segmented */
                return split_equiv_class(stnt, i, k, n, n_dfa);
        }
    }

    return n;
}

void print_equiv_classes(char stnt[][STATES + 1], int n)
{
    int i;

    printf("\nEQUIV. CLASS CANDIDATE ==>");
    for (i = 0; i < n; i++)
        printf(" %d:[%s]", i, stnt[i]);
    printf("\n");
}

/*
    State-minimization of DFA: 'dfa' --> 'newdfa'
    Return value: number of DFA states.
*/
int optimize_DFA(
    int dfa[][SYMBOLS],      /* DFA state-transition table */
    int n_dfa,               /* number of DFA states */
    int n_sym,               /* number of input symbols */
    char *finals,            /* final states of DFA */
    char stnt[][STATES + 1], /* state name table */
    int newdfa[][SYMBOLS])   /* reduced DFA table */
{
    char nextstate[STATES + 1];
    int n;  /* number of new DFA states */
    int n2; /* 'n' + <num. of state-dividing info> */

    n = init_equiv_class(stnt, n_dfa, finals);

    while (1)
    {
        print_equiv_classes(stnt, n);
        n2 = get_optimized_DFA(stnt, n, dfa, n_sym, newdfa);
        if (n != n2)
            n = set_new_equiv_class(stnt, n, newdfa, n_sym, n_dfa);
        else
            break; /* equiv. class segmentation ended!!! */
    }

    return n; /* number of DFA states */
}

/*
    Check if 't' is a subset of 's'.
*/
int is_subset(char *s, char *t)
{
    int i;

    for (i = 0; *t; i++)
        if (!strchr(s, *t++))
            return 0;
    return 1;
}

/*
    New finals states of reduced DFA.
*/
void get_NEW_finals(
    char *newfinals,         /* new DFA finals */
    char *oldfinals,         /* source DFA finals */
    char stnt[][STATES + 1], /* state name table */
    int n)                   /* number of states in 'stnt' */
{
    int i;

    for (i = 0; i < n; i++)
        if (is_subset(oldfinals, stnt[i]))
            *newfinals++ = i + 'A';
    *newfinals++ = '\0';
}

int main()
{
    load_DFA_table();
    print_dfa_table(DFAtab, N_DFA_states, N_symbols, DFA_finals);

    N_optDFA_states = optimize_DFA(DFAtab, N_DFA_states,
                                   N_symbols, DFA_finals, StateName, OptDFA);
    get_NEW_finals(NEW_finals, DFA_finals, StateName, N_optDFA_states);

    print_dfa_table(OptDFA, N_optDFA_states, N_symbols, NEW_finals);

    return 0;
}

/*
mec@cc-2-2:~/cs7a$ ./dfaminimize.o
Enter the number of DFA states: 5
Enter the number of input symbols: 2
Enter the transition table (each row separated by new line):
Transition for state A with symbol 0: B
Transition for state A with symbol 1: C
Transition for state B with symbol 0: A
Transition for state B with symbol 1: D
Transition for state C with symbol 0: E
Transition for state C with symbol 1: C
Transition for state D with symbol 0: D
Transition for state D with symbol 1: E
Transition for state E with symbol 0: B
Transition for state E with symbol 1: C
Enter the final states (e.g., 'EF'): AE

DFA: STATE TRANSITION TABLE
     |   0    1
-----+------------
  A  |   B    C
  B  |   A    D
  C  |   E    C
  D  |   D    E
  E  |   B    C
Final states = AE

EQUIV. CLASS CANDIDATE ==> 0:[BCD] 1:[AE]
   0:[BCD]      --> [AED] (110)
   0:[BCD]      --> [DCE] (001)
   1:[AE]       --> [BB] (00)
   1:[AE]       --> [CC] (00)

EQUIV. CLASS CANDIDATE ==> 0:[AE] 1:[BC] 2:[D]
   0:[AE]       --> [BB] (11)
   0:[AE]       --> [CC] (11)
   1:[BC]       --> [AE] (00)
   1:[BC]       --> [DC] (21)
   2:[D]        --> [D] (2)
   2:[D]        --> [E] (0)

EQUIV. CLASS CANDIDATE ==> 0:[AE] 1:[B] 2:[C] 3:[D]
   0:[AE]       --> [BB] (11)
   0:[AE]       --> [CC] (22)
   1:[B]        --> [A] (0)
   1:[B]        --> [D] (3)
   2:[C]        --> [E] (0)
   2:[C]        --> [C] (2)
   3:[D]        --> [D] (3)
   3:[D]        --> [E] (0)

DFA: STATE TRANSITION TABLE
     |   0    1
-----+------------
  A  |   B    C
  B  |   A    D
  C  |   A    C
  D  |   D    A
Final states = A
*/