# include <stdio.h>
# include <stdlib.h>
# define N 20
# define L 25

// Structure for all the processes
struct Process
{
      int pid;       // Process ID
      int priority;  // Priority
      int AT;        // Arrival Time
      int BT;        // Burst Time
      int ct;        // Current Time
      int WT;        // Waiting Time
      int TAT;       // Turnaround Time
      int RT;        // Remaining Time
      int CH;        // Check / Status Variable
};

void get_input(struct Process **ready);
void display_queue(struct Process *p, int size);

int main()
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

      get_input(ready);
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
                fscanf(fp, "%d", &(ready[j][i].AT));
                ready[j][i].CH = 0;
                ready[j][i].WT = 0;
                ready[j][i].RT = ready[j][i].BT;
            }
        }
    }
    fclose(fp);
}


// Function to display a Process queue
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
