# include <stdio.h> 
# include <stdlib.h> 
# define N 20
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
void sort_arrival_time(struct Process **ready);
void display_queue(struct Process *p, int size);

int main() 
{ 
	int i, limit, x, time_quantum, j; 
	int total[L] = {0}, counter[L] = {0}, wait_time[L] = {0}, turnaround_time[L] = {0};

	int arrival_time[L][N]; 
	int burst_time[L][N]; 
	int temp[L][N]; 
	
	float average_wait_time[L] = {0}; 
	float average_turnaround_time[L] = {0};
	float throughput[L] = {0};
	float total_time[L] = {0};

	struct Process **p = (struct Process **)malloc(L * sizeof(struct Process *));

	for(i = 0; i < L; i++)
	{
		p[i] = (struct Process *)malloc(N * sizeof(struct Process));
	}


	printf("\t\t ROUND ROBIN SCHEDULING \n");

	get_input(p);
	sort_arrival_time(p);
	//display_queue(p[9], N);

	for (i = 0; i < L; i++) 
	{
		for(j = 0; j < N; j++)
		{
			arrival_time[i][j] = p[i][j].AT;
			burst_time[i][j]   = p[i][j].BT;
			temp[i][j]         = burst_time[i][j];
			total_time[i]     += p[i][j].BT;
		}
	}

	time_quantum = 4;
	
	//printf("\nProcess ID\t\tBurst Time\t Turnaround Time\t Waiting Time\n");

	for(i = 0; i < L; i++)
	{	
		limit = N;

		x = N;

		wait_time[i] = p[i][0].AT;
		total[i]     = p[i][0].AT;

		for(j = 0; x != 0;) 
		{ 
			if(temp[i][j] <= time_quantum && temp[i][j] > 0) 
			{ 
				total[i] = total[i] + temp[i][j]; 
				temp[i][j] = 0; 
				counter[i] = 1; 
			} 
			else if(temp[i][j] > 0) 
			{ 
				temp[i][j] = temp[i][j] - time_quantum; 
				total[i] = total[i] + time_quantum; 
			} 
			if(temp[i][j] == 0 && counter[i] == 1) 
			{ 
				x--; 
				//printf("\nProcess[%d]\t\t%d\t\t %d\t\t\t %d", p[i].pid, burst_time[i], total - arrival_time[i], total - arrival_time[i] - burst_time[i]);
				wait_time[i] = wait_time[i] + total[i] - arrival_time[i][j] - burst_time[i][j]; 
				turnaround_time[i] = turnaround_time[i] + total[i] - arrival_time[i][j]; 
				counter[i] = 0; 
			} 
			if(j == limit - 1) 
			{
				j = 0; 
			}
			else if(arrival_time[i][j + 1] <= total[i]) 
			{
				j++;
			}
			else 
			{
				j = 0;
			}
		}

		average_wait_time[i] = ((float) wait_time[i]) / limit;
		average_turnaround_time[i] = ((float) turnaround_time[i]) / limit;
		throughput[i] = ((float) N) / total_time[i];
	
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
            fprintf(fp1, "%f,", average_wait_time[i]);
            fprintf(fp2, "%f,", average_turnaround_time[i]);
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