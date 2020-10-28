/*********************************************************************************************************************************
 *                           C PROGRAM TO SIMULATE SHORTEST JOB FIRST SCHEDULING  NON - PREEMPTIVE                               *
 * ******************************************************************************************************************************/

# include <stdio.h>
# include <stdlib.h>
# define n 20
# define L 25

// Structure for all the processes
struct Process
{
    int pid;      // Process ID
    int priority; // Process Priority, -1 since not applicable
    int AT;       // Process arrival time
    int BT;       // Process burst time
    int RT;       // Process remaining time
    int WT;       // Process waiting time
    int TAT;      // Process turnaround time
    int CH;       // Check / Status variable
};

void get_input(struct Process **ready);
void sort_arrival_burst_time(struct Process **ready);
void display_queue(struct Process *p, int size);
void wait_time_SJFNP(struct Process *p);
void turn_time_SJFNP(struct Process *p);


// Main Function
int main()
{

    int i, j, count, start; 
    int time[L] = {0}, completed[L] = {0}, sum_wait[L] = {0}, sum_turnaround[L] = {0}, total_time[L] = {0};
    float avgwait[L], avgturn[L], throughput[L];

    struct Process **p = (struct Process **)malloc(L * sizeof(struct Process *));

    for(i = 0; i < L; i++)
    {
        p[i] = (struct Process *)malloc(n * sizeof(struct Process));
    }

    get_input(p);
    sort_arrival_burst_time(p);
    //display_queue(p[6], n);

    // printf("\n\nProcess\tArrival time\tBurst time\tStart time\tEnd time\tWaiting time\tTurnaround time\n");

    for(i = 0; i < L; i++)
    {
        for(j = 0; j < n; j++)
        {
            total_time[i] += p[i][j].BT;
        }
    }

    for(i = 0; i < L; i++)
    {
        time[i] = p[i][0].AT;

        wait_time_SJFNP(p[i]);
        turn_time_SJFNP(p[i]);

        while(completed[i] < n)
        {
            count = 0;

            for(j = completed[i]; j < n; j++)
            {
                if(p[i][j].AT <= time[i])
                {
                    count++;
                }
                else
                {
                    break;
                }
            }

            start = time[i];
            time[i]  += p[i][completed[i]].BT;

            //printf("   %d\t     %d\t\t     %d\t\t     %d\t\t     %d\t\t     %d\t\t     %d\n", p[completed].pid, p[completed].AT, p[completed].BT, start, time, time-p[completed].AT-p[completed].BT,time-p[completed].AT);
            sum_wait[i]       += time[i] - p[i][completed[i]].AT -p[i][completed[i]].BT;
            sum_turnaround[i] += time[i] - p[i][completed[i]].AT;
            completed[i]++;
        }

        avgwait[i] = ((float)sum_wait[i]) / ((float) n);
        avgturn[i] = ((float)sum_turnaround[i]) / ((float) n);
        throughput[i] = ((float) n) / ((float) total_time[i]);

        // printf("Average waiting time is %f\n",avgwait[i]);
        // printf("Average turnaround time is %f\n",avgturn[i]);
        // printf("Throughput %d = %f\n\n", i+1, throughput[i]);
    }

    FILE *fp1, *fp2, *fp3;

    fp1 = fopen("op_avg_wt.csv", "a+");
    fp2 = fopen("op_avg_tat.csv", "a+");
    fp3 = fopen("op_throughput.csv", "a+");

    if(fp1 == NULL || fp2 == NULL || fp3 == NULL)
    {
        perror("ERROR: No file to be read");
        exit(1);
    }
    else
    {
        for(i = 0; i < L; i++)
        {
            fprintf(fp1, "%f,", avgwait[i]);
            fprintf(fp2, "%f,", avgturn[i]);
            fprintf(fp3, "%f,", throughput[i]);
        }
        fprintf(fp1, "\n");
        fprintf(fp2, "\n");
        fprintf(fp3, "\n");
    }

    fclose(fp1);
    fclose(fp2);
    fclose(fp3);

    return 0;
}


// Function to read the input from the input file
void get_input(struct Process **ready)
{
    int i, j;
    FILE *fp;
    fp = fopen("final_not_priority.txt", "r");

    if(fp == NULL)
    {
        perror("ERROR: No file to be read");
        exit(1);
    }
    else
    {
        for(i = 0; i < n; i++)
        {
            for(j = 0; j < L; j++)
            {            
                fscanf(fp, "%d ", &(ready[j][i].pid));
                fscanf(fp, "%d ", &(ready[j][i].priority));
                fscanf(fp, "%d ", &(ready[j][i].BT));
                fscanf(fp, "%d\n", &(ready[j][i].AT));
                ready[j][i].CH = 0;
                ready[j][i].WT = 0;
                ready[j][i].RT = ready[j][i].BT;
            }
        }
    }
    fclose(fp);
}


// According to arrival time & then burst time, sort the queue
void sort_arrival_burst_time(struct Process **ready)
{
	int h, i, j;
    struct Process temp;

    
    for(h = 0; h < L; h++)
    {
        for(i = 0; i < n; i++)
        {
            for(j = i + 1; j < n; j++)
            {
                if(ready[h][i].AT > ready[h][j].AT)
                {
                    temp        = ready[h][i];
                    ready[h][i] = ready[h][j];
                    ready[h][j] = temp;
                }
                else if (ready[h][i].AT == ready[h][j].AT && ready[h][i].BT > ready[h][j].BT)
                {
                    temp        = ready[h][i];
                    ready[h][i] = ready[h][j];
                    ready[h][j] = temp;
                }

            }            
        }
	}
}


// Function to display a process queue
void display_queue(struct Process *p, int size)
{
    int i;
    printf("\n------------------------------------\n");
    printf(" PID\t AT \t BT \tPrio\t RT \n");
    printf("------------------------------------\n");

    for(i = 0; i < size; i++)
    {
        printf("%4d\t%4d\t%4d\t%4d\t%4d\n", p[i].pid, p[i].AT, p[i].BT, p[i].priority, p[i].RT);
    }
}


// Function to calculate wait time of all the processes
void wait_time_SJFNP(struct Process *p)
{
    int i;

    p[0].WT = 0;
    p[0].RT = 0;

    for(i = 1; i < n; i++)
    {
        p[i].WT = p[i - 1].WT + p[i - 1].BT - p[i].AT;
        if(p[i].WT < 0)
        {
            p[i].WT = 0;
        }
        p[i].RT = 0;
    }

}

// Function to calculate turnaround time of all the processes
void turn_time_SJFNP(struct Process *p)
{
    int i;

    for(i = 0; i < n; i++)
    {
        p[i].TAT = p[i].WT + p[i].BT;
    }
}