/*********************************************************************************************************************************
 *                                       C PROGRAM TO SIMULATE FIRST COME FIRST SERVE SCHEDULING                                 *
 * ******************************************************************************************************************************/

# include <stdio.h>
# include <stdlib.h>
# define N 20
# define L 25

//structure for all the processes
struct Process
{
	int pid;      // process id
	int priority; // priority
	int AT;       // arrival time
	int BT;       // burst time
	int WT;       // waiting time
    int TAT;      // turnaround time
	int RT;       // remaining time
	int CH;       // check variable
};

void get_input(struct Process **ready);
void sort_arrival_time(struct Process **ready);
void display_queue(struct Process **p, int size, int num);
void wait_time_FCFS(struct Process *p, int n);
void turn_time_FCFS(struct Process *p, int n);

int main()
{
    
    int i, j; //status, i;

    struct Process **ready;

    ready = (struct Process **)malloc(L * sizeof(struct Process *));

    for(i = 0; i < L; i++)
    {
        ready[i] = (struct Process *)malloc(N * sizeof(struct Process));
    }

    printf("\n----- FIRST COME FIRST SERVE SCHEDULING -----\n\n");
    

    get_input(ready);
    sort_arrival_time(ready);


    for(i = 0; i < L; i++)
    {
        wait_time_FCFS(ready[i], N);
        turn_time_FCFS(ready[i], N);
    }  
    

    float *sum_wait = (float *)calloc(L, sizeof(float));
    float *sum_tat = (float *)calloc(L, sizeof(float));
    float *avg_wait = (float *)calloc(L, sizeof(float));
    float *avg_tat = (float *)calloc(L, sizeof(float));
    float *total_time = (float *)calloc(L, sizeof(float));
    float *throughput = (float *)calloc(L, sizeof(float));
    float *cpu_idle_time = (float *)calloc(L, sizeof(float));
    float *cpu_utilization = (float *)calloc(L, sizeof(float));

    for(i = 0; i < L; i++)
    {
        for(j = 0; j < N; j++)
        {
            sum_tat[i]  += ready[i][j].TAT;
            sum_wait[i] += ready[i][j].WT;
            if(total_time[i] < ready[i][j].AT)
            {
                cpu_idle_time[i] += (ready[i][j].AT - total_time[i]);
            }
            total_time[i] += ready[i][j].BT;
        }


        cpu_utilization[i] = (total_time[i])/(total_time[i] + cpu_idle_time[i]);
        avg_wait[i] = sum_wait[i] / ((float) N);
        avg_tat[i]  = sum_tat[i]  / ((float) N);
        throughput[i]  = (float)N / total_time[i];
    }


    // Output files

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
            fprintf(fp1, "%f,", avg_wait[i]);
            fprintf(fp2, "%f,", avg_tat[i]);
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
        for(i = 0; i < N; i++)
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


// According to arrival time, sort the queue
void sort_arrival_time(struct Process **ready)
{
	int h, i, j;
    struct Process temp;

    
    for(h = 0; h < L; h++)
    {
        for(i = 0; i < N; i++)
        {
            for(j = i + 1; j < N; j++)
            {
                if(ready[h][i].AT > ready[h][j].AT)
                {
                    temp        = ready[h][i];
                    ready[h][i] = ready[h][j];
                    ready[h][j] = temp;
                }
                else if (ready[h][i].AT == ready[h][j].AT && ready[h][i].pid > ready[h][j].pid)
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
void display_queue(struct Process **p, int size, int num)
{
    int i;    
    
    printf("\n------------------------------------\n");
    printf(" PID\t AT \t BT \tPrio\t RT \n");
    printf("------------------------------------\n");

    for(i = 0; i < size; i++)
    {
        printf("%4d\t%4d\t%4d\t%4d\t%4d\n", p[num][i].pid, p[num][i].AT, p[num][i].BT, p[num][i].priority, p[num][i].RT);
    }
}


// Function to calculate wait time of all the processes
void wait_time_FCFS(struct Process *p, int n)
{
    int i;// ind;

    p[0].WT = 0;
    p[0].CH = 1;
    p[0].RT = 0;

    for(i = 1; i < n; i++)
    {
        p[i].CH = 1;
        p[i].WT = p[i - 1].WT + p[i - 1].BT - p[i].AT;
        if(p[i].WT < 0)
        {
            p[i].WT = 0;
        }
        p[i].RT = 0;
    }

}

// Function to calculate turnaround time of all the processes
void turn_time_FCFS(struct Process *p, int n)
{
    int i;

    for(i = 0; i < n; i++)
    {
        p[i].TAT = p[i].WT + p[i].BT;
    }
}