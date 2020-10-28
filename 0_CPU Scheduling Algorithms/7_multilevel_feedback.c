# include <stdio.h>
# include <stdlib.h>
# define N 20
# define L 25


struct Process
{
    int id;
    int AT;
    int BT;
    int WT;
    int priority;
    int TAT;
    int RT;
    int CT;
}Queue1[L][N],Queue2[L][N],Queue3[L][N];


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
                fscanf(fp, "%d ", &(Queue1[j][i].id));
                fscanf(fp, "%d ", &(Queue1[j][i].priority));
                fscanf(fp, "%d ", &(Queue1[j][i].BT));
                fscanf(fp, "%d\n", &(Queue1[j][i].AT));
                Queue1[j][i].RT=Queue1[j][i].BT;
            }
        }
    }

    fclose(fp);

}
int main()
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
                  Queue2[h][k].id=Queue1[h][i].id;
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
              Queue3[h][r].id=Queue2[h][i].id;
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