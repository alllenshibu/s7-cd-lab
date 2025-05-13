#include <stdio.h>
#include <stdlib.h>

struct node
{
    int state;
    struct node *next;
};

void insert_transition(struct node *transition[], int f, char o, int t)
{
    struct node *temp = (struct node *)malloc(sizeof(struct node));
    temp->state = t;
    temp->next = transition[f];  // Insert at the head of the list for state f
    transition[f] = temp;
}

void find_e_closure(struct node *transition[], struct node *e_closure[], int state, int visited[])
{
    if (visited[state])
        return;  // If already visited, return to avoid infinite recursion

    visited[state] = 1;

    // Add the current state to its own e-closure (if not already added)
    struct node *temp = (struct node *)malloc(sizeof(struct node));
    temp->state = state;
    temp->next = e_closure[state];
    e_closure[state] = temp;

    // Traverse the epsilon transitions (transition[state])
    temp = transition[state];
    while (temp != NULL)
    {
        find_e_closure(transition, e_closure, temp->state, visited);
        temp = temp->next;
    }
}

int main()
{
    int NUMBER_OF_ALPHABETS = 0;
    printf("Enter the number of alphabets: ");
    scanf("%d", &NUMBER_OF_ALPHABETS);

    char alphabets[NUMBER_OF_ALPHABETS];

    printf("Enter the alphabets: ");
    for (int i = 0; i < NUMBER_OF_ALPHABETS; i++)
        scanf(" %c", &alphabets[i]);

    int NUMBER_OF_STATES = 0;
    printf("Enter the number of states: ");
    scanf("%d", &NUMBER_OF_STATES);

    int NUMBER_OF_FINAL_STATES = 0;
    printf("Enter the number of final states: ");
    scanf("%d", &NUMBER_OF_FINAL_STATES);

    int NUMBER_OF_TRANSITIONS = 0;
    printf("Enter the number of transitions: ");
    scanf("%d", &NUMBER_OF_TRANSITIONS);

    // Initialize the arrays to NULL
    struct node *transition[NUMBER_OF_STATES];
    struct node *e_closure[NUMBER_OF_STATES];
    
    for (int i = 0; i < NUMBER_OF_STATES; i++)
    {
        transition[i] = NULL;
        e_closure[i] = NULL;
    }

    printf("Enter the transitions (from_state epsilon to_state): \n");
    for (int i = 0; i < NUMBER_OF_TRANSITIONS; i++)
    {
        int f, t;
        char o;
        scanf("%d %c %d", &f, &o, &t);

        insert_transition(transition, f, o, t);  // Insert epsilon transition
    }

    printf("\ne-Closures\n");
    for (int i = 0; i < NUMBER_OF_STATES; i++)
    {
        int visited[NUMBER_OF_STATES];  // Visited array for each state
        for (int j = 0; j < NUMBER_OF_STATES; j++)
            visited[j] = 0;  // Initialize visited array to 0 (unvisited)

        printf("e-closure(q%d): ", i);
        find_e_closure(transition, e_closure, i, visited);

        struct node *temp = e_closure[i];
        while (temp != NULL)
        {
            printf("q%d ", temp->state);
            temp = temp->next;
        }
        printf("\n");
    }

    return 0;
}
