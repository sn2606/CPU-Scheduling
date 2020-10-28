# include <stdio.h>
# include <stdlib.h>
# define n 20
# define N 20
# define L 25

//structure for all the SJF Preemptive
struct Process{
    int pid;        // Process id
    int priority;   // priority
    int AT;         // arrival time
    int BT;         // burst time
    int WT;         // waiting time
    int TAT;        // turnaround time
    int RT;         // remaining time
    int CH;         // check variable
    int CT;         //Completion time

}readySJFP[L][N];

void get_input_SJFP(struct Process readySJFP[L][N]);
void display_queue_SJFP(struct Process readySJFP[L][N], int size, int num);
void sort_SJFP(int readySJFP[2][50],int n1);
void completion_time(struct Process readySJFP[L][N],int h);
float turn_time_SJFP(struct Process readySJFP[L][N],int h);
float wait_time_SJFP(struct Process readySJFP[L][N],int h);
int flg =0;
int firstarr,lastcomp;


int main()
{
    float throughput;
    float total_time[L];
    int i, j, h;

    get_input_SJFP(readySJFP);
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

void sort_SJFP(int readySJFP[2][50],int n1){
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

void completion_time(struct Process readySJFP[L][N],int h){
    int i,check=1,count = 0;
    //int flag2=0
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
        /*if(j==0){
            for(int i=0;i<N;i++){
                if(readySJFP[h][i].RT!=0){
                    flag2=1;
                }
            }
            if(flag2==1){
                idletime=idletime+1;
            }
        }*/
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

float turn_time_SJFP(struct Process readySJFP[L][N],int h){
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

void get_input_SJFP(struct Process readySJFP[L][N])
{
    int i;
    FILE *fp;
    fp = fopen("final_not_priority.txt", "r");

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
