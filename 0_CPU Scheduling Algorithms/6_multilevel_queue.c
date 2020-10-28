/*********************************************************************************************************************************
 *                                       C PROGRAM TO SIMULATE MULTILEVEL QUEUE SCHEDULING                                       *
 * ******************************************************************************************************************************/

# include <stdio.h>
# include <stdlib.h>
# define N 20
# define L 25
# define n 20

//structure for all the processes
struct Process
{
	int pid;      // process id
	int priority; // priority
	int AT;       // arrival time
	int BT;       // burst time
	int WT;       // waiting time
	int RT;       // remaining time
	int CH;       // check variable
	int TAT;      // turnaround timw
};

/*****************************************************************
 * System process queue - Priority Scheduling (Preemptive)       *
 * Interactive process queue - Round Robin Scheduling            *
 * Fixed priority preemptive scheduling between the queues       *
 *****************************************************************/

int *scount, *icount;
struct Process **syst, **inte;

void get_input(struct Process **ready);
void sort_arrival_priority_time(struct Process **ready);
void sort_priority(struct Process *ready, int size);
void sort_pid(struct Process *p, int size);
void display_queue(struct Process *p, int size);
void copy_struct_info(struct Process *p, struct Process *q);
void level_queueing(struct Process **ready);

/****************************************************************************************************************/

//main
int main()
{	
	struct Process current;
	int i, j, k, h, diff;
	
	int cpuState = 0; //CPU busy = 1 CPU idle = 0
	int QT = 2 ;      //Time Quantum for process (RR Scheduling)

	current.pid      = -1;
	current.priority = 1000000;

	int t  = 0;       // time counter variable
	int PP_flag = 0;
	int RR_flag = 0;
	float throughput[L];

	struct Process **ready;

    ready = (struct Process **)malloc(L * sizeof(struct Process *));

    for(i = 0; i < L; i++)
    {
        ready[i] = (struct Process *)malloc(N * sizeof(struct Process));
    }

	struct Process interactive_q[L][n];  	 // priority queue
	struct Process system_q[L][n];    // round robin queue
	int pqcount = -1, rbcount = -1;

	get_input(ready);
	
	sort_arrival_priority_time(ready);

	display_queue(ready[3], n);

	//level_queueing(ready);

	FILE *fp1, *fp2, *fp3;

    fp1 = fopen("op_avg_wt.csv", "a+");
    fp2 = fopen("op_avg_tat.csv", "a+");
    fp3 = fopen("op_throughput.csv", "a+");

    if(fp1 == NULL || fp2 == NULL || fp3 == NULL)
    {
        perror("ERROR: No file to be read");
        exit(1);
    }
	
	for(h = 0; h < L; h++)
	{	
		int Time_taken = 0;  // calculate total time for creating gantt chart
		
		Time_taken = Time_taken + ready[h][0].AT;
		
		for(i = 0; i < n; i++ )
		{
			if( Time_taken >= ready[h][i].AT )
			{
				Time_taken = Time_taken + ready[h][i].BT;
			}
			else
			{
				// If no process has arrived yet, CPU will be idle
				diff       = (ready[h][i].AT - Time_taken);
				Time_taken = Time_taken + diff + ready[h][i].BT;
			}
		}

		throughput[h] = ((float) N) / Time_taken;

		// Ghantt chart initialization. -1 
		int *Ghantt = (int *)malloc((Time_taken + 1) * sizeof(int));

		for(i = 0; i < Time_taken; i++ )
		{
			Ghantt[i] = -1;
		}

		// Considering integer readys
		for (t = 0; t <= Time_taken; t++)
		{
			for(i = 0; i < n; i++) //Insert the process in Priority Queue whose arrival time matches with time clock
			{
				if(t == ready[h][i].AT)
				{
					pqcount++;   // Increasing the count of processes in the priority queue

					copy_struct_info(&ready[h][i], &interactive_q[h][pqcount]);
					
					sort_priority(interactive_q[h], pqcount + 1);// sort by priority
				}
			}
			
			if(cpuState == 0) //If CPU is idle
			{
				if(pqcount + 1 != 0)
				{
					current = interactive_q[h][0]; // Dequeue the first object in priority queue
					for(i = 1; i < pqcount+1; i++)
					{
						interactive_q[h][i-1] = interactive_q[h][i]; // Expensive dequeueing operation
					}
					pqcount--;
					cpuState = 1;
					PP_flag  = 1;
					//Q=2;
				}
				else if(rbcount + 1 != 0)
				{
					current = system_q[h][0]; // Dequeueing the first object in round robin queue
					for(i=1;i<rbcount+1;i++)
					{
						system_q[h][i-1] = system_q[h][i];
					}
					rbcount--;
					cpuState = 1;
					RR_flag  = 1;
					QT       = 2;
				}
			}
			else if(cpuState == 1) //If cpu is not idle
			{
				if(PP_flag == 1 && (pqcount+1!=0))
				{
					if(interactive_q[h][0].priority < current.priority )
					{
						rbcount++;

						copy_struct_info(&current, &system_q[h][rbcount]);
						
						current = interactive_q[h][0];
						// popping out the first object in priority queue
						for(i = 1; i < pqcount+1; i++)
						{
							interactive_q[h][i-1] = interactive_q[h][i];
						}
						pqcount--;
						//QT = 2; 
					}
				}
				else if(RR_flag == 1 && (pqcount + 1 != 0)) //If process is from RQ and new process comes in PQ
				{
					rbcount++;

					copy_struct_info(&current, &system_q[h][rbcount]);
					
					current = interactive_q[h][0]; //Dequeueing  the first object in priority queue
					for(i = 1; i < pqcount + 1; i++)
					{
						interactive_q[h][i-1] = interactive_q[h][i];
					}
					pqcount--;

					RR_flag = 0;
					PP_flag = 1;
					QT = 2;
				}
			}


			if(current.pid != -1) // Process is in Execution
			{
				current.RT--;
				QT--;
				Ghantt[t] = current.pid;
				if(current.RT == 0) //If process Finish
				{
					cpuState = 0 ;
					if(PP_flag == 0 && RR_flag == 1)
						QT = 2 ;
					current.pid      = -1;
					current.priority = 1000000 ;
					RR_flag = 0;
					PP_flag = 0;
				}
				// else if(QT==0)   //if you want to apply Quantum Time for Queue1 also uncomment this
				else if(QT == 0 && PP_flag==0 && RR_flag==1) //If time Qunatum of a current running process Finish
				{
					// push into system_q queue : it will excute acc to round robin 
					rbcount++;

					copy_struct_info(&current, &system_q[h][rbcount]);

					current.pid      = -1;
					current.priority = 1000000;

					RR_flag = 0;
					PP_flag = 0;
					cpuState=0;			
				}
			}
		}
		
		int cpu_idle_time = 0;
		
		// calculations for wait time of all processes using the ghantt chart
		for(i = 0; i <= Time_taken; i++)
		{ 
			for(j = 0; j < n; j++)
			{
				if(Ghantt[i] == ready[h][j].pid && ready[h][j].CH==0)
				{
					ready[h][j].CH = 1;   // Check variable . process is checked of for wait time calculation
					int ft         = i + 1; // Finish time of ghantt interval
					int st         = i;   // Start time of ghantt interval
					int wt      = st - ready[h][j].AT;

					for(k = i + 1; k <= Time_taken; k++)
					{
						if(Ghantt[k] == ready[h][j].pid)
						{
							wt += (k - ft);
							ft = k+1;
						}
					}

					ready[h][j].WT = wt;
				}
			}

			if(Ghantt[i] == -1)
				cpu_idle_time += 1;
		}

		sort_pid(ready[h], n);  // sort to display according to pid of processes
		float sumWT = 0, sumTA = 0;
		for(i = 0; i < n; i++)
		{
			sumWT += ready[h][i].WT;
			sumTA += ready[h][i].WT + ready[h][i].BT;
		}

		fprintf(fp1, "%f,", (sumWT)/((float)n));
        fprintf(fp2, "%f,", sumTA/((float)n));
        fprintf(fp3, "%f,", throughput[h]);
	
	}

	fprintf(fp1, "\n");
    fprintf(fp2, "\n");
    fprintf(fp3, "\n");

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
    fp = fopen("final_priority.txt", "r");

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
void sort_arrival_priority_time(struct Process **ready)
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
                else if (ready[h][i].AT == ready[h][j].AT && ready[h][i].priority > ready[h][j].priority)
                {
                    temp        = ready[h][i];
                    ready[h][i] = ready[h][j];
                    ready[h][j] = temp;
                }
                
            }
        }
    }
}

//According to priority , sort the queue
void sort_priority(struct Process *ready, int size)
{
	int i,j;
	struct Process temp;

	for(i = 0; i < size; i++)
	{
		for(j = i + 1; j < size; j++)
		{
			if(ready[j].priority < ready[i].priority)
			{
				temp     = ready[j];
				ready[j] = ready[i];
				ready[i] = temp;
			}
		}
	}
}

//According to pid , sort the queue
void sort_pid(struct Process *p, int size)
{
	int i, j;
    struct Process temp;

    for(i = 0; i < size; i++)
    {
        for(j = i + 1; j < size; j++)
        {
            if(p[i].priority > p[j].priority)
            {
                temp = p[i];
                p[i] = p[j];
                p[j] = temp;
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

// Function to copy one process information to another struct
void copy_struct_info(struct Process *p, struct Process *q)
{
	q->AT  = p->AT;
	q->BT  = p->BT;
	q->CH  = p->CH;
	q->pid = p->pid;
	q->priority = p->priority;
	q->RT  = p->RT;
	q->WT  = p->WT;
}

// Function to sort the ready queue into system, interactive and batch process queues
void level_queueing(struct Process **ready)
{
    int i, j;

	scount = (int *)calloc(L, sizeof(int));
	icount = (int *)calloc(L, sizeof(int));

	syst = (struct Process **)malloc(L * sizeof(struct Process*));
	inte = (struct Process **)malloc(L * sizeof(struct Process*)); 

	for(i = 0; i < L; i++)
	{
		for(j = 0; j < n; j++)
		{
			if(ready[i][j].priority >= 1 && ready[i][j].priority <= 10)
			{
				scount[i]++;
			}
			else if (ready[i][j].priority >= 11 && ready[i][j].priority <= 20)
			{
				icount[i]++;
			}      
		}
		
		syst[i] = (struct Process *)malloc(scount[i] * sizeof(struct Process));

		inte[i] = (struct Process *)malloc(icount[i] * sizeof(struct Process));
		
		scount[i] = icount[i] = 0;

		for(j = 0; j < n; j++)
		{
			if(ready[i][j].priority >= 1 && ready[i][j].priority <= 10)
			{
				copy_struct_info(&ready[i][j], &syst[i][scount[i]]);
				scount++;
			}
			else if (ready[i][j].priority >= 11 && ready[i][j].priority <= 20)
			{
				copy_struct_info(&ready[i][j], &inte[i][icount[i]]);
				icount++;
			}        
		}
	}
}