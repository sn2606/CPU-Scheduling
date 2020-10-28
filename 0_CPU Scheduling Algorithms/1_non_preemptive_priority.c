# include <stdio.h>
# include <stdlib.h>
# define n 20
# define L 25


// Main function
int main()
{
      
      float wavg[L] = {0}, tavg[L] = {0}, tsum[L] = {0}, wsum[L] = {0}, throughput[L];
      
      int i, j, temp, temp1, temp2, p[L][n], min[L], k = 1, burst[L] = {0}, pri[L][n];
      int bt[L][n], at[L][n], wt[L][n], rt[L][n], tt[L][n], ta[L] =  {0}, sum[L] = {0};
      
      FILE *fp;
      fp = fopen("final_priority.txt", "r");

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