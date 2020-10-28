// Final file with all the cpu scheduling algorithms

# include <stdio.h>
# include <stdlib.h>
# include "simulator.h"
# define N 20
# define n 20
# define L 25

// //structure for all the processes
// struct Process
// {
// 	int pid;      // process id
// 	int priority; // priority
// 	int AT;       // arrival time
// 	int BT;       // burst time
// 	int WT;       // waiting time
//     int TAT;      // turnaround time
// 	int RT;       // remaining time
// 	int CH;       // check variable
//     int ct;       // Current Time
//     int CT;       //Completion time
// };

// Function to read the input from the input file
void get_input(struct Process **ready, char *c)
{
    int i, j;
    FILE *fp;
    fp = fopen(c, "r");

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


/*********************************************************************************************************************************
 *                                       C PROGRAM TO SIMULATE FIRST COME FIRST SERVE SCHEDULING                                 *
 * ******************************************************************************************************************************/

// Function to calculate wait time of all the processes
void wait_time_FCFS(struct Process *p, int size)
{
    int i;// ind;

    p[0].WT = 0;
    p[0].CH = 1;
    p[0].RT = 0;

    for(i = 1; i < size; i++)
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
void turn_time_FCFS(struct Process *p, int size)
{
    int i;

    for(i = 0; i < size; i++)
    {
        p[i].TAT = p[i].WT + p[i].BT;
    }
}

// The actual function. Return value = 0 if all is good, = 1 if something went wrong
int FCFS(char *input)
{
    
    int i, j; //status, i;

    struct Process **ready;

    ready = (struct Process **)malloc(L * sizeof(struct Process *));

    for(i = 0; i < L; i++)
    {
        ready[i] = (struct Process *)malloc(N * sizeof(struct Process));
    }

    //printf("\n----- FIRST COME FIRST SERVE SCHEDULING -----\n\n");
    

    get_input(ready, input);
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

/*********************************************************************************************************************************
 *                              C PROGRAM TO SIMULATE PRIORITY SCHEDULING  NON - PREEMPTIVE                                      *
 * ******************************************************************************************************************************/

int PS_NON_PREEMPTIVE(char *input)
{
      
      float wavg[L] = {0}, tavg[L] = {0}, tsum[L] = {0}, wsum[L] = {0}, throughput[L];
      
      int i, j, temp, temp1, temp2, p[L][n], min[L], k = 1, burst[L] = {0}, pri[L][n];
      int bt[L][n], at[L][n], wt[L][n], rt[L][n], tt[L][n], ta[L] =  {0}, sum[L] = {0};
      
      FILE *fp;
      fp = fopen(input, "r");

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
                        fscanf(fp, "%d ", &(p[j][i]));
                        fscanf(fp, "%d ", &(pri[j][i]));
                        fscanf(fp, "%d ", &(bt[j][i]));
                        fscanf(fp, "%d\n", &(at[j][i]));
                        wt[j][i] = 0;
                        rt[j][i] = bt[j][i];
                  }
            }
      }
      fclose(fp); 

      for(i = 0; i < L; i++)
      {     
            for(j = 0; j < n; j++)
            {
                  burst[i] = burst[i] + bt[i][j];
                  min[i]   = bt[i][j];

                  for(k = j; k < n; k++)/*main logic*/
                  { 
                        min[i] = pri[i][j];

                        if (burst[i] >= at[i][k])
                        {
                              if(pri[i][k] < min[i])
                              {
                                    temp      = p[i][j];
                                    p[i][j]   = p[i][k];
                                    p[i][k]   = temp;

                                    temp2     = at[i][j];
                                    at[i][j]  = at[i][k];
                                    at[i][k]  = temp2;

                                    temp1     = bt[i][j];
                                    bt[i][j]  = bt[i][k];
                                    bt[i][k]  = temp1;

                                    temp      = pri[i][j];
                                    pri[i][j] = pri[i][k];
                                    pri[i][k] = temp;

                              }
                        }
                  }
            }
      }

      
      for(i = 0; i < L; i++)
      {     
            wt[i][0] = 0;
            for(j = 1; j < n; j++)
            {
                  sum[i]   = sum[i] + bt[i][j - 1];
                  wt[i][j] = sum[i] - at[i][j];
            }
            
            for(j = 0; j < n; j++)
            {
                  wsum[i] += wt[i][j];
                  ta[i]   += bt[i][j];
                  tt[i][j] = ta[i] - at[i][j];
                  tsum[i]  = tsum[i] + tt[i][j];
                  rt[i][j] = 0;
            }

            wavg[i] = wsum[i] / n;
            tavg[i] = tsum[i] / n;
            throughput[i] = ((float)n) / ((float)burst[i]);
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
                  fprintf(fp1, "%f,", wavg[i]);
                  fprintf(fp2, "%f,", tavg[i]);
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

/*********************************************************************************************************************************
 *                                   C PROGRAM TO SIMULATE PRIORITY SCHEDULING  PREEMPTIVE                                       *
 * ******************************************************************************************************************************/

int PS_PREEMPTIVE(char *input)
{
      int i, j, k, largest;
      int BT[L] = {0}, time[L] = {0};
      float wait_time[L] = {0}, TAT[L] = {0}, average_WT[L], average_TAT[L], throughput[L];
      struct Process temp;
      struct Process **ready = (struct Process **)malloc(L * sizeof(struct Process *));

      for(i = 0; i < L; i++)
      {
            ready[i] = (struct Process *)malloc((N + 1) * sizeof(struct Process));
      }

      get_input(ready, input);
      //display_queue(ready[0], N);

      for(j = 0; j < L; j++)
      {
            for(i = 0; i < N; i++)
            {
                BT[j] += ready[j][i].BT;
            }
      }


      for(i = 0; i < L; i++)
      {
            for(k = 0; k < N; k++)
            {
                  for(j = k + 1; j < N; j++)
                  {
                        if(ready[i][k].AT > ready[i][j].AT)
                        {
                              temp = ready[i][k];
                              ready[i][k] = ready[i][j];
                              ready[i][j] = temp;
                        }
                  }
            }
      }

      for(i = 0; i < L; i++)
      {
            ready[i][N].AT = 9999;
            ready[i][N].BT = 0;
            ready[i][N].CH = 1;
            ready[i][N].ct = 9999;
            ready[i][N].pid = 21;
            ready[i][N].priority = 9999;
            ready[i][N].RT = 0;
            ready[i][N].TAT = 0;
            ready[i][N].WT = 0;            
      }

      //display_queue(ready[3], N);

      for(i = 0; i < L; i++)
      {
            ready[i][N].priority = 9999;

            time[i] = ready[i][0].AT;
            int count = 0;

            while(time[i] <= BT[i] && count < N)
            {
                  largest = N;

                  for(j = 0; j < N; j++)
                  {
                        if(ready[i][j].AT <= time[i] && ready[i][j].CH != 1 && ready[i][j].priority < ready[i][largest].priority)
                        {
                          largest = j;
                        }
                  }

                  time[i] = time[i] + ready[i][largest].BT;

                  ready[i][largest].ct = time[i];
                  ready[i][largest].WT = ready[i][largest].ct - ready[i][largest].AT - ready[i][largest].BT;
                  ready[i][largest].TAT = ready[i][largest].ct - ready[i][largest].AT;
                  ready[i][largest].CH = 1;
                  wait_time[i] = wait_time[i] + ready[i][largest].WT;
                  TAT[i] = TAT[i] + ready[i][largest].TAT;
                  count++;
            }

            average_WT[i] = wait_time[i] / N;
            average_TAT[i] = TAT[i] / N;
            throughput[i]  = ((float) N)/((float) BT[i]);
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
                  fprintf(fp1, "%f,", average_WT[i]);
                  fprintf(fp2, "%f,", average_TAT[i]);
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


/*********************************************************************************************************************************
 *                           C PROGRAM TO SIMULATE SHORTEST JOB FIRST SCHEDULING  NON - PREEMPTIVE                               *
 * ******************************************************************************************************************************/

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


// SJF Non Preemptive program
int SJF_NON_PREEMPTIVE(char *input)
{

    int i, j, count, start; 
    int time[L] = {0}, completed[L] = {0}, sum_wait[L] = {0}, sum_turnaround[L] = {0}, total_time[L] = {0};
    float avgwait[L], avgturn[L], throughput[L];

    struct Process **p = (struct Process **)malloc(L * sizeof(struct Process *));

    for(i = 0; i < L; i++)
    {
        p[i] = (struct Process *)malloc(n * sizeof(struct Process));
    }

    get_input(p, input);
    sort_arrival_burst_time(p);

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
            sum_wait[i]       += time[i] - p[i][completed[i]].AT -p[i][completed[i]].BT;
            sum_turnaround[i] += time[i] - p[i][completed[i]].AT;
            completed[i]++;
        }

        avgwait[i] = ((float)sum_wait[i]) / ((float) n);
        avgturn[i] = ((float)sum_turnaround[i]) / ((float) n);
        throughput[i] = ((float) n) / ((float) total_time[i]);
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


/*********************************************************************************************************************************
 *                                 C PROGRAM TO SIMULATE SHORTEST JOB FIRST SCHEDULING PREEMPTIVE                                *
 * ******************************************************************************************************************************/

struct Process readySJFP[L][N];
int flg = 0;
int firstarr, lastcomp;

void sort_SJFP(int readySJFP[2][50],int n1)
{
    int i,j,x,temp;
    for(i=0; i<n1-1; i++){
        x=i;
        for(j=i; j<n1; j++){
            if(readySJFP[1][j] < readySJFP[1][x]){
                x = j;
            }
        }

        temp = readySJFP[1][i];
        readySJFP[1][i] = readySJFP[1][x];
        readySJFP[1][x] = temp;

        temp = readySJFP[0][i];
        readySJFP[0][i] = readySJFP[0][x];
        readySJFP[0][x] = temp;
    }
}

void completion_time(struct Process readySJFP[L][N],int h)
{
    int i,check=1,count = 0;
    while(check==1){
        int arr[2][50], j=0;
        check = 0;
        for(i=0; i<N; i++){
            if(readySJFP[h][i].RT > 0 && readySJFP[h][i].AT<count){
                arr[0][j] = readySJFP[h][i].pid;
                arr[1][j++] = readySJFP[h][i].RT;
            }
        }


        if(j != 0){
            sort_SJFP(arr, j);

            int k = arr[0][0] - 1;
            if(flg ==0){
                firstarr= k;
                flg =1;
            }
            readySJFP[h][k].CT = count;
            readySJFP[h][k].CH = 1;
            readySJFP[h][k].RT--;
        }
        count++;


        for(i =0; i<N; i++){
            if(readySJFP[h][i].CH == 0){
                check = 1;
            }
        }
    }
    int j =0, arr[2][50];
    for(i=0; i<N; i++)
        if(readySJFP[h][i].RT > 0){
            arr[0][j] = readySJFP[h][i].pid;
            arr[1][j++] = readySJFP[h][i].RT;
        }
    sort_SJFP(arr,j);
    count--;
    for(i=0; i<j; i++){
        int k = arr[0][i] - 1;
        if(readySJFP[h][k].RT >0){
            count = readySJFP[h][k].RT = count+ readySJFP[h][k].RT;
            readySJFP[h][k].RT = 0;
            readySJFP[h][k].CT = count;
            lastcomp=readySJFP[h][k].CT;
        }
    }
}

float turn_time_SJFP(struct Process readySJFP[L][N],int h)
{
    int i;
    float avg, temp=0;
    for(i=0; i<N; i++){
        readySJFP[h][i].TAT = readySJFP[h][i].CT - readySJFP[h][i].AT;
        temp =temp+ readySJFP[h][i].TAT;
    }
    avg = temp/N;
    return avg;
}

float wait_time_SJFP(struct Process readySJFP[L][N],int h){
    int i;
    float avg, temp = 0.0;
    for(i=0; i<N; i++)
    {
        readySJFP[h][i].WT = readySJFP[h][i].TAT - readySJFP[h][i].BT;
        temp += readySJFP[h][i].WT;
    }
    avg = temp/N;
    return avg;
}

void get_input_SJFP(struct Process readySJFP[L][N], char input[])
{
    int i;
    FILE *fp;
    fp = fopen(input, "r");

    if(fp == NULL){
        perror("ERROR: No file to be read");
        return;
    }
    else{
        for(i = 0; i < N; i++){
            for(int j = 0; j < L; j++){
                fscanf(fp, "%d ", &(readySJFP[j][i].pid));
                fscanf(fp, "%d ", &(readySJFP[j][i].priority));
                fscanf(fp, "%d ", &(readySJFP[j][i].BT));
                fscanf(fp, "%d\n", &(readySJFP[j][i].AT));
                readySJFP[j][i].CH=0;
                readySJFP[j][i].RT = readySJFP[j][i].BT;
            }
        }
    }
    fclose(fp);
}
void display_queue_SJFP(struct Process readySJFP[L][N], int size, int num){
    int i;

    printf("\n------------------------------------\n");
    printf(" PID\t AT \t BT \tPrio\t RT \n");
    printf("------------------------------------\n");

    for(i = 0; i < size; i++){
        printf("%4d\t%4d\t%4d\t%4d\t%4d\n", readySJFP[num][i].pid, readySJFP[num][i].AT, readySJFP[num][i].BT, readySJFP[num][i].priority, readySJFP[num][i].RT);
    }
}

int SJF_PREEMPTIVE(char input[])
{
    float throughput;
    float total_time[L];
    int i, j, h;

    get_input_SJFP(readySJFP, input);
    printf("\n----- Preemptive Shortest Job First SCHEDULING -----\n\n");

    for(i = 0; i < L; i++)
    {
        for(j = 0; j < N; j++)
        {
            total_time[i] += readySJFP[i][j].BT;
        }
    }

    FILE *fp1, *fp2,*fp3;
    
    fp1 = fopen("op_avg_wt.csv", "a+");
    fp2 = fopen("op_avg_tat.csv", "a+");
    fp3 = fopen("op_throughput.csv", "a+");
    
    if(fp1 == NULL || fp2 == NULL )
    {
        perror("ERROR: No file to be read");
        exit(1);
    }


    for(int h=0;h<L;h++){

        completion_time(readySJFP, h);
        float avtat = turn_time_SJFP(readySJFP, h);
        float avwt = wait_time_SJFP(readySJFP, h);
        throughput = (float)n/(lastcomp - readySJFP[h][firstarr].AT);
        
        fprintf(fp1, "%f,", avwt);
        fprintf(fp2, "%f,", avtat);
        fprintf(fp3, "%f,", throughput);
    }

    fprintf(fp1, "\n");
    fprintf(fp2, "\n");
    fprintf(fp3, "\n");

    fclose(fp1);
    fclose(fp2);
    fclose(fp3);

    return 0;
}


/*********************************************************************************************************************************
 *                                          C PROGRAM TO SIMULATE ROUND ROBIN SCHEDULING                                         *
 * ******************************************************************************************************************************/

int RR(char input[]) 
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

	get_input(p, input);
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

/*********************************************************************************************************************************
 *                                       C PROGRAM TO SIMULATE MULTILEVEL QUEUE SCHEDULING                                       *
 * ******************************************************************************************************************************/

/*****************************************************************
 * System process queue - Priority Scheduling (Preemptive)       *
 * Interactive process queue - Round Robin Scheduling            *
 * Fixed priority preemptive scheduling between the queues       *
 *****************************************************************/

int *scount, *icount;
struct Process **syst, **inte;

// Function to sort the ready queue into different level queues
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

// Multilevel queue scheduling function
int MLQ(char input[])
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

	get_input(ready, input);
	
	sort_arrival_priority_time(ready);

	// display_queue(ready, n, 3);

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

/*********************************************************************************************************************************
 *                                  C PROGRAM TO SIMULATE MULTILEVEL FEEDBACK QUEUE SCHEDULING                                   *
 * ******************************************************************************************************************************/

struct Process Queue1[L][N], Queue2[L][N], Queue3[L][N];

// Functiont to sort arrival time wise in a multilevel feedback queue
void arrivalsortMLFQ(){
    struct Process temp;
    int i,j;
    for(int h=0;h<L;h++){
        for(i=0;i<N;i++){
            for(j=i+1;j<N;j++){
                if(Queue1[h][i].AT>Queue1[h][j].AT){
                        temp = Queue1[h][i];
                        Queue1[h][i] = Queue1[h][j];
                        Queue1[h][j] = temp;
                }
            }
        }
    }

}

// Function to get input for multilevel feedback queue
void get_input_MLFQ(struct Process Queue1[L][N])
{
    int i, j;
    FILE *fp;
    fp = fopen("final_not_priority.txt", "r");

    if(fp == NULL)
    {
        perror("ERROR: No file to be read");
        exit(1);
    }
    else{
        for(i = 0; i < N; i++){
            for(j = 0; j < L; j++){
                fscanf(fp, "%d ", &(Queue1[j][i].pid));
                fscanf(fp, "%d ", &(Queue1[j][i].priority));
                fscanf(fp, "%d ", &(Queue1[j][i].BT));
                fscanf(fp, "%d\n", &(Queue1[j][i].AT));
                Queue1[j][i].RT=Queue1[j][i].BT;
            }
        }
    }

    fclose(fp);

}

// Multilevel feedback queue function
int MLFQ()
{
    int i,j,k=0,r=0,time=0,tQueue1=20,tQueue2=40,flag=0,firstarrival;
    get_input_MLFQ(Queue1);
    arrivalsortMLFQ();
    printf("\n----- Multilevel Feedback SCHEDULING -----\n\n");

    FILE *fp1, *fp2,*fp3;

    fp1 = fopen("op_avg_wt.csv", "a+");
    fp2 = fopen("op_avg_tat.csv", "a+");
    fp3 = fopen("op_throughput.csv", "a+");
        
    if(fp1 == NULL || fp2 == NULL || fp3 == NULL)
    {
        perror("ERROR: No file to be read");
        exit(1);
    }

    for(int h = 0; h < L; h++)
    {
        int waiting=0;
        int turn=0;
        time=0;
        k=0,r=0;
        time=Queue1[h][0].AT;
        firstarrival = time;

        /*printf("\n----------------------------------------------------------\n");
        printf("Process in first queue following RR with qt=20");
        printf("\nProcess\t\tRT\t\tWT\t\tTAT\t\t");*/
        for(i=0;i<N;i++){
          if(Queue1[h][i].AT<=time){
              if(Queue1[h][i].RT<=tQueue1){

                   time+=Queue1[h][i].RT;
                   Queue1[h][i].CT = time;
                   Queue1[h][i].WT=time-Queue1[h][i].AT-Queue1[h][i].BT;
                   Queue1[h][i].TAT=time-Queue1[h][i].AT;
                   waiting +=Queue1[h][i].WT;

                   turn += Queue1[h][i].CT-Queue1[h][i].AT;
                  // printf("\n%d\t\t%d\t\t%d\t\t%d",Queue1[h][i].id,Queue1[h][i].BT,Queue1[h][i].WT,Queue1[h][i].TAT);

              }
              else{
                  Queue2[h][k].WT=time;
                  time+=tQueue1;
                  Queue1[h][i].RT-=tQueue1;
                  Queue2[h][k].BT=Queue1[h][i].RT;
                  Queue2[h][k].RT=Queue2[h][k].BT;
                  Queue2[h][k].AT=Queue1[h][i].AT;
                  Queue2[h][k].pid=Queue1[h][i].pid;
                  k=k+1;
                  flag=1;
               }
            }
            else{
                time = Queue1[h][i].AT;
                i=i-1;
            }
        }
        /*if(flag==1){
          printf("\n----------------------------------------------------------\nProcess in second queue following RR with qt=40");
          printf("\nProcess\t\tRT\t\tWT\t\tTAT\t\t");
        }*/
        for(i=0;i<k;i++){
            if(Queue2[h][i].RT<=tQueue2){
               time+=Queue2[h][i].RT;
               Queue2[h][i].RT=0;
               Queue2[h][i].WT=time-tQueue1-Queue2[h][i].BT;
               Queue2[h][i].TAT=time-Queue2[h][i].AT;
               Queue2[h][i].CT = time;
               waiting +=Queue2[h][i].WT;
               turn += Queue2[h][i].CT-Queue2[h][i].AT;
               //printf("\n%d\t\t%d\t\t%d\t\t%d",Queue2[h][i].id,Queue2[h][i].BT,Queue2[h][i].WT,Queue2[h][i].TAT);

            }
            else{
              Queue3[h][r].AT=time;
              time+=tQueue2;
              Queue2[h][i].RT-=tQueue2;
              Queue3[h][r].BT=Queue2[h][i].RT;
              Queue3[h][r].RT=Queue3[h][r].BT;
              Queue3[h][r].pid=Queue2[h][i].pid;
              Queue3[h][r].AT=Queue2[h][i].AT;
              r=r+1;
              flag=2;
            }
          }

       /* if(flag==2){
            printf("\n----------------------------------------------------------\nProcess in third queue following FCFS ");
            printf("\nProcess\t\tRT\t\tWT\t\tTAT\t\t");
        }*/
        for(i=0;i<r;i++){
            if(i==0){
                    Queue3[h][i].CT=Queue3[h][i].BT+time-tQueue1-tQueue2;
            }
            else{
                Queue3[h][i].CT=Queue3[h][i-1].CT+Queue3[h][i].BT;
            }

        }

        for(i=0;i<r;i++){
                time = time+Queue3[h][i].RT;
                Queue3[h][i].RT=0;
                Queue3[h][i].CT = time;
                Queue3[h][i].TAT=Queue3[h][i].CT;
                Queue3[h][i].WT=Queue3[h][i].TAT-Queue3[h][i].BT;

                waiting +=Queue3[h][i].WT;
                turn += Queue3[h][i].CT-Queue3[h][i].AT;
               // printf("\n%d\t\t%d\t\t%d\t\t%d\t\t",Queue3[h][i].id,Queue3[h][i].BT,Queue3[h][i].WT,Queue3[h][i].TAT);

        }
       //printf("\n----------------------------------------------------------\n");
        float throughput = (float)N/(time - firstarrival);
        float avgwt = (float)waiting/N;

        float avtat =(float)turn/N;

        
        fprintf(fp1, "%f,", avgwt);
        fprintf(fp2, "%f,", avtat);
        fprintf(fp3, "%f,", throughput);
        
    }

    fprintf(fp3, "\n");
    fprintf(fp2, "\n");
    fprintf(fp1, "\n");

    fclose(fp1);
    fclose(fp2);
    fclose(fp3);

    return 0;

}