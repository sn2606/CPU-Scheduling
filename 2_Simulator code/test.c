# include <stdio.h>
# include <stdlib.h>
# include "simulator.h"


int main()
{

    char *input1 = (char *)malloc(50 * sizeof(char));
    input1 = "final_not_priority.txt";
    char input11[] = "final_not_priority.txt";
    char *input2 = (char *)malloc(50 * sizeof(char));
    input2 = "final_priority.txt";
    char input21[] = "final_priority.txt";

    // PS_NON_PREEMPTIVE(input2);

    // SJF_PREEMPTIVE(input21);

    return 0;
}